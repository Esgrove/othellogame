package othello;

import picocli.CommandLine;
import picocli.CommandLine.Command;
import picocli.CommandLine.Option;
import picocli.CommandLine.Parameters;

import java.util.Scanner;
import java.util.concurrent.Callable;

import static othello.ColorPrint.printBold;
import static othello.ColorPrint.printError;
import static othello.ColorPrint.printColor;
import static othello.ColorPrint.printWarn;

@Command(
    name = "othello_java",
    mixinStandardHelpOptions = true,
    version = VersionInfo.VERSION_STRING,
    description = "A simple Othello CLI game implementation in Java\n",
    customSynopsis = "java -jar othello_java [<options>] [<size>]\n"
)
public class Main implements Callable<Integer> {
    private static final int MIN_BOARD_SIZE = 4;
    private static final int MAX_BOARD_SIZE = 10;
    private static final int DEFAULT_BOARD_SIZE = 8;

    @Parameters(
        index = "0",
        description = "Optional board size (" + MIN_BOARD_SIZE + ".." + MAX_BOARD_SIZE + ")",
        paramLabel = "size",
        arity = "0..1"
    )
    private Integer size;

    @Option(names = {
        "-a", "--autoplay"
    },
        description = "Enable autoplay mode with computer control"
    )
    private boolean autoplay;

    @Option(names = {
        "-c", "--check"
    },
        description = "Autoplay and only print result"
    )
    private boolean checkMode;

    @Option(names = {
        "-d", "--default"
    },
        description = "Play with default settings"
    )
    private boolean useDefaults;

    @Option(names = {
        "-l", "--log"
    },
        description = "Show game log at the end"
    )
    private boolean showLog;

    @Option(names = {
        "-n", "--no-helpers"
    },
        description = "Hide disk placement hints"
    )
    private boolean noHelpers;

    @Option(names = {
        "-t", "--test"
    },
        description = "Enable test mode with deterministic computer moves"
    )
    private boolean testMode;

    @Override
    public Integer call() {
        printBold("OTHELLO GAME - JAVA", AnsiColor.GREEN);

        Integer boardSize = resolveBoardSize(size, autoplay, useDefaults);

        Settings settings = new Settings(
            boardSize,
            autoplay || checkMode,
            checkMode,
            !noHelpers,
            showLog || checkMode,
            testMode || checkMode,
            useDefaults
        );

        new Othello(settings).play();
        return 0;
    }

    private static int getBoardSize() {
        System.out.printf("Choose board size (default is %d): ", DEFAULT_BOARD_SIZE);

        String input = System.console() != null ? System.console().readLine() : new Scanner(System.in).nextLine();

        if (input == null || input.trim().isEmpty()) {
            printColor("Invalid size, defaulting to %d...", AnsiColor.YELLOW);
            return DEFAULT_BOARD_SIZE;
        }

        try {
            int boardSize = Integer.parseInt(input.trim());
            if (boardSize < MIN_BOARD_SIZE || boardSize > MAX_BOARD_SIZE) {
                printColor(
                    "Limiting board size to valid range " + MIN_BOARD_SIZE + ".." + MAX_BOARD_SIZE,
                    AnsiColor.YELLOW
                );
            }
            return Utils.clamp(boardSize, MIN_BOARD_SIZE, MAX_BOARD_SIZE);
        } catch (NumberFormatException ignored) {}

        printWarn("Invalid size, defaulting to " + DEFAULT_BOARD_SIZE + "...");
        return DEFAULT_BOARD_SIZE;
    }

    private Integer resolveBoardSize(Integer size, boolean autoplay, boolean useDefaults) {
        // Try to read board size from command line args
        if (size != null) {
            if (size < MIN_BOARD_SIZE || size > MAX_BOARD_SIZE) {
                printError("Unsupported board size: " + size);
                System.exit(1);
            }
            System.out.println("Using board size: " + size);
            return size;
        } else if (autoplay || useDefaults) {
            return DEFAULT_BOARD_SIZE;
        } else {
            // Otherwise ask the user for board size
            return getBoardSize();
        }
    }

    public static void main(String[] args) {
        int exitCode = new CommandLine(new Main()).execute(args);
        System.exit(exitCode);
    }
}
