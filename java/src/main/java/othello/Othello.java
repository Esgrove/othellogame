package othello;

import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;
import java.util.stream.Collectors;
import java.util.stream.IntStream;

import othello.Models.Disk;

import static othello.Settings.DEFAULT_BOARD_SIZE;
import static othello.Settings.MAX_BOARD_SIZE;
import static othello.Settings.MIN_BOARD_SIZE;

/**
 * Gameplay loop and main logic.
 */
public class Othello {
    private Board board;
    private final Settings settings;
    private final Player playerBlack;
    private final Player playerWhite;
    private final List<String> gameLog = new ArrayList<>();
    private int gamesPlayed = 0;
    private int roundsPlayed = 0;

    private static final Scanner scanner = new Scanner(System.in);

    /**
     * Initialize Othello game.
     */
    public Othello(Settings settings) {
        this.board = new Board(settings.boardSize());
        this.settings = settings;
        this.playerBlack = Player.black(settings.toPlayerSettings());
        this.playerWhite = Player.white(settings.toPlayerSettings());
    }

    /**
     * Play one full game of Othello.
     */
    public void play() {
        do {
            initGame();
            gameLoop();
            printResult();
            if (settings.showLog()) {
                printLog();
            }
        } while (!settings.autoplayMode() && getAnswer("Would you like to play again", "y", "n"));
    }

    /**
     * Initialize game board and players for a new game.
     */
    private void initGame() {
        // Re-use existing objects instead of initializing new ones
        if (gamesPlayed > 0) {
            board = new Board(settings.boardSize());
            playerBlack.reset();
            playerWhite.reset();
            roundsPlayed = 0;
            gameLog.clear();
        }
        if (settings.autoplayMode()) {
            // Computer plays both
            playerBlack.setComputer();
            playerWhite.setComputer();
        } else if (settings.useDefaults()) {
            // Default: play as black against white computer player
            playerWhite.setComputer();
        } else if (getAnswer("Would you like to play against the computer", "y", "n")) {
            if (getAnswer("Would you like to play as black or white", "b", "w")) {
                playerWhite.setComputer();
            } else {
                playerBlack.setComputer();
            }
        }
        if (!settings.checkMode()) {
            ColorPrint.printBold("\nPlayers:");
            printStatus();
        }
    }

    /**
     * Keep making moves until both players can't make a move any more.
     */
    private void gameLoop() {
        while (board.canPlay() && (playerBlack.canPlay() || playerWhite.canPlay())) {
            roundsPlayed++;
            printRoundHeader();
            for (Player player : new Player[] {
                playerBlack, playerWhite
            }) {
                String result = player.playOneMove(board);
                if (result != null) {
                    gameLog.add(result + ";" + board.logEntry());
                }
                if (!settings.checkMode()) {
                    System.out.println("--------------------------------");
                }
            }
        }
        gamesPlayed++;
        printGameEndFooter();
    }

    /**
     * Format game log with line numbers for each move.
     */
    private String formatGameLog() {
        return IntStream.range(0, gameLog.size())
            .mapToObj(i -> String.format("%02d: %s", i + 1, gameLog.get(i)))
            .collect(Collectors.joining("\n"));
    }

    /**
     * Print header for the current round.
     */
    private void printRoundHeader() {
        if (!settings.checkMode()) {
            ColorPrint.printBold("\n=========== ROUND: " + roundsPlayed + " ===========");
        }
    }

    /**
     * Print footer after the game has ended.
     */
    private void printGameEndFooter() {
        if (!settings.checkMode()) {
            ColorPrint.printBold("\n================================");
            ColorPrint.printGreenBold("The game is finished!\n");
        }
    }

    /**
     * Print game log which shows all moves made and the game board state after each move.
     */
    private void printLog() {
        String formattedLog = formatGameLog();
        if (!settings.checkMode()) {
            ColorPrint.printYellowBold("Game log:");
            System.out.println(formattedLog);
        }
        String hexHash = Utils.calculateSha256(formattedLog);
        System.out.println(hexHash);
    }

    /**
     * Print ending status and winner info.
     */
    private void printResult() {
        ColorPrint.printBold("Result:");
        printStatus();
        System.out.println();

        Disk winner = board.result();
        if (winner == Disk.EMPTY) {
            System.out.println("The game ended in a tie...\n");
        } else {
            System.out.println("The winner is " + winner.diskString() + "!\n");
        }
    }

    /**
     * Print current board and player info.
     */
    private void printStatus() {
        System.out.println(playerBlack);
        System.out.println(playerWhite + "\n");
        System.out.println(board);
    }

    /**
     * Ask a question with two options, and return bool from user answer.
     */
    private static boolean getAnswer(String question, String yes, String no) {
        System.out.print(question + " (" + yes + "/" + no + ")? ");
        String input = scanner.nextLine();
        return input.trim().toLowerCase().equals(yes);
    }

    /**
     * Ask and return the desired board size.
     */
    public static int getBoardSize() {
        System.out.print("Choose board size (default is " + DEFAULT_BOARD_SIZE + "): ");
        String input = scanner.nextLine();
        try {
            int boardSize = Integer.parseInt(input.trim());
            if (boardSize < MIN_BOARD_SIZE || boardSize > MAX_BOARD_SIZE) {
                ColorPrint.printYellow(
                    "Limiting board size to valid range " + MIN_BOARD_SIZE + ".." + MAX_BOARD_SIZE
                );
            }
            return Utils.clamp(boardSize, MIN_BOARD_SIZE, MAX_BOARD_SIZE);
        } catch (NumberFormatException _) {}
        ColorPrint.printWarn("Invalid size, defaulting to " + DEFAULT_BOARD_SIZE + "...");
        return DEFAULT_BOARD_SIZE;
    }
}
