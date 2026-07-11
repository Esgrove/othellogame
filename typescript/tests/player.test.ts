import { describe, expect, test } from 'bun:test';

import chalk from 'chalk';

import { Player } from '../src/player.ts';
import { PlayerSettings } from '../src/settings.ts';

describe('player', () => {
    test('new black player', () => {
        const player = Player.black(PlayerSettings.default());
        expect(player.human()).toBe(true);
        expect(player.computer()).toBe(false);
        expect(player.canPlay).toBe(true);
        expect(player.toString()).toBe(`${chalk.magenta('BLACK')} | Human    | Moves: 0`);
    });

    test('new white player', () => {
        const player = Player.white(PlayerSettings.default());
        expect(player.human()).toBe(true);
        expect(player.computer()).toBe(false);
        expect(player.canPlay).toBe(true);
        expect(player.toString()).toBe(`${chalk.cyan('WHITE')} | Human    | Moves: 0`);
    });

    test('set human and computer', () => {
        const player = Player.black(PlayerSettings.default());
        expect(player.human()).toBe(true);

        player.setComputer();
        expect(player.computer()).toBe(true);
        expect(player.human()).toBe(false);

        player.setHuman();
        expect(player.human()).toBe(true);
        expect(player.computer()).toBe(false);
    });

    test('player type string', () => {
        const player = Player.black(PlayerSettings.default());
        expect(player.typeString()).toBe('Human   ');

        player.setComputer();
        expect(player.typeString()).toBe('Computer');
    });

    test('reset player', () => {
        const player = Player.white(PlayerSettings.default());
        player.canPlay = false;
        player.reset();
        expect(player.canPlay).toBe(true);
        expect(player.toString()).toBe(`${chalk.cyan('WHITE')} | Human    | Moves: 0`);
    });
});
