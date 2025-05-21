package othello;

import java.util.List;
import java.util.Random;
import java.util.Scanner;

import othello.Models.Disk;
import othello.Models.Move;
import othello.Models.Square;

/**
 * Defines one player that can be either human or computer controlled.
 */
public class Player {
    private final Disk disk;
    private final PlayerSettings settings;
    private boolean canPlay = true;
    private boolean isHuman = true;
    private int roundsPlayed = 0;
    private static final Random random = new Random();

    private static final Scanner scanner = new Scanner(System.in);

    public Player(Disk disk, PlayerSettings settings) {
        this.disk = disk;
        this.settings = settings;
    }

    public static Player black(PlayerSettings settings) {
        return new Player(Disk.BLACK, settings);
    }

    public static Player white(PlayerSettings settings) {
        return new Player(Disk.WHITE, settings);
    }

    /**
     * Play one round as this player.
     *
     * @param board the game board
     * @return log entry string or null if no move was made
     */
    public String playOneMove(Board board) {
        if (!settings.checkMode()) {
            System.out.println("Turn: " + disk.name());
        }

        List<Move> moves = board.possibleMoves(disk);
        if (!moves.isEmpty()) {
            canPlay = true;

            if (isHuman && settings.showHelpers() && !settings.checkMode()) {
                board.printPossibleMoves(moves);
            }

            Move chosenMove = isHuman ? getHumanMove(moves) : getComputerMove(moves);

            board.placeDisk(chosenMove);

            if (!settings.checkMode()) {
                board.printScore();
            }

            roundsPlayed++;
            if (!settings.testMode()) {
                try {
                    Thread.sleep(1000);
                } catch (InterruptedException ignored) {}
            }

            return chosenMove.logEntry();
        }

        canPlay = false;

        if (!settings.checkMode()) {
            ColorPrint.printColor("  No moves available...", AnsiColor.YELLOW);
        }

        return null;
    }

    /**
     * Reset player status for a new game.
     */
    public void reset() {
        roundsPlayed = 0;
        canPlay = true;
    }

    /**
     * Set the player as human or computer controlled.
     *
     * @param isHuman true if human player, false if computer
     */
    public void setHuman(boolean isHuman) {
        this.isHuman = isHuman;
    }

    /**
     * Return move chosen by computer.
     */
    private Move getComputerMove(List<Move> moves) {
        if (!settings.checkMode()) {
            System.out.println("  Computer plays...");
        }

        Move chosenMove;
        if (settings.testMode()) {
            chosenMove = moves.get(0);
        } else {
            try {
                Thread.sleep(1000 + random.nextInt(1000));
            } catch (InterruptedException ignored) {}
            chosenMove = moves.get(random.nextInt(moves.size()));
        }

        if (!settings.checkMode()) {
            System.out.println("  " + chosenMove.square() + " -> " + chosenMove.value());
        }

        return chosenMove;
    }

    /**
     * Return move chosen by a human player.
     */
    private Move getHumanMove(List<Move> moves) {
        while (true) {
            Square square = getSquare();
            for (Move move : moves) {
                if (move.square().equals(square)) {
                    return move;
                }
            }
            ColorPrint.printError("  Can't place a " + disk.name() + " disk in square " + square);
        }
    }

    /**
     * Ask human player for square coordinates.
     */
    private Square getSquare() {
        while (true) {
            try {
                System.out.print("  Give disk position (x,y): ");
                String input = scanner.nextLine().trim();

                if (input.length() != 3 || input.charAt(1) != ',') {
                    throw new IllegalArgumentException();
                }

                int x = Integer.parseInt(input.substring(0, 1));
                int y = Integer.parseInt(input.substring(2, 3));

                return new Square(x, y);
            } catch (IllegalArgumentException e) {
                ColorPrint.printError("  Give coordinates in the form 'x,y'");
            }
        }
    }

    /**
     * Return player type description string.
     */
    private String typeString() {
        return isHuman ? "Human   " : "Computer";
    }

    public boolean canPlay() {
        return canPlay;
    }

    public Disk disk() {
        return disk;
    }

    @Override
    public String toString() {
        return disk.name() + " | " + typeString() + " | Moves: " + roundsPlayed;
    }
}
