import java.io.*;
import java.util.Scanner;
import java.lang.Runtime;

public class Project1{
    public static void main(String args[]) {
        // String file = args[0];
        String file = "example1.txt";

        try{
            Runtime rt = Runtime.getRuntime();

            // Process p1 = rt.exec("javac Memory.java");
            Process proc = rt.exec("java Memory");

            InputStream is = proc.getInputStream();
            OutputStream os = proc.getOutputStream();
            PrintWriter pw = new PrintWriter(os);

            Scanner memScanner = new Scanner(is);

            pw.printf(file + "\n");
            pw.flush();

            Scanner sc = new Scanner(is);

            System.out.println("Howdy");

            while(true){
                int currVal = readMem(pw, is, memScanner, os, 0);

                System.out.println(currVal);
            }
            // sc.close();

        } catch(Throwable t) {
            t.printStackTrace();
        }
    }

    private static int readMem(PrintWriter pw, InputStream is, Scanner memScanner, OutputStream os, int address) {
        pw.printf("1, " + address + "\n");
        pw.flush();

        if(memScanner.hasNext()) {
            String temp = memScanner.next();

            if(!temp.isEmpty()){
                int tempInstr = Integer.parseInt(temp);
                return (tempInstr);
            }
        }
        return -1;
    }
}