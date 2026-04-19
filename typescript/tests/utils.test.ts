import { describe, expect, test } from 'bun:test';

import { clamp } from '../src/utils.ts';

describe('utils', () => {
    test('clamp values', () => {
        expect(clamp(0, 0, 0)).toBe(0);
        expect(clamp(0, 0, 1)).toBe(0);
        expect(clamp(1, 0, 1)).toBe(1);
        expect(clamp(2, 0, 1)).toBe(1);
        expect(clamp(-2, -1, 0)).toBe(-1);
        expect(clamp(0, -1, 1)).toBe(0);
    });
});
