import java.io.*;
import java.util.Scanner;

public class Memory {

    final static int[] mainMem = new int[2000];
    public static void main(String args[]) {
        System.out.println("Howdy from Memory!");

        try {
            System.out.println("Howdy from the try!");

            Scanner scProj1 = new Scanner(System.in);
            System.out.println("Howdy from the try!");

            // File inFile = new File(scProj1.nextLine());

            String inFile = (scProj1.nextLine());

            System.out.println(inFile);

            if(scProj1.hasNextLine()) {
                System.out.println(inFile);
            }
            scProj1.close();
        } catch (Exception e) {
            // TODO: handle exception
        }
    }
}
