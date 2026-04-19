package othello

import kotlin.test.Test
import kotlin.test.assertEquals
import kotlin.test.assertFalse
import kotlin.test.assertTrue

class PlayerTest {
    @Test
    fun newPlayer() {
        val player = Player.default()

        assertEquals(Disk.Black, player.disk)
        assertTrue(player.human())
        assertFalse(player.computer())
        assertTrue(player.canPlay)
        assertEquals(0, player.roundsPlayed)
        assertEquals(PlayerSettings.default(), player.settings)
    }

    @Test
    fun resetPlayer() {
        val player = Player.white(PlayerSettings.default())
        player.canPlay = false
        player.roundsPlayed = 10
        player.reset()
        assertTrue(player.canPlay)
        assertEquals(0, player.roundsPlayed)
    }

    @Test
    fun setHumanAndComputer() {
        val player = Player.default()

        assertTrue(player.human())

        player.setComputer()
        assertTrue(player.computer())
        assertFalse(player.human())

        player.setHuman()
        assertTrue(player.human())
        assertFalse(player.computer())
    }

    @Test
    fun playerTypeString() {
        val player = Player.default()
        assertEquals("Human   ", player.typeString())

        player.setComputer()
        assertEquals("Computer", player.typeString())
    }
}
