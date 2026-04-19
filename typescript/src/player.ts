/** Defines one player that can be either human or computer controlled. */
export class Player {
    /** Shorthand to initialize a new player for black disks. */
    static black(settings: PlayerSettings): Player {
        return new Player('black', settings);
    }

    /** Shorthand to initialize a new player for white disks. */
    static white(settings: PlayerSettings): Player {
        return new Player('white', settings);
    }

    disk: string;
    settings: PlayerSettings;

    constructor(disk: string, settings: PlayerSettings) {
        this.disk = disk;
        this.settings = settings;
    }
}

/** Player settings. */
export class PlayerSettings {
    showHelpers: boolean;
    checkMode: boolean;
    testMode: boolean;

    constructor(showHelpers: boolean, checkMode: boolean, testMode: boolean) {
        this.showHelpers = showHelpers;
        this.checkMode = checkMode;
        this.testMode = testMode;
    }
}
