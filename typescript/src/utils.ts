//==========================================================
// Utils
// Helper utilities
// Akseli Lukkarila
// 2019-2026
//==========================================================

import { createHash } from 'node:crypto';

/** Calculate SHA256 hash for the given string. */
export function calculateSha256(formattedLog: string): string {
    return createHash('sha256').update(formattedLog).digest('hex');
}

/** Clamp value to the given range. */
export function clamp(value: number, min: number, max: number): number {
    return Math.min(Math.max(value, min), max);
}

// Shared stdin line iterator, initialized lazily so importing this module
// does not touch stdin (for example in tests).
let stdinIterator: AsyncIterator<string> | undefined;

/** Read one line from stdin. Returns an empty string on EOF. */
export async function readLine(): Promise<string> {
    stdinIterator ??= console[Symbol.asyncIterator]();
    const { value, done } = await stdinIterator.next();
    return done || value === undefined ? '' : value;
}
