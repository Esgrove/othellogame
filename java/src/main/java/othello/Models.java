package othello;

import java.util.Comparator;
import java.util.List;

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

        public int value() {
            return value;
        }

        public AnsiColor diskColor() {
            return switch (this) {
            case EMPTY -> AnsiColor.WHITE;
            case WHITE -> AnsiColor.CYAN;
            case BLACK -> AnsiColor.MAGENTA;
            };
        }

        public Disk opponent() {
            return switch (this) {
            case EMPTY -> EMPTY;
            case WHITE -> BLACK;
            case BLACK -> WHITE;
            };
        }

        /**
         * Returns string character representing board status (black, white, empty).
         */
        public String boardChar(boolean color) {
            return switch (this) {
            case EMPTY -> "_";
            case WHITE -> color ? ColorPrint.getColor("W", this.diskColor()) : "W";
            case BLACK -> color ? ColorPrint.getColor("B", this.diskColor()) : "B";
            };
        }
    }

    /**
     * Represents a step direction on the board.
     */
    public record Step(int x, int y) {
        @Override
        public String toString() {
            return "[" + x + "," + y + "]";
        }
    }

    /**
     * Represents one square location on the board.
     */
    public record Square(int x, int y) implements Comparable<Square> {
        public Square add(Step step) {
            return new Square(x + step.x(), y + step.y());
        }

        public Square add(Square square) {
            return new Square(x + square.x(), y + square.y());
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
     * Represents one possible disk placement for given disk colour.
     */
    public record Move(Square square, int value, Disk disk, List<Direction> directions)
        implements
        Comparable<Move> {
        @Override
        public int compareTo(Move other) {
            return Comparator
                .comparingInt(Move::value).reversed()
                .thenComparing(Move::square)
                .compare(this, other);
        }

        public String logEntry() {
            return disk.boardChar(false) + ":" + square + "," + value;
        }

        @Override
        public String toString() {
            return "Square: " + square + " -> value: " + value;
        }
    }

    /**
     * Represents a continuous line of squares in one direction.
     *
     * <p>The {@code step} component determines the direction on the board,
     * and {@code count} describes how many consecutive squares in that direction there are.</p>
     */
    public record Direction(Step step, int count) {}
}
