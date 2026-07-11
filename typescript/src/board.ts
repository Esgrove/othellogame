//==========================================================
// Board
// Defines the game board
// Akseli Lukkarila
// 2019-2026
//==========================================================

import chalk from 'chalk';

import { printYellow } from './colorprint.ts';
import { boardChar, boardCharWithColor, Direction, Disk, Move, opponent, Square, Step } from './models.ts';

const UP = 1;
const DOWN = -1;
const LEFT = -1;
const RIGHT = 1;
const STILL = 0;

/** All possible step directions for a square on the board. */
const STEP_DIRECTIONS: readonly Step[] = [
    new Step(DOWN, LEFT),
    new Step(DOWN, RIGHT),
    new Step(DOWN, STILL),
    new Step(STILL, LEFT),
    new Step(STILL, RIGHT),
    new Step(UP, LEFT),
    new Step(UP, RIGHT),
    new Step(UP, STILL),
];

/** Handles game board state and logic. */
export class Board {
    private board: Disk[];
    // Keep track of empty squares on board to avoid checking already filled positions.
    // Stores board indices since JS Set compares objects by reference.
    private emptySquares: Set<number>;
    // Index list (0...size) to avoid repeating same range in loops.
    private readonly indices: number[];
    readonly size: number;

    /** Initialize a new board for the given board size. */
    constructor(size: number) {
        this.board = Board.initBoard(size);
        this.emptySquares = Board.initEmptySquares(this.board);
        this.indices = Array.from({ length: size }, (_, i) => i);
        this.size = size;
    }

    /** Return true if board contains empty squares. */
    canPlay(): boolean {
        return this.emptySquares.size > 0;
    }

    /** Update board for given disk placement. */
    placeDisk(chosenMove: Move): void {
        const start = chosenMove.square;
        const square = this.getSquare(start);
        if (square === undefined) {
            throw new Error(`Invalid coordinates: ${start}`);
        }
        if (square !== Disk.Empty) {
            throw new Error(`Trying to place disk to an occupied square: ${start}!`);
        }
        this.setSquare(start, chosenMove.disk);
        this.emptySquares.delete(this.squareIndex(start));
        for (const affectedSquare of chosenMove.affectedSquares()) {
            this.setSquare(affectedSquare, chosenMove.disk);
        }
    }

    /** Returns a list of possible moves for the given player. */
    possibleMoves(disk: Disk): Move[] {
        const moves: Move[] = [];
        const opposingDisk = opponent(disk);
        for (const index of this.emptySquares) {
            const square = new Square(index % this.size, Math.floor(index / this.size));
            let value = 0;
            const directions: Direction[] = [];
            for (const step of STEP_DIRECTIONS) {
                let pos = square.add(step);
                // Next square in this direction needs to be the opposing disk
                if ((this.getSquare(pos) ?? Disk.Empty) !== opposingDisk) {
                    continue;
                }
                let numSteps = 0;
                // Keep stepping over opponents disks
                while ((this.getSquare(pos) ?? Disk.Empty) === opposingDisk) {
                    numSteps += 1;
                    pos = pos.add(step);
                }
                // Valid move only if a line of opposing disks ends with own disk
                if ((this.getSquare(pos) ?? Disk.Empty) === disk) {
                    directions.push(new Direction(step, numSteps));
                    value += numSteps;
                }
            }
            if (value > 0) {
                moves.push(new Move(square, disk, value, directions));
            }
        }
        moves.sort(Move.compare);
        return moves;
    }

    /** Print board with available move coordinates and the resulting points gained. */
    printPossibleMoves(moves: Move[]): void {
        printYellow(`  Possible moves (${moves.length}):`);
        // Convert board from Disk enums to strings
        const formattedBoard = this.board.map(boardCharWithColor);
        // Add possible moves to board
        for (const possibleMove of moves) {
            const index = this.squareIndex(possibleMove.square);
            formattedBoard[index] = chalk.yellow(possibleMove.value.toString());
            console.log(`  ${possibleMove}`);
        }
        // Print board with move positions
        let text = '    ';
        for (const i of this.indices) {
            text += ` ${chalk.bold(i.toString())}`;
        }
        for (const y of this.indices) {
            text += `\n  ${chalk.bold(y.toString())}`;
            for (const x of this.indices) {
                text += ` ${formattedBoard[y * this.size + x]}`;
            }
        }
        console.log(text);
    }

    /** Print current score for both players. */
    printScore(): void {
        const [black, white] = this.playerScores();
        console.log(`\n${this}`);
        console.log(`Score: ${chalk.magenta(black.toString())} | ${chalk.cyan(white.toString())}`);
    }

    /** Returns the winning disk colour. Empty indicates a draw. */
    result(): Disk {
        const totalScore = this.score();
        if (totalScore === 0) {
            return Disk.Empty;
        }
        return totalScore > 0 ? Disk.White : Disk.Black;
    }

    /** Get board status string for game log. */
    logEntry(): string {
        return this.board.map(boardChar).join('');
    }

    /** Check that the given coordinates are valid (inside the board). */
    private checkCoordinates(x: number, y: number): boolean {
        return x >= 0 && x < this.size && y >= 0 && y < this.size;
    }

    /** Check that the given square is valid (inside the board). */
    private checkSquare(square: Square): boolean {
        return this.checkCoordinates(square.x, square.y);
    }

    /** Returns the state of the board (empty, white, black) at the given square. */
    private getSquare(square: Square): Disk | undefined {
        return this.checkSquare(square) ? this.board[this.squareIndex(square)] : undefined;
    }

    /** Map square to board index. */
    private squareIndex(square: Square): number {
        return square.y * this.size + square.x;
    }

    /** Count and return the number of black and white disks. */
    private playerScores(): [number, number] {
        let black = 0;
        let white = 0;
        for (const disk of this.board) {
            switch (disk) {
                case Disk.Black:
                    black += 1;
                    break;
                case Disk.White:
                    white += 1;
                    break;
                default:
                    break;
            }
        }
        return [black, white];
    }

    /**
     * Returns the total score.
     * Positive value means more white disks and negative means more black disks.
     */
    private score(): number {
        return this.board.reduce((sum, disk) => sum + disk, 0);
    }

    /** Sets the given square to the given value. */
    private setSquare(square: Square, disk: Disk): void {
        if (!this.checkSquare(square)) {
            throw new Error(`Invalid coordinates: ${square}`);
        }
        this.board[this.squareIndex(square)] = disk;
    }

    /** Initialize game board with starting disk positions. */
    private static initBoard(size: number): Disk[] {
        // Initialize game board with empty disks
        const board: Disk[] = new Array<Disk>(size * size).fill(Disk.Empty);
        // Set starting positions
        const half = Math.floor((size - 1) / 2);
        const row = size % 2 === 0 ? half : half - 1;
        const col = Math.floor(size / 2);
        board[row * size + row] = Disk.White;
        board[row * size + col] = Disk.Black;
        board[col * size + row] = Disk.Black;
        board[col * size + col] = Disk.White;
        return board;
    }

    /** Initialize empty squares for the board. */
    private static initEmptySquares(board: Disk[]): Set<number> {
        const emptySquares = new Set<number>();
        for (let index = 0; index < board.length; index++) {
            if (board[index] === Disk.Empty) {
                emptySquares.add(index);
            }
        }
        return emptySquares;
    }

    /** Format game board to string. */
    toString(): string {
        // Horizontal indices
        let text = `  ${chalk.bold(this.indices.join(' '))}`;
        for (const y of this.indices) {
            // Vertical index
            text += `\n${chalk.bold(y.toString())}`;
            // Row values
            for (const x of this.indices) {
                text += ` ${boardCharWithColor(this.board[y * this.size + x])}`;
            }
        }
        return text;
    }
}
