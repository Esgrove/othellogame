//==========================================================
// Settings
// Defines game settings
// Akseli Lukkarila
// 2019-2026
//==========================================================

export const MIN_BOARD_SIZE = 4;
export const MAX_BOARD_SIZE = 10;
export const DEFAULT_BOARD_SIZE = 8;

/** Player settings. */
export class PlayerSettings {
    constructor(
        readonly showHelpers: boolean,
        readonly checkMode: boolean,
        readonly testMode: boolean,
    ) {}

    /** Return default player settings. */
    static default(): PlayerSettings {
        return new PlayerSettings(true, false, false);
    }
}

/** Game settings. */
export class Settings {
    constructor(
        readonly boardSize: number,
        readonly autoplayMode: boolean,
        readonly checkMode: boolean,
        readonly showHelpers: boolean,
        readonly showLog: boolean,
        readonly testMode: boolean,
        readonly useDefaults: boolean,
    ) {}

    /** Return default settings. */
    static default(): Settings {
        return new Settings(DEFAULT_BOARD_SIZE, false, false, true, false, false, false);
    }

    /** Get player setting values from overall game settings. */
    toPlayerSettings(): PlayerSettings {
        return new PlayerSettings(this.showHelpers, this.checkMode, this.testMode);
    }

    toString(): string {
        return (
            `Settings:\n` +
            `  board_size: ${this.boardSize}\n` +
            `  autoplay: ${this.autoplayMode}\n` +
            `  check_mode: ${this.checkMode}\n` +
            `  use_defaults: ${this.useDefaults}\n` +
            `  show_helpers: ${this.showHelpers}\n` +
            `  show_log: ${this.showLog}\n` +
            `  test_mode: ${this.testMode}`
        );
    }
}
