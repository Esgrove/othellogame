package othello;

import org.junit.jupiter.api.Test;

import othello.Models.Disk;

import static org.junit.jupiter.api.Assertions.*;

public class PlayerTest {

    @Test
    public void newPlayer() {
        Player player = Player.black(PlayerSettings.defaultSettings());
        assertEquals(Disk.BLACK, player.disk);
        assertTrue(player.human());
        assertTrue(player.canPlay);
        assertEquals(0, player.roundsPlayed);
        assertEquals(PlayerSettings.defaultSettings(), player.settings);
    }

    @Test
    public void resetPlayer() {
        Player player = new Player(Disk.WHITE, PlayerSettings.defaultSettings());
        player.canPlay = false;
        player.roundsPlayed = 10;
        player.reset();
        assertTrue(player.canPlay);
        assertEquals(0, player.roundsPlayed);
    }

    @Test
    public void setHuman() {
        Player player = Player.black(PlayerSettings.defaultSettings());
        player.setComputer();
        assertTrue(player.computer());
        assertEquals(PlayerType.COMPUTER, player.playerType);
        player.setHuman();
        assertTrue(player.human());
        assertEquals(PlayerType.HUMAN, player.playerType);
    }

    @Test
    public void playerTypeString() {
        Player player = Player.black(PlayerSettings.defaultSettings());
        assertEquals("Human   ", player.typeString());

        player.setComputer();
        assertEquals("Computer", player.typeString());
    }
}
