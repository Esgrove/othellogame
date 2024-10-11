export class Player {
    static black(settings: PlayerSettings): Player {
        return new Player('black', settings);
    }

    static white(settings: PlayerSettings): Player {
        return new Player('white', settings);
    }

    color: string;
    settings: PlayerSettings;

    constructor(color: string, settings: PlayerSettings) {
        this.color = color;
        this.settings = settings;
    }
}

export class PlayerSettings {
    showHelpers: boolean;
    testMode: boolean;

    constructor(showHelpers: boolean, testMode: boolean) {
        this.showHelpers = showHelpers;
        this.testMode = testMode;
    }
}
