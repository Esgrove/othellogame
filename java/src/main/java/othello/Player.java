package othello;

import java.util.List;
import java.util.Random;
import java.util.Scanner;

import othello.Models.Disk;
import othello.Models.Move;
import othello.Models.PlayerType;
import othello.Models.Square;

/**
 * Defines one player that can be either human or computer controlled.
 */
public class Player {
    private final Disk disk;
    private final PlayerSettings settings;
    private boolean canPlay = true;
    private PlayerType playerType = PlayerType.HUMAN;
    private int roundsPlayed = 0;

    private static final Random random = new Random();
    private static final Scanner scanner = new Scanner(System.in);

    public Player(Disk disk, PlayerSettings settings) {
        this.disk = disk;
        this.settings = settings;
    }

    /** Shorthand to initialize a new player for black disks. */
    public static Player black(PlayerSettings settings) {
        return new Player(Disk.BLACK, settings);
    }

    /** Shorthand to initialize a new player for white disks. */
    public static Player white(PlayerSettings settings) {
        return new Player(Disk.WHITE, settings);
    }

    /** Return true if player can make a move. */
    public boolean canPlay() {
        return canPlay;
    }

    /**
     * Play one round as this player.
     */
    public String playOneMove(Board board) {
        if (!settings.checkMode()) {
            System.out.println("Turn: " + disk.name());
        }
        List<Move> moves = board.possibleMoves(disk);
        if (!moves.isEmpty()) {
            canPlay = true;
            if (human() && settings.showHelpers() && !settings.checkMode()) {
                board.printPossibleMoves(moves);
            }
            Move chosenMove = human() ? getHumanMove(moves) : getComputerMove(moves);
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
     * Returns true if the player is human.
     */
    public boolean human() {
        return playerType.human();
    }

    /**
     * Returns true if the player is controlled by computer.
     */
    public boolean computer() {
        return playerType.computer();
    }

    /**
     * Set the player as human controlled.
     */
    public void setHuman() {
        this.playerType = PlayerType.HUMAN;
    }

    /**
     * Set the player as computer controlled.
     */
    public void setComputer() {
        this.playerType = PlayerType.COMPUTER;
    }

    /**
     * Set the player type.
     */
    public void setPlayerType(PlayerType playerType) {
        this.playerType = playerType;
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
            chosenMove = moves.getFirst();
        } else {
            // Wait a bit and pick a random move
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
                // Check that the chosen square is actually one of the possible moves
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
        return playerType.toString();
    }

    @Override
    public String toString() {
        return disk.name() + " | " + typeString() + " | Moves: " + roundsPlayed;
    }
}
