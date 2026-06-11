@testable import OthelloLib
import Testing

@Test func newPlayer() {
    let player = Player(disk: .black, settings: PlayerSettings.default())
    #expect(player.disk == .black)
    #expect(player.human() == true)
    #expect(player.canPlay == true)
    #expect(player.roundsPlayed == 0)
    #expect(player.settings == PlayerSettings.default())
}

@Test func resetPlayer() {
    let player = Player(disk: .white, settings: PlayerSettings.default())
    player.canPlay = false
    player.roundsPlayed = 10
    player.reset()
    #expect(player.canPlay == true)
    #expect(player.roundsPlayed == 0)
}

@Test func setHuman() {
    let player = Player(disk: .black, settings: PlayerSettings.default())
    player.setComputer()
    #expect(player.computer() == true)
    #expect(player.playerType == .computer)
    player.setHuman()
    #expect(player.human() == true)
    #expect(player.playerType == .human)
}

@Test func playerTypeString() {
    let player = Player(disk: .black, settings: PlayerSettings.default())
    #expect(player.typeString() == "Human   ")

    player.setComputer()
    #expect(player.typeString() == "Computer")
}
