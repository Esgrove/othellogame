package othello;

import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;
import java.util.stream.Collectors;
import java.util.stream.IntStream;

import othello.Models.Disk;

/** Gameplay loop and main logic. */
public class Othello {
    private final Settings settings;
    private Board board;
    private final Player playerBlack;
    private final Player playerWhite;
    private int roundsPlayed = 0;
    private int gamesPlayed = 0;
    private final List<String> gameLog = new ArrayList<>();

    public Othello(Settings settings) {
        this.settings = settings;
        this.board = new Board(settings.boardSize());
        PlayerSettings playerSettings = settings.toPlayerSettings();
        this.playerBlack = Player.black(playerSettings);
        this.playerWhite = Player.white(playerSettings);
    }

    /** Play one full game of Othello. */
    public void play() {
        do {
            initGame();
            gameLoop();
            printResult();
            if (settings.showLog()) {
                printLog();
            }
        } while (!settings.autoplayMode() && getAnswer("Would you like to play again"));
    }

    /** Initialize game board and players for a new game. */
    private void initGame() {
        if (gamesPlayed > 0) {
            board = new Board(settings.boardSize());
            playerBlack.reset();
            playerWhite.reset();
            roundsPlayed = 0;
            gameLog.clear();
        }
        if (settings.autoplayMode()) {
            playerBlack.setHuman(false);
            playerWhite.setHuman(false);
        } else if (settings.useDefaults()) {
            playerWhite.setHuman(false);
        } else if (getAnswer("Would you like to play against the computer")) {
            if (getAnswer("Would you like to play as black or white", "b", "w")) {
                playerWhite.setHuman(false);
            } else {
                playerBlack.setHuman(false);
            }
        }
        if (!settings.checkMode()) {
            ColorPrint.printBold("\nPlayers:");
            printStatus();
        }
    }

    /** Keep making moves until both players can't make a move anymore. */
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
                printDivider();
            }
        }
        gamesPlayed++;
        printGameEndFooter();
    }

    private void printRoundHeader() {
        if (!settings.checkMode()) {
            ColorPrint.printBold("\n=========== ROUND: " + roundsPlayed + " ===========");
        }
    }

    private void printDivider() {
        if (!settings.checkMode()) {
            System.out.println("--------------------------------");
        }
    }

    private void printGameEndFooter() {
        if (!settings.checkMode()) {
            ColorPrint.printBold("\n================================");
            ColorPrint.printColor("The game is finished!\n", AnsiColor.GREEN);
        }
    }

    /** Print game log with all moves and board states. */
    private void printLog() {
        String formattedLog = formatGameLog();
        if (!settings.checkMode()) {
            ColorPrint.printBold("Game log:", AnsiColor.YELLOW);
            System.out.println(formattedLog);
        }
        String hash = Utils.calculateSha256(formattedLog);
        System.out.println(hash);
    }

    private String formatGameLog() {
        return IntStream.range(0, gameLog.size())
            .mapToObj(i -> String.format("%02d: %s", i + 1, gameLog.get(i)))
            .collect(Collectors.joining("\n"));
    }

    /**
     * Print final result and winner.
     */
    private void printResult() {
        ColorPrint.printBold("Result:");
        printStatus();
        System.out.println();

        Disk winner = board.result();
        if (winner == Disk.EMPTY) {
            ColorPrint.printBold("The game ended in a tie...\n");
        } else {
            ColorPrint.printBold("The winner is " + winner.name() + "!\n");
        }
    }

    /**
     * Print current board and player status.
     */
    private void printStatus() {
        System.out.println(playerBlack);
        System.out.println(playerWhite);
        System.out.println();
        System.out.println(board);
    }

    /** Prompt the user and return their answer as boolean. */
    private boolean getAnswer(String question) {
        return getAnswer(question, "y", "n");
    }

    private boolean getAnswer(String question, String yes, String no) {
        System.out.print(question + " (" + yes + "/" + no + ")? ");
        Scanner scanner = new Scanner(System.in);
        String ans = scanner.nextLine();
        return ans != null && !ans.isBlank() && ans.equalsIgnoreCase(yes);
    }
}
