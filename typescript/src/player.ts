export class Player {
    static black(settings: PlayerSettings): Player {
        return new Player('black', settings);
    }

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
