import { PlayerSettings } from './player.ts';

export class Settings {
    boardSize: number;
    autoplayMode: boolean;
    useDefaults: boolean;
    showHelpers: boolean;
    showLog: boolean;
    testMode: boolean;

    // Constructor to initialize the Settings class
    constructor(
        boardSize: number,
        autoplayMode: boolean,
        useDefaults: boolean,
        showHelpers: boolean,
        showLog: boolean,
        testMode: boolean
    ) {
        this.boardSize = boardSize;
        this.autoplayMode = autoplayMode;
        this.useDefaults = useDefaults;
        this.showHelpers = showHelpers;
        this.showLog = showLog;
        this.testMode = testMode;
    }

    // Static method to return default settings
    static default(): Settings {
        return new Settings(8, false, false, true, false, false);
    }

    // Method to convert settings to player-specific settings
    toPlayerSettings(): PlayerSettings {
        return new PlayerSettings(this.showHelpers, this.testMode);
    }
}

export function clamp(value: number, min: number, max: number): number {
    return Math.min(Math.max(value, min), max);
}
