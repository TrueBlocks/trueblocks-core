// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package cmd

import (
	"errors"
	"fmt"
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/base"
	"github.com/spf13/cobra"
)

// chifraCmd represents the base command when called without any subcommands
var chifraCmd = &cobra.Command{
	Use:   "chifra [flags] commands",
	Short: "access to all TrueBlocks tools (chifra <cmd> --help for more)",
	Long: `Purpose:
  Access to all TrueBlocks tools (chifra <cmd> --help for more).`,
	Version: versionText,
}

// Execute adds all child commands to the root command and sets flags appropriately.
func Execute() {
	if err := chifraCmd.Execute(); err != nil {
		// fmt.Fprintf(os.Stderr, "%s", err)
		os.Exit(1)
	}
}

func init() {
	chifraCmd.SetOut(os.Stderr)
	chifraCmd.SetFlagErrorFunc(ErrFunc)
	chifraCmd.Flags().SortFlags = false
	chifraCmd.SetUsageTemplate(helpText)
}

func ErrFunc(cmd *cobra.Command, errMsg error) error {
	_ = cmd
	msg := fmt.Sprintf("%s", errMsg)
	if base.IsTestMode() {
		msg = "\n  " + msg + "\n"
	} else {
		msg = "\n  \033[31m" + msg + "\033[0m\n"
	}
	return errors.New(msg)
}

func UsageWithNotes(notes string) string {
	t := `Usage:{{if .Runnable}}
  {{.UseLine}}{{end}}{{if .HasAvailableSubCommands}}
  {{.CommandPath}} [command]{{end}}{{if gt (len .Aliases) 0}}

Aliases:
  {{.NameAndAliases}}{{end}}{{if .HasExample}}

Examples:
{{.Example}}{{end}}{{if .HasAvailableSubCommands}}

Available Commands:{{range .Commands}}{{if (or .IsAvailableCommand (eq .Name "help"))}}
  {{rpad .Name .NamePadding }} {{.Short}}{{end}}{{end}}{{end}}{{if .HasAvailableLocalFlags}}

Flags:
{{.LocalFlags.FlagUsages | trimTrailingWhitespaces}}{{end}}{{if .HasAvailableInheritedFlags}}

Global Flags:
{{.InheritedFlags.FlagUsages | trimTrailingWhitespaces}}{{end}}{{if .HasHelpSubCommands}}

Additional help topics:{{range .Commands}}{{if .IsAdditionalHelpTopicCommand}}
  {{rpad .CommandPath .CommandPathPadding}} {{.Short}}{{end}}{{end}}{{end}}{{if .HasAvailableSubCommands}}

Use "{{.CommandPath}} [command] --help" for more information about a command.{{end}}
`

	usage := t + coloredNotes(notes)
	if usage != "" && usage[len(usage)-1] != '\n' {
		usage += "\n"
	}
	return usage
}

// TODO: Coloring in notes (search in makeClass for this note)
func coloredNotes(notes string) string {
	// if !strings.Contains(notes, "++") {
	// 	return notes
	// }
	// if os.Getenv("NO_COLOR") == "true" {
	// 	return strings.ReplaceAll(notes, "++", "")
	// }
	// cnt := 0
	// for {
	// 	if !strings.Contains(notes, "++") {
	// 		break
	// 	}
	// 	if cnt%2 == 0 {
	// 		notes = strings.Replace(notes, "++", colors.Cyan, 1)
	// 	} else {
	// 		notes = strings.Replace(notes, "++", colors.Off, 1)
	// 	}
	// 	cnt++
	// }
	// if cnt%2 == 1 {
	// 	logger.Panic("Mismatched backticks in notes. Quitting...")
	// }

	return notes
}
