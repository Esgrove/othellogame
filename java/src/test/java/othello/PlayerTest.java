package othello;

import org.junit.jupiter.api.Test;

import othello.Models.Disk;

import static org.junit.jupiter.api.Assertions.*;

public class PlayerTest {

    @Test
    public void newPlayer() {
        Player player = Player.black(new PlayerSettings(false, true, false));
        assertTrue(player.canPlay());
        assertTrue(player.human());
        assertFalse(player.computer());
    }

    @Test
    public void resetPlayer() {
        Player player = Player.white(new PlayerSettings(false, true, false));
        player.reset();
        assertTrue(player.canPlay());
    }

    @Test
    public void setHumanAndComputer() {
        Player player = Player.black(new PlayerSettings(false, true, false));
        assertTrue(player.human());

        player.setComputer();
        assertTrue(player.computer());
        assertFalse(player.human());

        player.setHuman();
        assertTrue(player.human());
        assertFalse(player.computer());
    }
}
