import java.io.*;
import java.util.Scanner;
import java.lang.Runtime;

public class Project1 {
    public static void main(String args[]) {
        try {
            int x;


            String filename = args[0];
            Runtime rt = Runtime.getRuntime();

            File fileName = new File(args[0]);

            System.out.println("Is this the file you want to open? " + fileName);

            fileManager(filename);

            System.out.println("You made it back to main!!");
            
        } catch (Throwable t) {
            t.printStackTrace();
        }
    }
    
    public static void fileManager(String fileName) {
        System.out.println("You made it to the file manager!");
    }
}