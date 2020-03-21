using System;
using System.Collections.Generic;
using System.Threading;

namespace Othello
{
    internal class Player
    {
        public Player(Disk color) {
            _color = color;
            _human = true;
            _random = new Random();
            _canPlay = true;
        }

        public bool CanPlay()
        {
            return _canPlay;
        }

        public void PlayOneMove(Board game)
        {
            Console.Write("Turn: ");
            ColorPrint.Write(DiskStringUpper() + "\n", _color.DiskColor());
            var moves = game.GetPossibleGetMoves(_color);
            if (moves.Count > 0) {
                _canPlay = true;
                if (_human) {
                    // sort moves in descending order according to value
                    moves.Sort((x, y) => y.Value.CompareTo(x.Value));
                    PrintMoves(moves);
                    while (true) {
                        Console.Write("  Give disk position (x,y): ");
                        var coords = Console.ReadLine();
                        if (!string.IsNullOrEmpty(coords) && coords.Length == 3 && coords[1] == ',') {
                            try {
                                var x = Convert.ToInt32(coords[0].ToString());
                                var y = Convert.ToInt32(coords[2].ToString());
                                if (game.PlaceDisc(x, y, _color)) {
                                    break;
                                }
                                ColorPrint.Error($"  Can't place a {_color.Name()} disk in square ({coords}).\n");
                            }
                            catch (FormatException) {
                                ColorPrint.Error("  Coordinates have to be integer numbers!\n");
                            }
                        } else {
                            ColorPrint.Error("  Give coordinates in the form 'x,y'!\n");
                        }
                    }
                } else {
                    // computer plays: pick a random move
                    Console.WriteLine("  Computer plays...");
                    Thread.Sleep(_random.Next(1000, 3000));
                    var pos = moves[_random.Next(moves.Count)].Square;
                    Console.WriteLine($"  -> {pos}");
                    game.PlaceDisc(pos.X, pos.Y, _color);
                }
                ++_rounds;
                Console.WriteLine("\nResult:");
                game.Print();
            } else {
                Console.WriteLine("  No moves available...");
                _canPlay = false;
            }
            Console.WriteLine("--------------------------------");
        }
        public void SetHuman(bool isHuman)
        {
            _human = isHuman;
        }
        public void Print()
        {
            ColorPrint.Write(_color.ToString().ToUpper(), _color.DiskColor());
            Console.WriteLine($" | {TypeString()} | moves: {_rounds}");
        }

        public override string ToString()
        {
            return $"{_color.ToString().ToUpper()} | {TypeString()} | moves: {_rounds}";
        }
        private string DiskStringUpper()
        {
            return _color.ToString().ToUpper();
        }
        private string TypeString()
        {
            return _human ? "Human   " : "Computer";
        }
        private static void PrintMoves(IReadOnlyCollection<Move> moves)
        {
            ColorPrint.Write($"  Possible plays ({moves.Count}):\n", ConsoleColor.Yellow);
            foreach (var move in moves) {
                Console.WriteLine($"  {move}");
            }
        }

        private readonly Disk _color;
        private readonly Random _random;
        private bool _canPlay;
        private bool _human;
        private int _rounds;
    }
}
