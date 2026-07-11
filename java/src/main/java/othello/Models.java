package othello;

import java.util.ArrayList;
import java.util.Comparator;
import java.util.List;
import java.util.Objects;

/**
 * Basic types and methods.
 */
public class Models {

    /**
     * Represents one game piece or lack of one.
     */
    public enum Disk {
        BLACK(-1),
        EMPTY(0),
        WHITE(1);

        private final int value;

        Disk(int value) {
            this.value = value;
        }

        /**
         * Returns the numeric value for this disk.
         */
        public int value() {
            return value;
        }

        /**
         * Returns a single character identifier string for the given disk.
         */
        public String boardChar() {
            return switch (this) {
            case BLACK -> "B";
            case EMPTY -> "_";
            case WHITE -> "W";
            };
        }

        /**
         * Returns a coloured single character identifier string for the given disk.
         */
        public String boardCharWithColor() {
            return ColorPrint.getColor(boardChar(), diskColor());
        }

        /**
         * Return the associated colour for this disk.
         */
        public AnsiColor diskColor() {
            return switch (this) {
            case BLACK -> AnsiColor.MAGENTA;
            case EMPTY -> AnsiColor.WHITE;
            case WHITE -> AnsiColor.CYAN;
            };
        }

        /**
         * Returns the disk formatted as a coloured string.
         */
        public String diskString() {
            return switch (this) {
            case BLACK -> ColorPrint.getColor("BLACK", diskColor());
            case EMPTY -> ColorPrint.getColor("EMPTY", diskColor());
            case WHITE -> ColorPrint.getColor("WHITE", diskColor());
            };
        }

        /**
         * Return the opposing disk colour for this disk.
         */
        public Disk opponent() {
            return switch (this) {
            case BLACK -> WHITE;
            case EMPTY -> EMPTY;
            case WHITE -> BLACK;
            };
        }
    }

    /**
     * Represents one step direction on the board.
     */
    public record Step(int x, int y) {
        /**
         * Returns a new step that is the sum of this step and the given step.
         */
        public Step add(Step step) {
            return new Step(x + step.x(), y + step.y());
        }

        @Override
        public String toString() {
            return "[" + x + "," + y + "]";
        }
    }

    /**
     * Represents one square location on the board.
     */
    public record Square(int x, int y) implements Comparable<Square> {
        /**
         * Returns a new square that is the sum of this square and the given step.
         */
        public Square add(Step step) {
            return new Square(x + step.x(), y + step.y());
        }

        /**
         * Returns a new square that is the sum of this square and the given square.
         */
        public Square add(Square square) {
            return new Square(x + square.x(), y + square.y());
        }

        /**
         * Get the index of this square on the board.
         */
        public int boardIndex(int boardSize) {
            return y * boardSize + x;
        }

        @Override
        public int compareTo(Square other) {
            return Comparator
                .comparingInt(Square::x)
                .thenComparingInt(Square::y)
                .compare(this, other);
        }

        @Override
        public String toString() {
            return "(" + x + "," + y + ")";
        }
    }

    /**
     * Represents a continuous line of squares in one direction.
     *
     * <p>
     * The {@code step} field determines the direction on the board, and {@code count} describes how many consecutive
     * squares in that direction there are.
     * </p>
     */
    public record Direction(Step step, int count) {}

    /**
     * Represents one possible disk placement for the given disk colour.
     */
    public record Move(Square square, Disk disk, int value, List<Direction> directions)
        implements
        Comparable<Move> {
        /**
         * Format move for log entry.
         */
        public String logEntry() {
            return disk.boardChar() + ":" + square + "," + value;
        }

        /**
         * Get all the squares playing this move will change.
         */
        public List<Square> affectedSquares() {
            List<Square> paths = new ArrayList<>();
            for (Direction direction : directions) {
                Square pos = square.add(direction.step());
                for (int i = 0; i < direction.count(); i++) {
                    paths.add(pos);
                    pos = pos.add(direction.step());
                }
            }
            paths.sort(Square::compareTo);
            return paths;
        }

        @Override
        public int compareTo(Move other) {
            return Comparator
                .comparingInt(Move::value).reversed()
                .thenComparing(Move::square)
                .compare(this, other);
        }

        // Custom equality that excludes `directions` to match the other implementations.
        @Override
        public boolean equals(Object other) {
            return other instanceof Move move
                && square.equals(move.square)
                && value == move.value
                && disk == move.disk;
        }

        @Override
        public int hashCode() {
            return Objects.hash(square, value, disk);
        }

        @Override
        public String toString() {
            return "Square: " + square + " -> value: " + value;
        }
    }
}
