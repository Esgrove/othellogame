@testable import OthelloLib
import Testing

@Test func newPlayer() {
    let settings = PlayerSettings(showHelpers: true, checkMode: false, testMode: true)
    let player = Player(disk: .black, settings: settings)
    #expect(player.canPlay == true)
    #expect(player.human() == true)
    #expect(player.computer() == false)
}

@Test func resetPlayer() {
    let settings = PlayerSettings(showHelpers: true, checkMode: false, testMode: true)
    let player = Player(disk: .white, settings: settings)
    player.canPlay = false
    player.reset()
    #expect(player.canPlay == true)
}

@Test func setHumanAndComputer() {
    let settings = PlayerSettings(showHelpers: true, checkMode: false, testMode: true)
    let player = Player(disk: .black, settings: settings)
    #expect(player.human() == true)

    player.setComputer()
    #expect(player.computer() == true)
    #expect(player.human() == false)

    player.setHuman()
    #expect(player.human() == true)
    #expect(player.computer() == false)
}

@Test func playerTypeString() {
    let settings = PlayerSettings(showHelpers: true, checkMode: false, testMode: true)
    let player = Player(disk: .black, settings: settings)
    #expect(player.typeString() == "Human   ")

    player.setComputer()
    #expect(player.typeString() == "Computer")
}
