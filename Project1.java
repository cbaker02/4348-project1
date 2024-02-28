import java.io.*;
import java.util.Scanner;
import java.lang.Runtime;
import java.util.Random;

public class Project1{
    // Default registers
    static int PC = 0;
    static int SP = 1000;
    static int IR;
    static int AC;
    static int X;
    static int Y;

    static int timerFlag;
    static int numInst = 0;

    static int systemStack = 2000;
    static int userStack = 1000;

    static boolean userMode = true;         // Changes on interrupt
    static boolean procInterrupt = false;   // No nested interrupts
    public static void main(String args[]) {

        String filename = "example.txt";

        try{ 
            Memory mem = new Memory();
            /*
            mem.memory();
            */

            Runtime rt = Runtime.getRuntime();
            Process proc = rt.exec(mem);
        }  catch (Throwable t) {
            t.printStackTrace();
        }
    }
}

class Memory {
    void memory() {
        System.out.println("In memory!");
    }
   
}