//==========================================================
// Othello
// Play Othello (Reversi) on the command line
// https://en.wikipedia.org/wiki/Reversi
// Akseli Lukkarila
// 2019-2026
//==========================================================

import chalk from 'chalk';

import { Board } from './board.ts';
import { printWarn } from './colorprint.ts';
import { Disk, diskString } from './models.ts';
import { Player } from './player.ts';
import { DEFAULT_BOARD_SIZE, MAX_BOARD_SIZE, MIN_BOARD_SIZE, Settings } from './settings.ts';
import { calculateSha256, clamp, readLine } from './utils.ts';

/** Gameplay loop and main logic. */
export class Othello {
    private board: Board;
    private readonly settings: Settings;
    private playerBlack: Player;
    private playerWhite: Player;
    private gameLog: string[] = [];
    private gamesPlayed = 0;
    private roundsPlayed = 0;

    /** Initialize Othello game. */
    constructor(settings: Settings) {
        this.board = new Board(settings.boardSize);
        this.settings = settings;
        this.playerBlack = Player.black(settings.toPlayerSettings());
        this.playerWhite = Player.white(settings.toPlayerSettings());
    }

    /** Play one full game of Othello. */
    async play(): Promise<void> {
        for (;;) {
            await this.initGame();
            await this.gameLoop();
            this.printResult();
            if (this.settings.showLog) {
                this.printLog();
            }
            if (this.settings.autoplayMode || !(await Othello.getAnswer('Would you like to play again', 'y', 'n'))) {
                break;
            }
        }
    }

    /** Initialize game board and players for a new game. */
    private async initGame(): Promise<void> {
        // Re-use existing objects instead of initializing new ones
        if (this.gamesPlayed > 0) {
            this.board = new Board(this.settings.boardSize);
            this.playerBlack.reset();
            this.playerWhite.reset();
            this.roundsPlayed = 0;
            this.gameLog = [];
        }
        if (this.settings.autoplayMode) {
            // Computer plays both
            this.playerBlack.setComputer();
            this.playerWhite.setComputer();
        } else if (this.settings.useDefaults) {
            // Default: play as black against white computer player
            this.playerWhite.setComputer();
        } else if (await Othello.getAnswer('Would you like to play against the computer', 'y', 'n')) {
            if (await Othello.getAnswer('Would you like to play as black or white', 'b', 'w')) {
                this.playerWhite.setComputer();
            } else {
                this.playerBlack.setComputer();
            }
        }
        if (!this.settings.checkMode) {
            console.log(chalk.bold('\nPlayers:'));
            this.printStatus();
        }
    }

    /** Keep making moves until both players can't make a move any more. */
    private async gameLoop(): Promise<void> {
        while (this.board.canPlay() && (this.playerBlack.canPlay || this.playerWhite.canPlay)) {
            this.roundsPlayed += 1;
            this.printRoundHeader();
            for (const player of [this.playerBlack, this.playerWhite]) {
                const result = await player.playOneMove(this.board);
                if (result !== null) {
                    this.gameLog.push(`${result};${this.board.logEntry()}`);
                }
                if (!this.settings.checkMode) {
                    console.log('--------------------------------');
                }
            }
        }
        this.gamesPlayed += 1;
        this.printGameEndFooter();
    }

    private formatGameLog(): string {
        return this.gameLog.map((line, index) => `${String(index + 1).padStart(2, '0')}: ${line}`).join('\n');
    }

    private printRoundHeader(): void {
        if (!this.settings.checkMode) {
            console.log(chalk.bold(`\n=========== ROUND: ${this.roundsPlayed} ===========`));
        }
    }

    private printGameEndFooter(): void {
        if (!this.settings.checkMode) {
            console.log(chalk.bold('\n================================'));
            console.log(chalk.green.bold('The game is finished!\n'));
        }
    }

    /** Print game log which shows all moves made and the game board state after each move. */
    private printLog(): void {
        const formattedLog = this.formatGameLog();
        if (!this.settings.checkMode) {
            console.log(chalk.yellow.bold('Game log:'));
            console.log(formattedLog);
        }
        const hexHash = calculateSha256(formattedLog);
        console.log(hexHash);
    }

    /** Print ending status and winner info. */
    private printResult(): void {
        console.log(chalk.bold('Result:'));
        this.printStatus();
        console.log('');

        const winner = this.board.result();
        if (winner === Disk.Empty) {
            console.log('The game ended in a tie...\n');
        } else {
            console.log(`The winner is ${diskString(winner)}!\n`);
        }
    }

    /** Print current board and player info. */
    private printStatus(): void {
        console.log(this.playerBlack.toString());
        console.log(`${this.playerWhite}\n`);
        console.log(this.board.toString());
    }

    /** Ask a question with two options, and return bool from user answer. */
    static async getAnswer(question: string, yes: string, no: string): Promise<boolean> {
        process.stdout.write(`${question} (${yes}/${no})? `);
        const input = await readLine();
        return input.trim().toLowerCase() === yes;
    }

    /** Ask and return the desired board size. */
    static async getBoardSize(): Promise<number> {
        process.stdout.write(`Choose board size (default is ${DEFAULT_BOARD_SIZE}): `);
        const input = await readLine();
        const boardSize = parseInt(input.trim());
        if (!isNaN(boardSize)) {
            if (boardSize < MIN_BOARD_SIZE || boardSize > MAX_BOARD_SIZE) {
                console.log(chalk.yellow(`Limiting board size to valid range ${MIN_BOARD_SIZE}..${MAX_BOARD_SIZE}`));
            }
            return clamp(boardSize, MIN_BOARD_SIZE, MAX_BOARD_SIZE);
        }
        printWarn(`Invalid size, defaulting to ${DEFAULT_BOARD_SIZE}...`);
        return DEFAULT_BOARD_SIZE;
    }
}
