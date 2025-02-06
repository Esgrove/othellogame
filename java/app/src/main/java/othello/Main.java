package othello;

import picocli.CommandLine;
import picocli.CommandLine.Command;
import picocli.CommandLine.Option;
import picocli.CommandLine.Parameters;

import java.util.Scanner;
import java.util.concurrent.Callable;

@Command(name = "othello_java",
    mixinStandardHelpOptions = true,
    version = "Othello Java 1.0",
    description = "A simple Othello CLI game implementation in Java.")
public class Main implements Callable<Integer> {
    private static final int MIN_BOARD_SIZE = 4;
    private static final int MAX_BOARD_SIZE = 10;
    private static final int DEFAULT_BOARD_SIZE = 8;

    @Parameters(index = "0", description = "Board size (" + MIN_BOARD_SIZE + ".." + MAX_BOARD_SIZE + ")", arity = "0..1")
    private Integer boardSize;

    @Option(names = {"-a", "--autoplay"}, description = "Enable autoplay mode")
    private boolean autoplay;

    @Option(names = {"-c", "--check"}, description = "Enable check mode (implies --test and --autoplay)")
    private boolean checkMode;

    @Option(names = {"-d", "--default"}, description = "Play with default settings")
    private boolean useDefaults;

    @Option(names = {"-l", "--log"}, description = "Show log after a game")
    private boolean showLog;

    @Option(names = {"-n", "--no-helpers"}, description = "Hide disk placement hints")
    private boolean noHelpers;

    @Option(names = {"-t", "--test"}, description = "Enable test mode")
    private boolean testMode;

    //@Option(names = {"-v", "--version"}, versionHelp = true, description = "Print version and exit")
    //private boolean version;

    @Override
    public Integer call() {
        System.out.println("\u001B[32m\u001B[1mOTHELLO GAME - JAVA\u001B[0m");

        if (boardSize == null) {
            if (autoplay || useDefaults) {
                boardSize = DEFAULT_BOARD_SIZE;
                System.out.println("Using default board size: " + boardSize);
            } else {
                boardSize = getBoardSize();
            }
        }

        // Validate board size
        if (boardSize < MIN_BOARD_SIZE || boardSize > MAX_BOARD_SIZE) {
            System.err.println("Error: Unsupported board size: " + boardSize);
            return 1;
        }

        System.out.println("Using board size: " + boardSize);

        Settings settings = new Settings(
            boardSize,
            autoplay || checkMode,
            checkMode,
            useDefaults,
            !noHelpers,
            showLog,
            testMode || checkMode
        );
        System.out.printf(settings.toString());
        //new Othello(settings).play();
        return 0;
    }

    public static int getBoardSize() {
        System.out.printf("Choose board size (default is %d): ", DEFAULT_BOARD_SIZE);

        String input = System.console() != null ? System.console().readLine() : new Scanner(System.in).nextLine();

        if (input == null || input.trim().isEmpty()) {
            System.out.printf("\u001B[33mInvalid size, defaulting to %d...\u001B[0m%n", DEFAULT_BOARD_SIZE);
            return DEFAULT_BOARD_SIZE;
        }

        try {
            int boardSize = Integer.parseInt(input.trim());
            if (boardSize < MIN_BOARD_SIZE || boardSize > MAX_BOARD_SIZE) {
                System.out.printf("\u001B[33mLimiting board size to valid range %d...%d\u001B[0m%n", MIN_BOARD_SIZE, MAX_BOARD_SIZE);
            }
            return Math.max(MIN_BOARD_SIZE, Math.min(boardSize, MAX_BOARD_SIZE));
        } catch (NumberFormatException ignored) {
        }

        System.out.printf("\u001B[33mInvalid size, defaulting to %d...\u001B[0m%n", DEFAULT_BOARD_SIZE);
        return DEFAULT_BOARD_SIZE;
    }

    public static void main(String[] args) {
        int exitCode = new CommandLine(new Main()).execute(args);
        System.exit(exitCode);
    }
}
