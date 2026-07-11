//==========================================================
// Models
// Basic types and methods
// Akseli Lukkarila
// 2019-2026
//==========================================================

import chalk, { type ChalkInstance } from 'chalk';

/** Represents one game piece or lack of one. */
export enum Disk {
    Black = -1,
    Empty = 0,
    White = 1,
}

/** Returns a single character identifier string for the given disk. */
export function boardChar(disk: Disk): string {
    switch (disk) {
        case Disk.Black:
            return 'B';
        case Disk.Empty:
            return '_';
        case Disk.White:
            return 'W';
    }
}

/** Returns a single character identifier string for the given disk. */
export function boardCharWithColor(disk: Disk): string {
    return diskColor(disk)(boardChar(disk));
}

/** Return the associated colour for this disk. */
export function diskColor(disk: Disk): ChalkInstance {
    switch (disk) {
        case Disk.Black:
            return chalk.magenta;
        case Disk.Empty:
            return chalk.white;
        case Disk.White:
            return chalk.cyan;
    }
}

/** Returns the disk formatted as a coloured string. */
export function diskString(disk: Disk): string {
    switch (disk) {
        case Disk.Black:
            return diskColor(disk)('BLACK');
        case Disk.Empty:
            return diskColor(disk)('EMPTY');
        case Disk.White:
            return diskColor(disk)('WHITE');
    }
}

/** Return the opposing disk colour for this disk. */
export function opponent(disk: Disk): Disk {
    switch (disk) {
        case Disk.Black:
            return Disk.White;
        case Disk.Empty:
            return Disk.Empty;
        case Disk.White:
            return Disk.Black;
    }
}

/** Represents one step direction on the board. */
export class Step {
    constructor(
        readonly x: number,
        readonly y: number,
    ) {}
}

/** Represents one square location on the board. */
export class Square {
    constructor(
        readonly x: number,
        readonly y: number,
    ) {}

    /** Get a new square by adding a step to this square. */
    add(step: Step): Square {
        return new Square(this.x + step.x, this.y + step.y);
    }

    equals(other: Square): boolean {
        return this.x === other.x && this.y === other.y;
    }

    /** Compare squares in ascending order by x, then y. */
    static compare(this: void, a: Square, b: Square): number {
        return a.x - b.x || a.y - b.y;
    }

    toString(): string {
        return `(${this.x},${this.y})`;
    }
}

/**
 * Represents a continuous line of squares in one direction.
 *
 * The `step` field determines the direction on the board,
 * and `count` describes how many consecutive squares in that direction there are.
 */
export class Direction {
    constructor(
        readonly step: Step,
        readonly count: number,
    ) {}
}

/** Represents one possible disk placement for the given disk colour. */
export class Move {
    constructor(
        readonly square: Square,
        readonly disk: Disk,
        readonly value: number,
        readonly directions: Direction[],
    ) {}

    /** Format move for log entry. */
    logEntry(): string {
        return `${boardChar(this.disk)}:${this.square},${this.value}`;
    }

    /** Get all the squares playing this move will change. */
    affectedSquares(): Square[] {
        const paths: Square[] = [];
        for (const { step, count } of this.directions) {
            let pos = this.square.add(step);
            for (let i = 0; i < count; i++) {
                paths.push(pos);
                pos = pos.add(step);
            }
        }
        paths.sort(Square.compare);
        return paths;
    }

    /** Compare moves in descending order by value, then ascending order by square. */
    static compare(this: void, a: Move, b: Move): number {
        return b.value - a.value || Square.compare(a.square, b.square);
    }

    toString(): string {
        return `Square: ${this.square} -> value: ${this.value}`;
    }
}
