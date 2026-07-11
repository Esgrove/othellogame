import { describe, expect, test } from 'bun:test';

import { Board } from '../src/board.ts';
import { Disk } from '../src/models.ts';

describe('board', () => {
    test('board initialization', () => {
        const board = new Board(8);
        expect(board.size).toBe(8);
        expect(board.logEntry()).toBe('___________________________WB______BW___________________________');
    });

    test('board initialization for small board', () => {
        const board = new Board(4);
        expect(board.logEntry()).toBe('_____WB__BW_____');
    });

    test('can play', () => {
        const board = new Board(8);
        expect(board.canPlay()).toBe(true);
    });

    test('possible moves', () => {
        const board = new Board(4);
        const moves = board.possibleMoves(Disk.Black);
        expect(moves.length).toBe(4);
        // All opening moves have value 1, sorted ascending by square
        expect(moves.every((move) => move.value === 1)).toBe(true);
    });

    test('place disk', () => {
        const board = new Board(4);
        const moves = board.possibleMoves(Disk.Black);
        board.placeDisk(moves[0]);
        expect(board.logEntry()).toBe('____BBB__BW_____');
    });

    test('game result', () => {
        const board = new Board(8);
        expect(board.result()).toBe(Disk.Empty);

        const moves = board.possibleMoves(Disk.Black);
        board.placeDisk(moves[0]);
        expect(board.result()).toBe(Disk.Black);
    });
});
