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
        assertTrue(player.isHuman)
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
    fun setHuman() {
        val player = Player.default()

        assertTrue(player.isHuman)

        player.setComputer()
        assertFalse(player.isHuman)

        player.setHuman()
        assertTrue(player.isHuman)
    }

    @Test
    fun playerTypeString() {
        val player = Player.default()
        assertEquals("Human   ", player.typeString())

        player.setComputer()
        assertEquals("Computer", player.typeString())
    }
}
