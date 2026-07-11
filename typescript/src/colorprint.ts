//==========================================================
// Colorprint
// Interface for coloured terminal printing
// Akseli Lukkarila
// 2019-2026
//==========================================================

import chalk from 'chalk';

/** Print text with bold style. */
export function printBold(text: string): void {
    console.log(chalk.bold(text));
}

/** Print text with green colour. */
export function printGreen(text: string): void {
    console.log(chalk.green(text));
}

/** Print text with green colour and bold style. */
export function printGreenBold(text: string): void {
    console.log(chalk.green.bold(text));
}

/** Print text with yellow colour. */
export function printYellow(text: string): void {
    console.log(chalk.yellow(text));
}

/** Print text with yellow colour and bold style. */
export function printYellowBold(text: string): void {
    console.log(chalk.yellow.bold(text));
}

/** Print text with red colour. */
export function printRed(text: string): void {
    console.log(chalk.red(text));
}

/** Print text with red colour and bold style. */
export function printRedBold(text: string): void {
    console.log(chalk.red.bold(text));
}

/** Print error message with red colour. */
export function printError(message: string): void {
    const [indent, text] = splitLeadingWhitespace(message);
    console.log(chalk.red(`${indent}Error: ${text}`));
}

/** Print warning message with yellow colour. */
export function printWarn(message: string): void {
    const [indent, text] = splitLeadingWhitespace(message);
    console.log(chalk.yellow(`${indent}Warning: ${text}`));
}

/** Split a string into the leading whitespace and the rest of the string. */
function splitLeadingWhitespace(message: string): [string, string] {
    const indentSize = message.length - message.trimStart().length;
    return [message.slice(0, indentSize), message.slice(indentSize)];
}
