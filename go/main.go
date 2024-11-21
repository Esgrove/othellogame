//==========================================================
// Main
// Play Othello (Reversi) on the command line
// https://en.wikipedia.org/wiki/Reversi
// Akseli Lukkarila
// 2019-2024
//==========================================================

package main

import (
	"fmt"
	"os"
	"strconv"

	// https://github.com/logrusorgru/aurora
	"github.com/logrusorgru/aurora/v4"

	// https://github.com/spf13/cobra
	"github.com/spf13/cobra"

	"othello_go/othello"
)

// CLI flags
var (
	autoplay   bool
	defaultOpt bool
	log        bool
	noHelpers  bool
	test       bool
	version    bool
)

func main() {
	// Override usage template so can have positional argument info there
	customUsageTemplate := fmt.Sprintf(`Usage:{{if .Runnable}}
  {{.UseLine}}{{end}}{{if .HasAvailableSubCommands}}
  {{.CommandPath}} [command]{{end}}{{if gt (len .Aliases) 0}}

Aliases:
  {{.NameAndAliases}}{{end}}{{if .HasExample}}

Examples:
{{.Example}}{{end}}{{if .HasAvailableSubCommands}}{{$cmds := .Commands}}{{if eq (len .Groups) 0}}

Available Commands:{{range $cmds}}{{if (or .IsAvailableCommand (eq .Name "help"))}}
  {{rpad .Name .NamePadding }} {{.Short}}{{end}}{{end}}{{else}}{{range $group := .Groups}}

{{.Title}}{{range $cmds}}{{if (and (eq .GroupID $group.ID) (or .IsAvailableCommand (eq .Name "help")))}}
  {{rpad .Name .NamePadding }} {{.Short}}{{end}}{{end}}{{end}}{{if not .AllChildCommandsHaveGroup}}

Additional Commands:{{range $cmds}}{{if (and (eq .GroupID "") (or .IsAvailableCommand (eq .Name "help")))}}
  {{rpad .Name .NamePadding }} {{.Short}}{{end}}{{end}}{{end}}{{end}}{{end}}{{if .HasAvailableLocalFlags}}

Arguments:
  [SIZE]             Optional board size (%d..%d)

Flags:
{{.LocalFlags.FlagUsages | trimTrailingWhitespaces}}{{end}}{{if .HasAvailableInheritedFlags}}

Global Flags:
{{.InheritedFlags.FlagUsages | trimTrailingWhitespaces}}{{end}}{{if .HasHelpSubCommands}}

Additional help topics:{{range .Commands}}{{if .IsAdditionalHelpTopicCommand}}
  {{rpad .CommandPath .CommandPathPadding}} {{.Short}}{{end}}{{end}}{{end}}{{if .HasAvailableSubCommands}}

Use "{{.CommandPath}} [command] --help" for more information about a command.{{end}}
`, othello.MinBoardSize, othello.MaxBoardSize)

	var rootCmd = &cobra.Command{
		Use:   "othello_go [SIZE]",
		Short: "A simple Othello CLI game implementation in Go",
		Long:  "A simple Othello CLI game implementation in Go",
		Args:  cobra.MaximumNArgs(1),
		Run: func(cmd *cobra.Command, args []string) {
			if version {
				fmt.Printf("Othello Go %s\n", othello.VersionInfo())
				os.Exit(0)
			}

			fmt.Println(aurora.Green("OTHELLO GAME - GO").Bold())

			var boardSize int
			if len(args) == 1 {
				size, err := strconv.Atoi(args[0])
				if err != nil {
					othello.PrintError("Invalid board size")
					os.Exit(1)
				} else if size < othello.MinBoardSize || size > othello.MaxBoardSize {
					othello.PrintError("Unsupported board size: %d", size)
					os.Exit(1)
				}
				boardSize = size
				fmt.Printf("Using board size: %d\n", boardSize)
			} else if autoplay || defaultOpt {
				boardSize = othello.DefaultBoardSize
			} else {
				boardSize = othello.GetBoardSize()
			}

			settings := othello.NewSettings(boardSize, autoplay, defaultOpt, !noHelpers, log, test)

			game := othello.InitOthello(settings)
			game.Play()
		},
	}

	rootCmd.SetUsageTemplate(customUsageTemplate)

	rootCmd.Flags().BoolVarP(&autoplay, "autoplay", "a", false, "Enable autoplay mode")
	rootCmd.Flags().BoolVarP(&defaultOpt, "default", "d", false, "Play with default settings")
	rootCmd.Flags().BoolVarP(&log, "log", "l", false, "Show log after a game")
	rootCmd.Flags().BoolVarP(&noHelpers, "no-helpers", "n", false, "Hide disk placement hints")
	rootCmd.Flags().BoolVarP(&test, "test", "t", false, "Enable test mode")
	rootCmd.Flags().BoolVarP(&version, "version", "v", false, "Print version and exit")

	if err := rootCmd.Execute(); err != nil {
		othello.PrintError(err.Error())
		os.Exit(1)
	}
}
