package utils;

import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;
import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;

public class Logger {

    private static final String LOG_FILE_PATH = "logs/log.txt";
    private static final String ANSI_RESET = "\u001B[0m";
    private static final String ANSI_CYAN = "\u001B[36m";
    private static final String ANSI_RED = "\u001B[31m";
    private static final String ANSI_GREEN = "\u001B[32m";
    private static final String ANSI_YELLOW = "\u001B[33m";
    private static final DateTimeFormatter DATE_FORMATTER = DateTimeFormatter.ofPattern("yyyy-MM-dd HH:mm:ss");

    public static void info(String message) {
        writeToLog("[*] " + message);
        System.out.println(ANSI_CYAN + "[*] " + message + ANSI_RESET);
    }

    public static void error(String message) {
        writeToLog("[-] " + message);
        System.out.println(ANSI_RED + "[-] " + message + ANSI_RESET);
    }

    public static void success(String message) {
        writeToLog("[+] " + message);
        System.out.println(ANSI_GREEN + "[+] " + message + ANSI_RESET);
    }

    public static void warning(String message) {
        writeToLog("[!] " + message);
        System.out.println(ANSI_YELLOW + "[!] " + message + ANSI_RESET);
    }

    private static void writeToLog(String message) {
        LocalDateTime currentTime = LocalDateTime.now();
        String formattedDateTime = DATE_FORMATTER.format(currentTime);
        String logEntry = formattedDateTime + " " + message;

        try (PrintWriter writer = new PrintWriter(new FileWriter(LOG_FILE_PATH, true))) {
            writer.println(logEntry);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
