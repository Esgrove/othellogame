//==========================================================
// Colorprint
// Interface for coloured terminal printing
// Akseli Lukkarila
// 2019-2026
//==========================================================

import chalk from 'chalk';

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
