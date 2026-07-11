@testable import OthelloLib
import Testing

@Test func stepAddition() {
    var result = Step(x: 0, y: 0) + Step(x: 1, y: 1)
    #expect(result == Step(x: 1, y: 1))

    result = Step(x: -1, y: 0) + Step(x: 1, y: 0)
    #expect(result == Step(x: 0, y: 0))

    result += Step(x: -1, y: -1)
    #expect(result == Step(x: -1, y: -1))

    result += Step(x: 1, y: 1)
    #expect(result == Step(x: 0, y: 0))
}

@Test func squareAddition() {
    var result = Square(x: 4, y: 4) + Square(x: 1, y: 1)
    #expect(result == Square(x: 5, y: 5))

    result = Square(x: 4, y: 4) + Square(x: 0, y: 0)
    #expect(result == Square(x: 4, y: 4))

    result = Square(x: 4, y: 4) + Step(x: -1, y: 1)
    #expect(result == Square(x: 3, y: 5))

    result += Square(x: 0, y: 0)
    #expect(result == Square(x: 3, y: 5))

    let another = Square(x: -3, y: -2) + Square(x: 2, y: 3)
    #expect(another == Square(x: -1, y: 1))

    result += Step(x: -1, y: -1)
    #expect(result == Square(x: 2, y: 4))

    result += Step(x: -1, y: -1)
    #expect(result == Square(x: 1, y: 3))

    result += Step(x: -1, y: -1)
    #expect(result == Square(x: 0, y: 2))

    result += Step(x: -1, y: -1)
    #expect(result == Square(x: -1, y: 1))

    result += Step(x: 1, y: -1)
    #expect(result == Square(x: 0, y: 0))

    result += Step(x: -1, y: -1)
    #expect(result == Square(x: -1, y: -1))
}

@Test func stepDirections() {
    let origin = Square(x: 1, y: 1)
    let steps: [(Step, Square)] = [
        (Step(x: -1, y: -1), Square(x: 0, y: 0)),
        (Step(x: -1, y: 0), Square(x: 0, y: 1)),
        (Step(x: -1, y: 1), Square(x: 0, y: 2)),
        (Step(x: 0, y: -1), Square(x: 1, y: 0)),
        (Step(x: 0, y: 1), Square(x: 1, y: 2)),
        (Step(x: 1, y: -1), Square(x: 2, y: 0)),
        (Step(x: 1, y: 0), Square(x: 2, y: 1)),
        (Step(x: 1, y: 1), Square(x: 2, y: 2)),
    ]
    for (step, expected) in steps {
        #expect(origin + step == expected)
    }
}

@Test func diskBoardChar() {
    #expect(Disk.black.boardChar() == "B")
    #expect(Disk.empty.boardChar() == "_")
    #expect(Disk.white.boardChar() == "W")
}

@Test func diskOpponent() {
    #expect(Disk.black.opponent() == .white)
    #expect(Disk.empty.opponent() == .empty)
    #expect(Disk.white.opponent() == .black)
}

@Test func moveLogEntry() {
    let b = Move(
        square: Square(x: 3, y: 2),
        disk: .black,
        value: 10,
        directions: [Direction(step: Step(x: 1, y: 0), count: 10)]
    )
    #expect(b.logEntry() == "B:(3,2),10")

    let w = Move(
        square: Square(x: 0, y: 0),
        disk: .white,
        value: 1,
        directions: [Direction(step: Step(x: 1, y: 0), count: 1)]
    )
    #expect(w.logEntry() == "W:(0,0),1")
}
