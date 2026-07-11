//==========================================================
// Main
// Play Othello (Reversi) on the command line
// https://en.wikipedia.org/wiki/Reversi
// Akseli Lukkarila
// 2019-2026
//==========================================================

import { Command } from '@commander-js/extra-typings';
import chalk from 'chalk';

import { printError } from './colorprint.ts';
import { Othello } from './othello.ts';
import { DEFAULT_BOARD_SIZE, MAX_BOARD_SIZE, MIN_BOARD_SIZE, Settings } from './settings.ts';
import { versionInfo } from './version.ts';

const program = new Command()
    .name('othello_typescript')
    .description('A simple Othello CLI game implementation in TypeScript')
    .argument('[SIZE]', `Optional board size (${MIN_BOARD_SIZE}..${MAX_BOARD_SIZE})`)
    .option('-a, --autoplay', 'Enable autoplay mode with computer control')
    .option('-c, --check', 'Autoplay and only print result')
    .option('-d, --default', 'Play with default settings')
    .option('-l, --log', 'Show game log at the end')
    .option('-n, --no-helpers', 'Hide disk placement hints')
    .option('-t, --test', 'Enable test mode with deterministic computer moves')
    .helpOption('-h, --help', 'Print help and exit')
    .version(versionInfo(), '-v, --version', 'Print version and exit');

program.parse(process.argv);

const options = program.opts();
const sizeArgument = program.args[0];

if (options.autoplay && options.default) {
    process.stderr.write("error: the argument '--autoplay' cannot be used with '--default'\n");
    process.exit(2);
}

process.on('SIGINT', () => {
    console.log('\naborted...');
    process.exit(1);
});

console.log(chalk.green.bold('OTHELLO GAME - TYPESCRIPT'));

/** Resolve the board size to use from CLI arguments, or by asking the user. */
async function resolveBoardSize(): Promise<number> {
    // Try to read board size from command line args
    if (sizeArgument !== undefined) {
        const size = parseInt(sizeArgument);
        if (isNaN(size) || size < MIN_BOARD_SIZE || size > MAX_BOARD_SIZE) {
            printError(`Unsupported board size: ${sizeArgument}`);
            process.exit(1);
        }
        console.log(`Using board size: ${size}`);
        return size;
    }
    if (options.autoplay || options.default) {
        return DEFAULT_BOARD_SIZE;
    }
    // Otherwise ask user for board size
    return Othello.getBoardSize();
}

const boardSize = await resolveBoardSize();

const settings = new Settings(
    boardSize,
    Boolean(options.autoplay || options.check),
    Boolean(options.check),
    options.helpers,
    Boolean(options.log || options.check),
    Boolean(options.test || options.check),
    Boolean(options.default),
);

await new Othello(settings).play();
process.exit(0);
