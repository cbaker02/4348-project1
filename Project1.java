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
        try {
            File file = new File(fileName);
            Scanner in = new Scanner(file);

            while (in.hasNextLine()) {
                String line = in.nextLine();
                System.out.println(line);
            }

            in.close();
        } catch (FileNotFoundException e) {
            System.out.println("An error occurred.");
            e.printStackTrace();
        }
    }
}