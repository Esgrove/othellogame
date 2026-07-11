package othello;

import java.util.ArrayList;
import java.util.Collection;
import java.util.Collections;
import java.util.HashSet;
import java.util.List;
import java.util.Set;
import java.util.stream.Collectors;
import java.util.stream.IntStream;

import othello.Models.Direction;
import othello.Models.Disk;
import othello.Models.Move;
import othello.Models.Square;
import othello.Models.Step;
import othello.Utils.IntPair;

import static othello.ColorPrint.getBold;
import static othello.ColorPrint.getColor;
import static othello.ColorPrint.printYellow;

/**
 * Handles game board state and logic.
 */
public class Board {
    private static final int UP = 1;
    private static final int DOWN = -1;
    private static final int LEFT = -1;
    private static final int RIGHT = 1;
    private static final int STILL = 0;

    /** All possible step directions for a square on the board. */
    private static final Step[] STEP_DIRECTIONS = new Step[] {
        new Step(DOWN, LEFT),
        new Step(DOWN, RIGHT),
        new Step(DOWN, STILL),
        new Step(STILL, LEFT),
        new Step(STILL, RIGHT),
        new Step(UP, LEFT),
        new Step(UP, RIGHT),
        new Step(UP, STILL)
    };

    private final List<Disk> board;
    private final Set<Square> emptySquares;
    private final List<Integer> indices;
    final int size;

    /**
     * Initialize a new board for the given board size.
     */
    public Board(int size) {
        this.board = initBoard(size);

        // Index list (0...size) to avoid repeating same range in loops.
        this.indices = IntStream.range(0, size)
            .boxed()
            .toList();

        // Keep track of empty squares on board to avoid checking already filled positions.
        this.emptySquares = initEmptySquares(size, board);

        this.size = size;
    }

    /**
     * Return true if board contains empty squares.
     */
    public boolean canPlay() {
        return !emptySquares.isEmpty();
    }

    /**
     * Update board for given disk placement.
     */
    public void placeDisk(Move chosenMove) {
        Square start = chosenMove.square();
        Disk oldDisk = getSquare(start);
        if (oldDisk == null) {
            throw new IllegalArgumentException("Invalid coordinates: " + start);
        }
        if (oldDisk != Disk.EMPTY) {
            throw new IllegalStateException("Trying to place disk to an occupied square: " + start + "!");
        }
        setSquare(start, chosenMove.disk());
        emptySquares.remove(start);
        for (Square square : chosenMove.affectedSquares()) {
            setSquare(square, chosenMove.disk());
        }
    }

    /**
     * Returns a list of possible moves for the given player.
     */
    public List<Move> possibleMoves(Disk disk) {
        List<Move> moves = new ArrayList<>();
        Disk opposingDisk = disk.opponent();
        for (Square square : emptySquares) {
            int value = 0;
            List<Direction> directions = new ArrayList<>();
            for (Step step : STEP_DIRECTIONS) {
                Square pos = square.add(step);
                // Next square in this direction needs to be the opposing disk
                if (getSquare(pos) != opposingDisk) {
                    continue;
                }
                int numSteps = 0;
                // Keep stepping over opponents disks
                while (getSquare(pos) == opposingDisk) {
                    numSteps++;
                    pos = pos.add(step);
                }
                // Valid move only if a line of opposing disks ends with own disk
                if (getSquare(pos) == disk) {
                    directions.add(new Direction(step, numSteps));
                    value += numSteps;
                }
            }
            if (value > 0) {
                moves.add(new Move(square, disk, value, directions));
            }
        }
        moves.sort(null);
        return moves;
    }

    /**
     * Print board with available move coordinates and the resulting points gained.
     */
    public void printPossibleMoves(Collection<Move> moves) {
        printYellow("  Possible moves (" + moves.size() + "):");
        // Convert board from Disk enums to strings
        List<String> formattedBoard = new ArrayList<>(board.size());
        for (Disk disk : board) {
            formattedBoard.add(disk.boardCharWithColor());
        }
        // Add possible moves to board
        for (Move possibleMove : moves) {
            int index = squareIndex(possibleMove.square());
            formattedBoard.set(index, getColor(Integer.toString(possibleMove.value()), AnsiColor.YELLOW));
            System.out.println("  " + possibleMove);
        }
        // Print board with move positions
        System.out.print("    ");
        for (int i : indices) {
            System.out.print(" " + getBold(Integer.toString(i)));
        }
        for (int y : indices) {
            System.out.print("\n  " + getBold(Integer.toString(y)));
            for (int x : indices) {
                System.out.print(" " + formattedBoard.get(y * size + x));
            }
        }
        System.out.println();
    }

    /**
     * Print current score for both players.
     */
    public void printScore() {
        IntPair scores = playerScores();
        int black = scores.first();
        int white = scores.second();
        System.out.println("\n" + this);
        System.out.println(
            "Score: " + getColor(Integer.toString(black), AnsiColor.MAGENTA)
                + " | " + getColor(Integer.toString(white), AnsiColor.CYAN)
        );
    }

    /**
     * Returns the winning disk colour. Empty indicates a draw.
     */
    public Disk result() {
        int totalScore = score();
        if (totalScore > 0) {
            return Disk.WHITE;
        }
        if (totalScore < 0) {
            return Disk.BLACK;
        }
        return Disk.EMPTY;
    }

    /**
     * Get board status string for game log.
     */
    public String logEntry() {
        return board.stream().map(Disk::boardChar).collect(Collectors.joining());
    }

    /**
     * Check that the given coordinates are valid (inside the board).
     */
    private boolean checkCoordinates(int x, int y) {
        return x >= 0 && x < size && y >= 0 && y < size;
    }

    /**
     * Check that the given square is valid (inside the board).
     */
    private boolean checkSquare(Square square) {
        return checkCoordinates(square.x(), square.y());
    }

    /**
     * Returns the state of the board (empty, white, black) at the given square.
     */
    Disk getSquare(Square square) {
        if (checkSquare(square)) {
            return board.get(squareIndex(square));
        }
        return null;
    }

    /**
     * Map square to board index.
     */
    private int squareIndex(Square square) {
        return square.y() * size + square.x();
    }

    /**
     * Count and return the number of black and white disks.
     */
    IntPair playerScores() {
        int black = 0;
        int white = 0;
        for (Disk disk : board) {
            switch (disk) {
            case BLACK -> black++;
            case WHITE -> white++;
            default -> {}
            }
        }
        return new IntPair(black, white);
    }

    /**
     * Returns the total score. Positive value means more white disks and negative means more black disks.
     */
    int score() {
        return board.stream().mapToInt(Disk::value).sum();
    }

    /**
     * Sets the given square to the given value.
     */
    private void setSquare(Square square, Disk disk) {
        if (!checkSquare(square)) {
            throw new IllegalArgumentException("Invalid coordinates: " + square);
        }
        board.set(squareIndex(square), disk);
    }

    /**
     * Initialize game board with starting disk positions.
     */
    private static List<Disk> initBoard(int size) {
        // Initialize game board with empty disks
        List<Disk> board = new ArrayList<>(Collections.nCopies(size * size, Disk.EMPTY));
        // Set starting positions
        int row = (size % 2 == 0) ? (size - 1) / 2 : (size - 1) / 2 - 1;
        int col = size / 2;
        board.set(row * size + row, Disk.WHITE);
        board.set(row * size + col, Disk.BLACK);
        board.set(col * size + row, Disk.BLACK);
        board.set(col * size + col, Disk.WHITE);
        return board;
    }

    /**
     * Initialize empty squares for the board.
     */
    private static Set<Square> initEmptySquares(int size, List<Disk> board) {
        Set<Square> emptySquares = new HashSet<>();
        for (int i = 0; i < board.size(); i++) {
            if (board.get(i) == Disk.EMPTY) {
                emptySquares.add(new Square(i % size, i / size));
            }
        }
        return emptySquares;
    }

    @Override
    public String toString() {
        // Horizontal indices
        StringBuilder builder = new StringBuilder("  ");
        builder.append(getBold(indices.stream().map(String::valueOf).collect(Collectors.joining(" "))));
        for (int y : indices) {
            // Vertical index
            builder.append("\n").append(getBold(Integer.toString(y)));
            // Row values
            for (int x : indices) {
                builder.append(" ").append(board.get(y * size + x).boardCharWithColor());
            }
        }
        return builder.toString();
    }
}
