"""
Main
Play Othello (Reversi) on the command line
https://en.wikipedia.org/wiki/Reversi
Akseli Lukkarila
"""
import colorama

from util import Disk
from player import Player
from othello import Othello
from color import Color, printColor, printBold, getColor
#===========================================================

def main():
    colorama.init()
    printBold("OTHELLO GAME\n")
    
    size = int(input("Choose board size (4...8, default is 8): "))
    size = max(4, min(size, 8))
    game = Othello(size)

    playerOne = Player(Disk.BLACK)
    playerTwo = Player(Disk.WHITE)
    playAgainstComputer = input("Would you like to play against the computer (y/n)? ")
    if playAgainstComputer.lower() == "y":
        side = input("Would you like to play as black or white (b/w)? ")
        if side.lower() == "b":
            playerTwo.human = False
        else:
            playerOne.human = False

    printBold("\nPlayers:")
    print(playerOne)
    print(playerTwo, end="\n\n")
    print(game)
    while playerOne.canPlay or playerTwo.canPlay:
        print("turn: " + getColor("BLACK", Color.cyan))
        playerOne.playOneMove(game)

        print("turn: " + getColor("WHITE", Color.magenta))
        playerTwo.playOneMove(game)

        game.printScore()

    print("The game is finished!")
    print("Result:\n{}".format(game))
    print(playerOne)
    print(playerTwo + "\n")

    winner = game.getResult()
    if winner == Disk.WHITE:
        print("The white player won!")
    elif winner == Disk.BLACK:
        print("The black player won!")
    else:
        print("The game endend in a tie...")

#===========================================================

if __name__ == "__main__":
    main()
