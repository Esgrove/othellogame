/** Player can be controlled either by a human or computer. */
export enum PlayerType {
    Human = 'Human',
    Computer = 'Computer',
}

/** Defines one player that can be either human or computer controlled. */
export class Player {
    /** Shorthand to initialize a new player for black disks. */
    static black(settings: PlayerSettings): Player {
        return new Player('black', settings, PlayerType.Human);
    }

    /** Shorthand to initialize a new player for white disks. */
    static white(settings: PlayerSettings): Player {
        return new Player('white', settings, PlayerType.Computer);
    }

    disk: string;
    settings: PlayerSettings;
    canPlay: boolean;
    playerType: PlayerType;

    constructor(disk: string, settings: PlayerSettings, playerType: PlayerType = PlayerType.Human) {
        this.disk = disk;
        this.settings = settings;
        this.canPlay = true;
        this.playerType = playerType;
    }

    /** Returns true if the player is human. */
    human(): boolean {
        return this.playerType === PlayerType.Human;
    }

    /** Returns true if the player is controlled by computer. */
    computer(): boolean {
        return this.playerType === PlayerType.Computer;
    }

    /** Set the player as human controlled. */
    setHuman(): void {
        this.playerType = PlayerType.Human;
    }

    /** Set the player as computer controlled. */
    setComputer(): void {
        this.playerType = PlayerType.Computer;
    }

    /** Set the player type. */
    setPlayerType(playerType: PlayerType): void {
        this.playerType = playerType;
    }

    /** Reset player status for a new game. */
    reset(): void {
        this.canPlay = true;
    }

    /** Return player type description string. */
    typeString(): string {
        return this.playerType === PlayerType.Human ? 'Human   ' : 'Computer';
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
