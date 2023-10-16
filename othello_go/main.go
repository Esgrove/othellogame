//==========================================================
// Main
// Play Othello (Reversi) on the command line
// https://en.wikipedia.org/wiki/Reversi
// Akseli Lukkarila
// 2019-2023
//==========================================================

//go:generate ./update_version.sh

package main

import (
	"fmt"
	"os"
	"strconv"

	// https://github.com/logrusorgru/aurora
	"github.com/logrusorgru/aurora/v4"
	// https://github.com/urfave/cli/
	"github.com/urfave/cli/v2"

	"othello_go/othello"
)

func main() {
	fmt.Println(aurora.Green("OTHELLO GAME - GO").Bold())

	// This `urfave/cli` lib is quite annoying to use,
	// but the Go standard lib `flags` is even more horrible.
	cli.HelpFlag = &cli.BoolFlag{
		Name:               "help",
		Aliases:            []string{"h"},
		Usage:              "Print help and exit",
		DisableDefaultText: true,
	}

	app := &cli.App{
		Name:  "othello_go",
		Usage: "A simple Othello CLI game implementation.",
		Flags: []cli.Flag{
			&cli.BoolFlag{
				Name:               "autoplay",
				Aliases:            []string{"a"},
				Usage:              "Enable autoplay mode",
				DisableDefaultText: true,
			},
			&cli.BoolFlag{
				Name:               "default",
				Aliases:            []string{"d"},
				Usage:              "Play with default settings",
				DisableDefaultText: true,
			},
			&cli.BoolFlag{
				Name:               "log",
				Aliases:            []string{"l"},
				Usage:              "Show log after a game",
				DisableDefaultText: true,
			},
			&cli.BoolFlag{
				Name:               "no-helpers",
				Aliases:            []string{"n"},
				Usage:              "Hide disk placement hints",
				DisableDefaultText: true,
			},
			&cli.BoolFlag{
				Name:               "test",
				Aliases:            []string{"t"},
				Usage:              "Enable test mode",
				DisableDefaultText: true,
			},
			&cli.BoolFlag{
				Name:               "version",
				Aliases:            []string{"v"},
				Usage:              "Print version and exit",
				DisableDefaultText: true,
			},
		},
		CustomAppHelpTemplate: `{{.Usage}}

USAGE:
    {{.HelpName}} [OPTIONS] [SIZE]

ARGUMENTS:
    [SIZE]    Optional board size

OPTIONS:
    {{range .VisibleFlags}}{{.}}
    {{end}}
`,
		Action: func(c *cli.Context) error {
			autoplay := c.Bool("autoplay")
			defaultOpt := c.Bool("default")
			log := c.Bool("log")
			noHelpers := c.Bool("no-helpers")
			test := c.Bool("test")
			version := c.Bool("version")

			if version {
				fmt.Printf("Othello Go %s\n", othello.VersionInfo())
				os.Exit(0)
			}

			if c.NArg() > 1 {
				fmt.Println("Too many positional arguments.")
				_ = cli.ShowAppHelp(c)
				os.Exit(1)
			}

			var boardSize int
			if c.NArg() == 1 {
				arg := c.Args().Get(0)
				size, err := strconv.Atoi(arg)
				if err != nil {
					othello.PrintError("Invalid board size")
					os.Exit(1)
				} else if size < othello.MinBoardSize || size > othello.MaxBoardSize {
					othello.PrintError("Unsupported board size: %d", size)
					os.Exit(1)
				} else {
					boardSize = size
					fmt.Printf("Using board size: %d\n", boardSize)
				}
			} else if autoplay || defaultOpt {
				boardSize = othello.DefaultBoardSize
			} else {
				boardSize = othello.GetBoardSize()
			}

			settings := othello.NewSettings(boardSize, autoplay, defaultOpt, !noHelpers, log, test)

			game := othello.InitOthello(settings)
			game.Play()

			return nil
		},
	}

	err := app.Run(os.Args)
	if err != nil {
		othello.PrintError(err.Error())
	}
}
