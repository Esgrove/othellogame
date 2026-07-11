//==========================================================
// Player
// Defines one player for Othello
// Akseli Lukkarila
// 2019-2026
//==========================================================

import chalk from 'chalk';

import type { Board } from './board.ts';
import { printError } from './colorprint.ts';
import { Disk, diskString, Move, Square } from './models.ts';
import { PlayerSettings } from './settings.ts';
import { readLine } from './utils.ts';

/** Player can be controlled either by a human or computer. */
export enum PlayerType {
    Human,
    Computer,
}

/** Defines one player that can be either human or computer controlled. */
export class Player {
    canPlay = true;
    private readonly disk: Disk;
    private playerType = PlayerType.Human;
    private roundsPlayed = 0;
    private readonly settings: PlayerSettings;

    /** Initialize new player for the given disk color. */
    constructor(disk: Disk, settings: PlayerSettings) {
        this.disk = disk;
        this.settings = settings;
    }

    /** Shorthand to initialize a new player for black disks. */
    static black(settings: PlayerSettings): Player {
        return new Player(Disk.Black, settings);
    }

    /** Shorthand to initialize a new player for white disks. */
    static white(settings: PlayerSettings): Player {
        return new Player(Disk.White, settings);
    }

    /** Play one round as this player. */
    async playOneMove(board: Board): Promise<string | null> {
        if (!this.settings.checkMode) {
            console.log(`Turn: ${diskString(this.disk)}`);
        }
        const moves = board.possibleMoves(this.disk);
        if (moves.length === 0) {
            this.canPlay = false;
            if (!this.settings.checkMode) {
                console.log(chalk.yellow('  No moves available...'));
            }
            return null;
        }
        this.canPlay = true;
        if (this.human() && this.settings.showHelpers && !this.settings.checkMode) {
            board.printPossibleMoves(moves);
        }
        const chosenMove = this.human() ? await this.getHumanMove(moves) : await this.getComputerMove(moves);
        board.placeDisk(chosenMove);
        if (!this.settings.checkMode) {
            board.printScore();
        }
        this.roundsPlayed += 1;
        if (!this.settings.testMode) {
            await Bun.sleep(1000);
        }
        return chosenMove.logEntry();
    }

    /** Reset player status for a new game. */
    reset(): void {
        this.canPlay = true;
        this.roundsPlayed = 0;
    }

    /** Returns true if player is controlled by a human player. */
    human(): boolean {
        return this.playerType === PlayerType.Human;
    }

    /** Returns true if player is controlled by computer. */
    computer(): boolean {
        return this.playerType === PlayerType.Computer;
    }

    /** Set the player as human or computer controlled. */
    setPlayerType(playerType: PlayerType): void {
        this.playerType = playerType;
    }

    /** Set the player as human controlled. */
    setHuman(): void {
        this.setPlayerType(PlayerType.Human);
    }

    /** Set the player as computer controlled. */
    setComputer(): void {
        this.setPlayerType(PlayerType.Computer);
    }

    /** Return move chosen by computer. */
    private async getComputerMove(moves: Move[]): Promise<Move> {
        if (!this.settings.checkMode) {
            console.log('  Computer plays...');
        }
        let chosenMove: Move;
        if (this.settings.testMode) {
            chosenMove = moves[0];
        } else {
            // Wait a bit and pick a random move
            await Bun.sleep(1000 + Math.random() * 1000);
            chosenMove = moves[Math.floor(Math.random() * moves.length)];
        }
        if (!this.settings.checkMode) {
            console.log(`  ${chosenMove.square} -> ${chosenMove.value}`);
        }
        return chosenMove;
    }

    /** Return move chosen by a human player. */
    private async getHumanMove(moves: Move[]): Promise<Move> {
        for (;;) {
            const square = await Player.getSquare();
            // Check that the chosen square is actually one of the possible moves
            const validMove = moves.find((move) => move.square.equals(square));
            if (validMove) {
                return validMove;
            }
            printError(`  Can't place a ${diskString(this.disk)} disk in square ${square}!`);
        }
    }

    /** Ask human player for square coordinates. */
    private static async getSquare(): Promise<Square> {
        for (;;) {
            process.stdout.write('  Give disk position (x,y): ');
            const input = await readLine();
            const values = input.trim().split(',');
            if (values.length === 2) {
                const x = parseInt(values[0]);
                const y = parseInt(values[1]);
                if (x >= 0 && y >= 0) {
                    return new Square(x, y);
                }
            }
            printError("  Give coordinates in the form 'x,y'!");
        }
    }

    /** Return player type description string. */
    typeString(): string {
        return this.playerType === PlayerType.Human ? 'Human   ' : 'Computer';
    }

    toString(): string {
        return `${diskString(this.disk)} | ${this.typeString()} | Moves: ${this.roundsPlayed}`;
    }
}
