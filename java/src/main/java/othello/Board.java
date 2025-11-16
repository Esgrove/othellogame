package othello;

import java.util.*;
import java.util.stream.IntStream;

import othello.Models.Direction;
import othello.Models.Disk;
import othello.Models.Move;
import othello.Models.Square;
import othello.Models.Step;
import othello.Utils.IntPair;

import static othello.ColorPrint.getColor;
import static othello.ColorPrint.printColor;

/**
 * Handles game board state and logic.
 */
public class Board {
    private final int size;
    private final List<Disk> board;
    private final Set<Square> emptySquares;
    private final List<Integer> indices;

    // Store all possible step directions on board
    public static final Step[] stepDirections = new Step[] {
        new Step(-1, -1),
        new Step(-1, 0),
        new Step(-1, 1),
        new Step(0, -1),
        new Step(0, 1),
        new Step(1, -1),
        new Step(1, 0),
        new Step(1, 1)
    };

    public Board(int size) {
        this.size = size;
        int numSquares = size * size;
        this.board = new ArrayList<>(Collections.nCopies(numSquares, Disk.EMPTY));

        // Set starting positions
        int row = (size % 2 == 0) ? (size - 1) / 2 : (size - 1) / 2 - 1;
        int col = size / 2;
        this.board.set(row * size + row, Disk.WHITE);
        this.board.set(row * size + col, Disk.BLACK);
        this.board.set(col * size + row, Disk.BLACK);
        this.board.set(col * size + col, Disk.WHITE);

        // Index list (0...size) to avoid repeating the same range in loops
        this.indices = IntStream.range(0, size)
            .boxed()
            .toList();

        // Keep track of empty squares on board to avoid checking already filled positions
        this.emptySquares = new HashSet<>();
        for (int y : indices) {
            for (int x : indices) {
                if (board.get(y * size + x) == Disk.EMPTY) {
                    emptySquares.add(new Square(x, y));
                }
            }
        }
    }

    /** Return true if board contains empty squares. */
    public boolean canPlay() {
        return !emptySquares.isEmpty();
    }

    /** Update board for given disk placement. */
    public void placeDisk(Move move) {
        Square start = move.square();
        if (getSquare(start) != Disk.EMPTY) {
            throw new RuntimeException("Trying to place disk to an occupied square " + start + "!");
        }
        setSquare(start, move.disk());
        emptySquares.remove(start);
        for (Square square : move.affectedSquares()) {
            setSquare(square, move.disk());
        }
    }

    /** Returns a list of possible moves for the given player. */
    public List<Move> possibleMoves(Disk color) {
        List<Move> moves = new ArrayList<>();
        Disk other = color.opponent();
        for (Square square : emptySquares) {
            int value = 0;
            List<Direction> directions = new ArrayList<>();
            for (Step step : stepDirections) {
                Square pos = square.add(step);
                // Next square in this direction needs to be the opposing disk
                if (getSquare(pos) != other) {
                    continue;
                }
                int numSteps = 0;
                // Keep stepping over opponents disks
                while (getSquare(pos) == other) {
                    numSteps++;
                    pos = pos.add(step);
                }
                // Valid move only if a line of opposing disks ends in own disk
                if (getSquare(pos) != color) {
                    continue;
                }
                value += numSteps;
                directions.add(new Direction(step, numSteps));
            }
            if (value > 0) {
                moves.add(new Move(square, value, color, directions));
            }
        }
        if (!moves.isEmpty()) {
            moves.sort(null);
        }
        return moves;
    }

    /** Print board with available move coordinates and the resulting points gained. */
    public void printPossibleMoves(Collection<Move> moves) {
        printColor("  Possible moves (" + moves.size() + "):", AnsiColor.YELLOW);
        // Convert board from Disk enums to strings
        List<String> formattedBoard = new ArrayList<>();
        for (Disk disk : board) {
            formattedBoard.add(disk.boardChar(true));
        }
        // Add possible moves to board
        for (Move move : moves) {
            System.out.println("  " + move);
            int x = move.square().x();
            int y = move.square().y();
            formattedBoard.set(y * size + x, getColor(Integer.toString(move.value()), AnsiColor.YELLOW));
        }
        // print board with move positions
        System.out.print("   ");
        for (int col : indices) {
            System.out.print(" " + col);
        }
        for (int row : indices) {
            System.out.print("\n  " + row);
            for (int col : indices) {
                System.out.print(" " + formattedBoard.get(row * size + col));
            }
        }
        System.out.println();
    }

    /** Print current score for both players. */
    public void printScore() {
        IntPair scores = playerScores();
        int black = scores.first();
        int white = scores.second();
        System.out.println("\n" + this);
        System.out.println(
            "Score: " + getColor(Integer.toString(black), Disk.BLACK.diskColor())
                + " | " + getColor(Integer.toString(white), Disk.WHITE.diskColor())
        );
    }

    /** Calculates the final score and returns the winning player. */
    public Disk result() {
        int sum = score();
        if (sum < 0)
            return Disk.BLACK;
        if (sum > 0)
            return Disk.WHITE;
        return Disk.EMPTY;
    }

    /** Get board status string for game log. */
    public String logEntry() {
        StringBuilder builder = new StringBuilder();
        for (Disk disk : board) {
            builder.append(disk.boardChar(false));
        }
        return builder.toString();
    }

    /** Check that the given coordinates are valid (inside the board). */
    private boolean checkCoordinates(int x, int y) {
        return x >= 0 && x < size && y >= 0 && y < size;
    }

    /** Returns the state of the board (empty, white, black) at the given coordinates. */
    private Disk getSquare(Square square) {
        int x = square.x();
        int y = square.y();
        if (!checkCoordinates(x, y)) {
            return null;
        }
        return board.get(y * size + x);
    }

    /** Count and return the number of black and white disks. */
    private IntPair playerScores() {
        int black = 0;
        int white = 0;
        for (Disk disk : board) {
            switch (disk) {
            case WHITE -> white++;
            case BLACK -> black++;
            default -> {}
            }
        }
        return new IntPair(black, white);
    }

    /**
     * Returns the total score. Positive value means more white disks and negative means more black disks.
     */
    private int score() {
        return board.stream().mapToInt(Disk::value).sum();
    }

    /** Sets the given square to given value. */
    private void setSquare(Square square, Disk disk) {
        int x = square.x();
        int y = square.y();
        if (!checkCoordinates(x, y)) {
            throw new RuntimeException("Invalid coordinates (" + x + "," + y + ")!");
        }
        board.set(y * size + x, disk);
    }

    @Override
    public String toString() {
        StringBuilder builder = new StringBuilder(" ");
        for (int col : indices) {
            builder.append(" ").append(col);
        }
        for (int row : indices) {
            builder.append("\n").append(row);
            for (int col : indices) {
                builder.append(" ").append(board.get(row * size + col).boardChar(true));
            }
        }
        return builder.toString();
    }
}
