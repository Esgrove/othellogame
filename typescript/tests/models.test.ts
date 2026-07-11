import { describe, expect, test } from 'bun:test';

import { boardChar, Direction, Disk, Move, opponent, Square, Step } from '../src/models.ts';

describe('models', () => {
    test('square addition', () => {
        let result = new Square(4, 4).add(new Step(1, 1));
        expect(result.equals(new Square(5, 5))).toBe(true);

        result = new Square(4, 4).add(new Step(0, 0));
        expect(result.equals(new Square(4, 4))).toBe(true);

        result = new Square(4, 4).add(new Step(-1, 1));
        expect(result.equals(new Square(3, 5))).toBe(true);
    });

    test('step directions', () => {
        const origin = new Square(1, 1);
        const stepDirections = [
            new Step(-1, -1),
            new Step(-1, 0),
            new Step(-1, 1),
            new Step(0, -1),
            new Step(0, 1),
            new Step(1, -1),
            new Step(1, 0),
            new Step(1, 1),
        ];
        const expectedResults = [
            new Square(0, 0),
            new Square(0, 1),
            new Square(0, 2),
            new Square(1, 0),
            new Square(1, 2),
            new Square(2, 0),
            new Square(2, 1),
            new Square(2, 2),
        ];
        for (const [i, step] of stepDirections.entries()) {
            expect(origin.add(step).equals(expectedResults[i])).toBe(true);
        }
    });

    test('square board index', () => {
        expect(new Square(1, 1).boardIndex(8)).toBe(9);
        expect(new Square(0, 0).boardIndex(8)).toBe(0);
        expect(new Square(7, 7).boardIndex(8)).toBe(63);
        expect(new Square(3, 2).boardIndex(4)).toBe(11);
    });

    test('square to string', () => {
        expect(new Square(3, 2).toString()).toBe('(3,2)');
        expect(`${new Square(0, 0)}`).toBe('(0,0)');
    });

    test('disk board char', () => {
        expect(boardChar(Disk.Black)).toBe('B');
        expect(boardChar(Disk.Empty)).toBe('_');
        expect(boardChar(Disk.White)).toBe('W');
    });

    test('disk opponent', () => {
        expect(opponent(Disk.Black)).toBe(Disk.White);
        expect(opponent(Disk.Empty)).toBe(Disk.Empty);
        expect(opponent(Disk.White)).toBe(Disk.Black);
    });

    test('move log entry', () => {
        const black = new Move(new Square(3, 2), Disk.Black, 10, [new Direction(new Step(1, 0), 10)]);
        expect(black.logEntry()).toBe('B:(3,2),10');

        const white = new Move(new Square(0, 0), Disk.White, 1, [new Direction(new Step(1, 0), 1)]);
        expect(white.logEntry()).toBe('W:(0,0),1');
    });

    test('move ordering', () => {
        // Descending value, then ascending square (x, then y)
        const moves = [
            new Move(new Square(2, 2), Disk.Black, 1, []),
            new Move(new Square(0, 0), Disk.Black, 2, []),
            new Move(new Square(1, 1), Disk.Black, 2, []),
            new Move(new Square(0, 1), Disk.Black, 1, []),
        ];
        moves.sort(Move.compare);
        expect(moves.map((move) => move.logEntry())).toEqual(['B:(0,0),2', 'B:(1,1),2', 'B:(0,1),1', 'B:(2,2),1']);
    });

    test('move affected squares', () => {
        const move = new Move(new Square(0, 0), Disk.Black, 2, [new Direction(new Step(1, 0), 2)]);
        const squares = move.affectedSquares();
        expect(squares.length).toBe(2);
        expect(squares[0].equals(new Square(1, 0))).toBe(true);
        expect(squares[1].equals(new Square(2, 0))).toBe(true);
    });
});
