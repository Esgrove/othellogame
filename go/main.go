//==========================================================
// Main
// Play Othello (Reversi) on the command line
// https://en.wikipedia.org/wiki/Reversi
// Akseli Lukkarila
// 2019-2025
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

// CLI flags.
var (
	autoplay        bool
	check           bool
	defaultSettings bool
	log             bool
	noHelpers       bool
	test            bool
	version         bool
)

// Args initializes the CLI commands and flags
//
//nolint:lll
func Args() *cobra.Command {
	// Have to override usage template so can have positional argument info there :(
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

	CLI := &cobra.Command{
		Use:   "othello_go [SIZE]",
		Short: "A simple Othello CLI game implementation in Go",
		Long:  "A simple Othello CLI game implementation in Go",
		Args:  cobra.MaximumNArgs(1),
		Run:   runGame,
	}

	CLI.SetUsageTemplate(customUsageTemplate)
	initFlags(CLI)

	return CLI
}

// initFlags registers CLI flags
//
//nolint:lll
func initFlags(cmd *cobra.Command) {
	cmd.Flags().BoolVarP(&autoplay, "autoplay", "a", false, "Enable autoplay mode with computer control")
	cmd.Flags().BoolVarP(&check, "check", "c", false, "Autoplay and only print result")
	cmd.Flags().BoolVarP(&defaultSettings, "default", "d", false, "Play with default settings")
	cmd.Flags().BoolVarP(&log, "log", "l", false, "Show game log at the end")
	cmd.Flags().BoolVarP(&noHelpers, "no-helpers", "n", false, "Hide disk placement hints")
	cmd.Flags().BoolVarP(&test, "test", "t", false, "Enable test mode with deterministic computer moves")
	cmd.Flags().BoolVarP(&version, "version", "v", false, "Print version and exit")
}

func runGame(_ *cobra.Command, args []string) {
	if version {
		fmt.Printf("Othello Go %s\n", othello.VersionInfo())
		os.Exit(0)
	}

	fmt.Println(aurora.Green("OTHELLO GAME - GO").Bold())

	boardSize := resolveBoardSize(args)

	settings := othello.NewSettings(
		boardSize,
		autoplay || check,
		check,
		!noHelpers,
		log || check,
		test || check,
		defaultSettings,
	)

	game := othello.InitOthello(settings)
	game.Play()
}

func resolveBoardSize(args []string) int {
	// Try to read board size from command line args
	if len(args) == 1 {
		size, err := strconv.Atoi(args[0])
		if err != nil {
			othello.PrintError("Invalid board size")
			os.Exit(1)
		} else if size < othello.MinBoardSize || size > othello.MaxBoardSize {
			othello.PrintError("Unsupported board size: %d", size)
			os.Exit(1)
		}
		fmt.Printf("Using board size: %d\n", size)
		return size
	} else if autoplay || defaultSettings {
		return othello.DefaultBoardSize
	} else {
		// Otherwise ask user for board size
		return othello.GetBoardSize()
	}
}

func main() {
	CLI := Args()
	if err := CLI.Execute(); err != nil {
		othello.PrintError(err.Error())
		os.Exit(1)
	}
}
