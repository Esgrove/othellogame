import { Command } from '@commander-js/extra-typings';
import chalk from 'chalk';

const MIN_BOARD_SIZE = 4;
const MAX_BOARD_SIZE = 10;

console.log(chalk.green('OTHELLO NODE.JS'));

const program  = new Command()
  .name("Othello Node")
  .description("A simple Othello CLI game implementation.")
  .argument('<SIZE>', `Optional board size (${MIN_BOARD_SIZE}..${MAX_BOARD_SIZE})`)
  .option('-a, --autoplay', 'Enable autoplay mode')
  .option('-d, --default', 'Play with default settings', false)
  .option('-l, --log', 'Show log after a game')
  .option('-n, --no-helpers', 'Hide disk placement hints')
  .option('-t, --test', 'Enable test mode')
  .helpOption('-h, --help', 'Print usage and exit')
  .version(`Othello Node 0.1.0`, '-v, --version', 'Print version and exit');

program.parse(process.argv);

const options = program.opts();

console.log(options)
