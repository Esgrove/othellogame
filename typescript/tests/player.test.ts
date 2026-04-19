import { describe, expect, test } from 'bun:test';

import { Player, PlayerSettings, PlayerType } from '../src/player.ts';

describe('player', () => {
    test('new black player', () => {
        const settings = new PlayerSettings(true, false, true);
        const player = Player.black(settings);
        expect(player.disk).toBe('black');
        expect(player.human()).toBe(true);
        expect(player.computer()).toBe(false);
        expect(player.canPlay).toBe(true);
    });

    test('new white player', () => {
        const settings = new PlayerSettings(true, false, true);
        const player = Player.white(settings);
        expect(player.disk).toBe('white');
        expect(player.human()).toBe(false);
        expect(player.computer()).toBe(true);
    });

    test('set human and computer', () => {
        const settings = new PlayerSettings(true, false, true);
        const player = Player.black(settings);
        expect(player.human()).toBe(true);

        player.setComputer();
        expect(player.computer()).toBe(true);
        expect(player.human()).toBe(false);

        player.setHuman();
        expect(player.human()).toBe(true);
        expect(player.computer()).toBe(false);
    });

    test('player type string', () => {
        const settings = new PlayerSettings(true, false, true);
        const player = Player.black(settings);
        expect(player.typeString()).toBe('Human   ');

        player.setComputer();
        expect(player.typeString()).toBe('Computer');
    });

    test('reset player', () => {
        const settings = new PlayerSettings(true, false, true);
        const player = Player.black(settings);
        player.canPlay = false;
        player.reset();
        expect(player.canPlay).toBe(true);
    });
});
