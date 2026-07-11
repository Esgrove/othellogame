package othello;

import picocli.CommandLine;
import picocli.CommandLine.Command;
import picocli.CommandLine.Model.CommandSpec;
import picocli.CommandLine.Option;
import picocli.CommandLine.ParameterException;
import picocli.CommandLine.Parameters;
import picocli.CommandLine.Spec;

import java.util.concurrent.Callable;

import static othello.ColorPrint.printError;
import static othello.ColorPrint.printGreenBold;
import static othello.Settings.DEFAULT_BOARD_SIZE;
import static othello.Settings.MAX_BOARD_SIZE;
import static othello.Settings.MIN_BOARD_SIZE;

/**
 * Play Othello (Reversi) on the command line.
 * <a href="https://en.wikipedia.org/wiki/Reversi">https://en.wikipedia.org/wiki/Reversi</a>
 */
@Command(
    name = "othello_java",
    description = "A simple Othello CLI game implementation in Java\n",
    customSynopsis = "java -jar othello_java [<options>] [<size>]\n"
)
public class Main implements Callable<Integer> {
    @Parameters(
        index = "0",
        description = "Optional board size (" + MIN_BOARD_SIZE + ".." + MAX_BOARD_SIZE
            + ")",
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
    private boolean check;

    @Option(names = {
        "-d", "--default"
    },
        description = "Play with default settings"
    )
    private boolean useDefaults;

    @Option(names = {
        "-h", "--help"
    },
        usageHelp = true,
        description = "Show this help message and exit"
    )
    private boolean help;

    @Option(names = {
        "-l", "--log"
    },
        description = "Show game log at the end"
    )
    private boolean log;

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
    private boolean test;

    // Custom version flag instead of the picocli default,
    // since picocli uses uppercase `-V` for the short version option.
    @Option(names = {
        "-v", "--version"
    },
        description = "Print version and exit"
    )
    private boolean version;

    @Spec
    private CommandSpec spec;

    @Override
    public Integer call() {
        if (version) {
            System.out.println(VersionInfo.versionInfo());
            return 0;
        }

        if (autoplay && useDefaults) {
            throw new ParameterException(
                spec.commandLine(),
                "-a/--autoplay cannot be used with -d/--default"
            );
        }

        printGreenBold("OTHELLO GAME - JAVA");

        int boardSize = resolveBoardSize();

        Settings settings = new Settings(
            boardSize,
            autoplay || check,
            check,
            !noHelpers,
            log || check,
            test || check,
            useDefaults
        );

        new Othello(settings).play();
        return 0;
    }

    /**
     * Resolve the board size to use from CLI arguments, or by asking the user.
     */
    private int resolveBoardSize() {
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
            // Otherwise ask user for board size
            return Othello.getBoardSize();
        }
    }

    public static void main(String[] args) {
        int exitCode = new CommandLine(new Main()).execute(args);
        System.exit(exitCode);
    }
}
