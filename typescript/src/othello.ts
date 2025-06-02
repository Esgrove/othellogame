import { Command } from '@commander-js/extra-typings';
import packageJson from '../package.json' assert { type: 'json' };
import chalk from 'chalk';
import { clamp, Settings } from './utils.ts';
import { Board } from './board.ts';
import { Player } from './player.ts';

const MIN_BOARD_SIZE = 4;
const MAX_BOARD_SIZE = 10;
const DEFAULT_BOARD_SIZE = 8;

const program = new Command()
    .name('Othello Node')
    .description('A simple Othello CLI game implementation.')
    .argument('[SIZE]', `Optional board size (${MIN_BOARD_SIZE}..${MAX_BOARD_SIZE})`)
    .option('-a, --autoplay', 'Enable autoplay mode', false)
    .option('-d, --default', 'Play with default settings', false)
    .option('-l, --log', 'Show log after a game', false)
    .option('-n, --no-helpers', 'Hide disk placement hints', false)
    .option('-t, --test', 'Enable test mode', false)
    .helpOption('-h, --help', 'Print usage and exit')
    .version(`Othello Node ${packageJson.version}`, '-v, --version', 'Print version and exit');

program.parse(process.argv);

console.log(chalk.green('OTHELLO NODE.JS'));

const options = program.opts();
const sizeArgument = program.args[0];

class Othello {
    board: Board;
    settings: Settings;
    playerBlack: Player;
    playerWhite: Player;
    roundsPlayed: number;
    gamesPlayed: number;
    gameLog: string[];

    constructor(settings: Settings) {
        this.board = new Board(settings.boardSize);
        this.settings = settings;
        this.playerBlack = Player.black(settings.toPlayerSettings());
        this.playerWhite = Player.white(settings.toPlayerSettings());
        this.roundsPlayed = 0;
        this.gamesPlayed = 0;
        this.gameLog = [];
    }

    play() {
        console.info('Starting game...');
    }

    static async getBoardSize(): Promise<number> {
        const prompt = `Choose board size (default is ${DEFAULT_BOARD_SIZE}): `;
        process.stdout.write(prompt);

        for await (const line of console) {
            const input = line.trim();
            if (input === '') {
                console.info(`No input provided, defaulting to ${DEFAULT_BOARD_SIZE}...`);
                return DEFAULT_BOARD_SIZE;
            }

            const size = parseInt(input);
            if (!isNaN(size)) {
                if (size < MIN_BOARD_SIZE || size > MAX_BOARD_SIZE) {
                    console.info(`Limiting board size to valid range ${MIN_BOARD_SIZE}...${MAX_BOARD_SIZE}`);
                }
                return clamp(size, MIN_BOARD_SIZE, MAX_BOARD_SIZE);
            }

            console.warn(`Invalid size, defaulting to ${DEFAULT_BOARD_SIZE}...`);
            return DEFAULT_BOARD_SIZE;
        }

        return DEFAULT_BOARD_SIZE;
    }
}

try {
    let boardSize: number;
    if (sizeArgument !== undefined) {
        boardSize = parseInt(sizeArgument);
        if (isNaN(boardSize)) {
            console.error(chalk.red(`Invalid board size: ${sizeArgument}`));
            process.exit(1);
        }
        if (boardSize < MIN_BOARD_SIZE || boardSize > MAX_BOARD_SIZE) {
            console.error(chalk.red(`Unsupported board size: ${boardSize}`));
            process.exit(1);
        }
        console.log(chalk.blue(`Using board size: ${boardSize}`));
    } else if (options.autoplay || options.default) {
        boardSize = DEFAULT_BOARD_SIZE;
        console.log(chalk.blue(`Using default board size: ${boardSize}`));
    } else {
        boardSize = await Othello.getBoardSize();
    }

    const settings = new Settings(
        boardSize,
        options.autoplay,
        options.default,
        !options.helpers,
        options.log,
        options.test,
    );

    const game = new Othello(settings);
    game.play();
} catch (err) {
    if (err instanceof Error && err.name === 'SIGINT') {
        console.log(chalk.yellow('\ncancelled...'));
        process.exit(0);
    } else {
        throw err;
    }
}
