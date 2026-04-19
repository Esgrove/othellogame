import { PlayerSettings } from './player.ts';

/** Game settings. */
export class Settings {
    boardSize: number;
    autoplayMode: boolean;
    checkMode: boolean;
    useDefaults: boolean;
    showHelpers: boolean;
    showLog: boolean;
    testMode: boolean;

    // Constructor to initialize the Settings class
    constructor(
        boardSize: number,
        autoplayMode: boolean,
        checkMode: boolean,
        useDefaults: boolean,
        showHelpers: boolean,
        showLog: boolean,
        testMode: boolean,
    ) {
        this.boardSize = boardSize;
        this.autoplayMode = autoplayMode;
        this.checkMode = checkMode;
        this.useDefaults = useDefaults;
        this.showHelpers = showHelpers;
        this.showLog = showLog;
        this.testMode = testMode;
    }

    /** Return default settings. */
    static default(): Settings {
        return new Settings(8, false, false, false, true, false, false);
    }

    /** Convert settings to player-specific settings. */
    toPlayerSettings(): PlayerSettings {
        return new PlayerSettings(this.showHelpers, this.checkMode, this.testMode);
    }
}

/** Clamp value to the given range. */
export function clamp(value: number, min: number, max: number): number {
    return Math.min(Math.max(value, min), max);
}
