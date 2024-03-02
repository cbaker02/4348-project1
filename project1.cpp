#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <string>
#include <fstream>
#include <ctype.h>

using namespace std;


int * loadInput(const char* fileName);

int main(int argc, char** argv) {
    /* Add in error verification*/


    string fileName = argv[1];
    int time = atoi(argv[2]);

    // string fileName = "sample1.txt";
    // int time = 30;

    // Piping

    int CPUtoMem[2];
    int MemtoCPU[2];
    int r1 = pipe(CPUtoMem);
    int r2 = pipe(MemtoCPU);

    // Check that pipe worked
    if(r1 == -1 || r2 == -1) {
        exit(1);
    }

    char buf[30];

    fflush(0);
    int z = fork();

    // Check fork
    if(z < 0) {
        exit(-1);
    } else if(z == 0) {
        // Memory 
    
        int *memory;

        memory = loadInput(fileName.c_str());

        int PC;
        string temp;
        int value;
        int address;

        while(true) {
            read(CPUtoMem[0], &PC, sizeof(PC));

            
            if(PC == -1) {
                // Write

                read(CPUtoMem[0], &address, sizeof(address));
                read(CPUtoMem[0], &value, sizeof(value));
                memory[address] = value;

            } else {
                // Read
                int instr = memory[PC];
                write(MemtoCPU[1], &instr, sizeof(instr));
                
            }
        }

    } else {
        // CPU

        // Initialize registers
        int PC = 0;
        int SP = 1000;
        int IR = 0;
        int AC = 0;
        int X = 0;
        int Y = 0;
        string tempString = "";

        int timerCount = 0;

        int tempSP;

        int timerFlag = 0;
        int numInst = 0;

        int local_reg;

        int systemStack = 2000;
        int userStack = 1000;

        bool kernelMode = false;            // Changes on interrupt (kernel)
        bool procInterrupt = false;         // No nested interrupts

        int writeFlag = -1;
        
        while(true) {

            // Timer Implementation
            if(procInterrupt && timerFlag == 0) {
                procInterrupt = false;
                timerFlag = 2;

                kernelMode = true;

                tempSP = SP;
                SP = 2000;


                // PC to Stack
                SP--;
                PC++;

                // Write
                write(CPUtoMem[1], &writeFlag, sizeof(writeFlag));
                // Add to stack
                write(CPUtoMem[1], &SP, sizeof(SP));
                // Return Address
                write(CPUtoMem[1], &PC, sizeof(PC));

                // SP to Stack
                SP--;
                // Write
                write(CPUtoMem[1], &writeFlag, sizeof(writeFlag));
                // Add to Stack
                write(CPUtoMem[1], &SP, sizeof(SP));
                // Return Address
                write(CPUtoMem[1], &tempSP, sizeof(tempSP));

                PC = 1000;
            }

            // Get next instruction
            write(CPUtoMem[1], &PC, sizeof(PC));
            read(MemtoCPU[0], &IR, sizeof(IR));

            switch(IR) {
                // Load Value
                case 1:
                    PC++;

                    write(CPUtoMem[1], &PC, sizeof(PC));
                    read(MemtoCPU[0], &numInst, sizeof(numInst));

                    AC = numInst;
                    break;
                
                // Load Address
                case 2:
                    PC++;

                    write(CPUtoMem[1], &PC, sizeof(PC));
                    read(MemtoCPU[0], &numInst, sizeof(numInst));

                    if(numInst >= 1000 && kernelMode == false) {
                        cout << "Memory violation: accessing system address "<< numInst <<" in user mode " << '\n';
                    }

                    write(CPUtoMem[1], &numInst, sizeof(numInst));
                    read(MemtoCPU[0], &numInst, sizeof(numInst));

                    // Save to AC
                    AC= numInst;
                    break;

                // LoadInd address
                case 3:
                    PC++;

                    write(CPUtoMem[1], &PC, sizeof(PC));
                    read(MemtoCPU[0], &numInst, sizeof(numInst));

                    write(CPUtoMem[1], &numInst, sizeof(numInst));
                    read(MemtoCPU[0], &numInst, sizeof(numInst));

                    if(numInst >= 1000 && kernelMode == false) {
                        cout << "Memory violation: accessing system address "<< numInst <<" in user mode " << '\n';
                    }

                    write(CPUtoMem[1], &numInst, sizeof(numInst));
                    read(MemtoCPU[0], &numInst, sizeof(numInst));
                    AC = numInst;
                    break;

                // LoadIdxX addr
                case 4:
                    PC++;

                    write(CPUtoMem[1], &PC, sizeof(PC));
                    read(MemtoCPU[0], &numInst, sizeof(numInst));

                    numInst += X;

                    write(CPUtoMem[1], &numInst, sizeof(numInst));
                    read(MemtoCPU[0], &numInst, sizeof(numInst));

                    AC = numInst;
                    break;

                // LoadIDxY addr
                case 5:
                    PC++;

                    write(CPUtoMem[1], &PC, sizeof(PC));
                    read(MemtoCPU[0], &numInst, sizeof(numInst));

                    numInst += Y;

                    write(CPUtoMem[1], &numInst, sizeof(numInst));
                    read(MemtoCPU[0], &numInst, sizeof(numInst));

                    AC = numInst;
                    break;

                // LoadSpX
                case 6:
                    numInst = SP + X;

                    write(CPUtoMem[1], &numInst, sizeof(numInst));
                    read(MemtoCPU[0], &numInst, sizeof(numInst));

                    AC = numInst;

                    break;

                // Store addr
                case 7:
                    PC++;
                    write(CPUtoMem[1], &PC, sizeof(PC));
                    read(MemtoCPU[0], &numInst, sizeof(numInst));

                    write(CPUtoMem[1], &writeFlag, sizeof(writeFlag));
                    write(CPUtoMem[1], &numInst, sizeof(numInst));
                    write(CPUtoMem[1], &AC, sizeof(AC));
                    break;

                // Get
                case 8:
                    AC = rand() % 100 + 1;
                    break;

                // Put port
                case 9:
                    PC++;

                    write(CPUtoMem[1], &PC, sizeof(PC));
                    read(MemtoCPU[0], &numInst, sizeof(numInst));

                    if(numInst == 1) {
                        printf("%i", AC);
                    } else if(numInst == 2) {
                        printf("%c", AC);
                    }

                    break;

                // AddX
                case 10:
                    AC += X;
                    break;

                // AddY
                case 11:
                    AC += Y;
                    break;

                // SubX
                case 12:
                    AC -= X;
                    break;

                // SubY
                case 13: 
                    AC -= Y;
                    break;

                // CopytoX
                case 14:
                    X = AC;
                    break;

                // CopyfromX
                case 15:
                    AC = X;
                    break;

                // CopytoY
                case 16:
                    Y = AC;
                    break;

                // CopyfromY
                case 17:
                    AC = Y;
                    break;

                // CopytoSP
                case 18:
                    SP = AC;
                    break;

                // CopyfromSP
                case 19:
                    AC = SP;
                    break;

                // Jump addr
                case 20:
                    PC++;
                    write(CPUtoMem[1], &PC, sizeof(PC));
                    read(MemtoCPU[0], &numInst, sizeof(numInst));

                    PC = numInst - 1;
                    break;

                // JumpIfEqual addr
                case 21:
                    PC++;
                    write(CPUtoMem[1], &PC, sizeof(PC));
                    read(MemtoCPU[0], &numInst, sizeof(numInst));

                    if(AC == 0) {
                        PC = numInst - 1;
                    }

                    break;

                // JumpIfNotEqual addr
                case 22:
                    PC++;
                    write(CPUtoMem[1], &PC, sizeof(PC));
                    read(MemtoCPU[0], &numInst, sizeof(numInst));

                    if(AC != 0) {
                        PC = numInst - 1;
                    }

                    break;

                // Call addr
                case 23:
                    PC++;
                    write(CPUtoMem[1], &PC, sizeof(PC));
                    read(MemtoCPU[0], &numInst, sizeof(numInst));

                    SP--;
                    PC++;

                    write(CPUtoMem[1], &writeFlag, sizeof(writeFlag));
                    write(CPUtoMem[1], &SP, sizeof(SP));
                    write(CPUtoMem[1], &PC, sizeof(PC));

                    PC = numInst - 1;
                    break;

                // Ret
                case 24:
                    // Pop from stack
                    write(CPUtoMem[1], &SP, sizeof(SP));

                    // Jump to Addr
                    read(MemtoCPU[0], &PC, sizeof(PC));

                    PC--;
                    SP++;
                    break;

                // IncX
                case 25:
                    X++;
                    break;


                // DecX
                case 26:
                    X--;
                    break;

                // Push
                case 27:
                    SP--;
                    write(CPUtoMem[1], &writeFlag, sizeof(writeFlag));
                    write(CPUtoMem[1], &SP, sizeof(SP));
                    write(CPUtoMem[1], &AC, sizeof(AC));
                    break;

                // Pop
                case 28:
                    write(CPUtoMem[1], &SP, sizeof(SP));
                    read(MemtoCPU[0], &AC, sizeof(AC));

                    SP++;
                    break;

                // Int
                case 29:
                    if(timerFlag != 0) {
                        break;
                    }

                    timerFlag = 1;

                    kernelMode = true;

                    tempSP = SP;
                    SP = 2000;

                    // Save Registers to stack

                    // PC to Stack
                    SP--;
                    write(CPUtoMem[1], &writeFlag, sizeof(writeFlag));
                    write(CPUtoMem[1], &SP, sizeof(SP));
                    write(CPUtoMem[1], &PC, sizeof(PC));

                    // SP to Stack
                    SP--;
                    write(CPUtoMem[1], &writeFlag, sizeof(writeFlag));
                    write(CPUtoMem[1], &SP, sizeof(SP));
                    write(CPUtoMem[1], &tempSP, sizeof(tempSP));

                    PC = 1499;
                    break;

                // IRet
                case 30:
                    // Pop Return address
                    write(CPUtoMem[1], &SP, sizeof(SP));
                    read(MemtoCPU[0], &tempSP, sizeof(tempSP));

                    SP++;

                    // Pop Return Adress
                    write(CPUtoMem[1], &SP, sizeof(SP));
                    read(MemtoCPU[0], &PC, sizeof(PC));

                    PC -= 2;
                    SP++;

                    // Unflag
                    if(timerFlag == 2) {
                        procInterrupt = false;
                    }
                    timerFlag = 0;

                    SP = tempSP;

                    kernelMode = false;
                    break;

                // End
                case 50:
                    _exit(0);
                    break;

                default:
                    cout << "ERROR\n";
            }

            timerCount++;
            PC++;

            if(timerCount % time == 0) {
                procInterrupt = true;
            }
        }
    }

    return 0;

}

int * loadInput(const char* fileName) {
    FILE *ptrFile;
        char buf[1000];

    char insStr[50];
    static int memory[2000];
    int line = 0;

    int n = sizeof(memory) / sizeof(memory[0]);

    for(size_t i = 0; i < n; i++) {
        memory[i] = 0;
    }

        ptrFile = fopen(fileName, "r");
        if(!ptrFile) {
            printf("Error opening file");
            return memory;
        }

        while(fgets(buf, sizeof(buf), ptrFile) != NULL) {
            int i = (int) buf[0];
            int c = (char) buf[0];

            if(isdigit(i) == 1 || (buf[0] == '.')) {
                int num;
                if(buf[0] == '.') {
                    sscanf(buf, ".%d %*s\n", &num);
                    line = num;
                    memory[line] = num;
                } else {
                    sscanf(buf, "%d %*s\n", &num);
                    memory[line] = num;
                    line++;
                }
            }
        }
        fclose(ptrFile);
    return memory;
}