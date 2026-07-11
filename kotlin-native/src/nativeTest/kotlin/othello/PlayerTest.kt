package othello

import kotlin.test.Test
import kotlin.test.assertEquals
import kotlin.test.assertTrue

class PlayerTest {
    @Test
    fun newPlayer() {
        val player = Player.default()
        assertEquals(Disk.Black, player.disk)
        assertTrue(player.human())
        assertTrue(player.canPlay)
        assertEquals(0, player.roundsPlayed)
        assertEquals(PlayerSettings.default(), player.settings)
    }

    @Test
    fun resetPlayer() {
        val player = Player(Disk.White, PlayerSettings.default())
        player.canPlay = false
        player.roundsPlayed = 10
        player.reset()
        assertTrue(player.canPlay)
        assertEquals(0, player.roundsPlayed)
    }

    @Test
    fun setHuman() {
        val player = Player.default()
        player.setComputer()
        assertTrue(player.computer())
        assertEquals(PlayerType.Computer, player.playerType)
        player.setHuman()
        assertTrue(player.human())
        assertEquals(PlayerType.Human, player.playerType)
    }

    @Test
    fun playerTypeString() {
        val player = Player.default()
        assertEquals("Human   ", player.typeString())

        player.setComputer()
        assertEquals("Computer", player.typeString())
    }
}
