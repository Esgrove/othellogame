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
    boolean canPlay = true;
    final Disk disk;
    PlayerType playerType = PlayerType.HUMAN;
    int roundsPlayed = 0;
    final PlayerSettings settings;

    private static final Random random = new Random();
    private static final Scanner scanner = new Scanner(System.in);

    /**
     * Initialize new player for the given disk color.
     */
    public Player(Disk disk, PlayerSettings settings) {
        this.disk = disk;
        this.settings = settings;
    }

    /**
     * Shorthand to initialize a new player for black disks.
     */
    public static Player black(PlayerSettings settings) {
        return new Player(Disk.BLACK, settings);
    }

    /**
     * Shorthand to initialize a new player for white disks.
     */
    public static Player white(PlayerSettings settings) {
        return new Player(Disk.WHITE, settings);
    }

    /**
     * Return true if player can make a move.
     */
    public boolean canPlay() {
        return canPlay;
    }

    /**
     * Play one round as this player.
     */
    public String playOneMove(Board board) {
        if (!settings.checkMode()) {
            System.out.println("Turn: " + disk.diskString());
        }
        List<Move> moves = board.possibleMoves(disk);
        if (moves.isEmpty()) {
            canPlay = false;
            if (!settings.checkMode()) {
                ColorPrint.printYellow("  No moves available...");
            }
            return null;
        }
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
            } catch (InterruptedException _) {}
        }
        return chosenMove.logEntry();
    }

    /**
     * Reset player status for a new game.
     */
    public void reset() {
        canPlay = true;
        roundsPlayed = 0;
    }

    /**
     * Returns true if player is controlled by a human player.
     */
    public boolean human() {
        return playerType.human();
    }

    /**
     * Returns true if player is controlled by computer.
     */
    public boolean computer() {
        return playerType.computer();
    }

    /**
     * Set the player as human or computer controlled.
     */
    public void setPlayerType(PlayerType playerType) {
        this.playerType = playerType;
    }

    /**
     * Set the player as human controlled.
     */
    public void setHuman() {
        setPlayerType(PlayerType.HUMAN);
    }

    /**
     * Set the player as computer controlled.
     */
    public void setComputer() {
        setPlayerType(PlayerType.COMPUTER);
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
            } catch (InterruptedException _) {}
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
            // Check that the chosen square is actually one of the possible moves
            for (Move move : moves) {
                if (move.square().equals(square)) {
                    return move;
                }
            }
            ColorPrint.printError("  Can't place a " + disk.diskString() + " disk in square " + square + "!");
        }
    }

    /**
     * Ask human player for square coordinates.
     */
    private static Square getSquare() {
        while (true) {
            System.out.print("  Give disk position (x,y): ");
            String input = scanner.nextLine();
            String[] values = input.trim().split(",");
            if (values.length == 2) {
                try {
                    int x = Integer.parseInt(values[0]);
                    int y = Integer.parseInt(values[1]);
                    if (x >= 0 && y >= 0) {
                        return new Square(x, y);
                    }
                } catch (NumberFormatException _) {}
            }
            ColorPrint.printError("  Give coordinates in the form 'x,y'!");
        }
    }

    /**
     * Return player type description string.
     */
    public String typeString() {
        return playerType.toString();
    }

    @Override
    public String toString() {
        return disk.diskString() + " | " + typeString() + " | Moves: " + roundsPlayed;
    }
}

/**
 * Player can be controlled either by a human or computer.
 */
enum PlayerType {
    HUMAN,
    COMPUTER;

    /**
     * Check if the player is controlled by a human.
     */
    public boolean human() {
        return this == HUMAN;
    }

    /**
     * Check if the player is controlled by a computer.
     */
    public boolean computer() {
        return this == COMPUTER;
    }

    @Override
    public String toString() {
        return this == HUMAN ? "Human   " : "Computer";
    }
}
