import java.io.*;
import java.util.Scanner;
import java.util.ArrayList;
import java.util.List;
import java.lang.Runtime;

public class Project1v1 {
    public static void main(String args[]) {
        try {
            int x;

            String fileName = args[0];


            Runtime rt = Runtime.getRuntime();

            Process proc = rt.exec(fileName);
            // String filename = "example1.txt";

            // System.out.println("Is this the file you want to open? " + filename);

            List<Integer> instr = fileManager(fileName);

            // System.out.println("You made it back to main!!");

            for(int i = 0; i < instr.size(); i++){
                System.out.println(instr.get(i));
            }


        } catch (Throwable t) {
            t.printStackTrace();
        }
    }
    
    public static List<Integer> fileManager(String fileName) {
        // This functions opens and parses the given file 
        try {
            // Setup to parse the file
            File file = new File(fileName);
            Scanner in = new Scanner(file); 
            List<Integer> instruction = new ArrayList<Integer>();

            while (in.hasNextLine()) {
                // Parse by line and split by space
                String line = in.nextLine();
                String[] words = line.split(" ");

                int curIns = -1;
                
                // Send only the instruction to verify int
                curIns = getInt(words[0]);

                // Verifies that it's not the initialized var 
                // and adds to the array of instructions
                if(curIns != -1) {
                    instruction.add(curIns);
                }
            }
            in.close();
            return instruction;

        } catch (FileNotFoundException e) {
            System.out.println("An error occurred.");
            e.printStackTrace();
        }
        return null;
    }

    public static int getInt(String curr) {
        // Verify that the first item was an int and pass back
        int curIns = -1;
        try {
            curIns = Integer.parseInt(curr);
        } catch (Exception e) {
            // TODO: handle exception
        }
        return curIns;
    }

    public static void CPU(){

    }

    public static void mainMem(){

    }
}