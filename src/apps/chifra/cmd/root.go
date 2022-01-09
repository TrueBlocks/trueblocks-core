// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package cmd

import (
	"bufio"
	"bytes"
	"fmt"
	"io"
	"os"

	"github.com/spf13/cobra"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
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
	if utils.IsApiMode() {
		chifraCmd.SetOut(os.Stderr)
		chifraCmd.SetErr(os.Stdout)
	} else {
		chifraCmd.SetOut(os.Stderr)
	}
	chifraCmd.SetFlagErrorFunc(ErrFunc)
	chifraCmd.Flags().SortFlags = false

	chifraCmd.SetUsageTemplate(helpText)
}

func ErrFunc(cmd *cobra.Command, errMsg error) error {
	msg := fmt.Sprintf("%s", errMsg)
	if os.Getenv("TEST_MODE") == "true" {
		msg = "\n  " + msg + "\n"
	} else {
		msg = "\n  \033[31m" + msg + "\033[0m\n"
	}
	return fmt.Errorf(msg)
}

// enums
// https://pkg.go.dev/github.com/thediveo/enumflag

// dropNL drops new line characters (\n) from the progress stream
func dropNL(data []byte) []byte {
	if len(data) > 0 && data[len(data)-1] == '\n' {
		return data[0 : len(data)-1]
	}
	return data
}

// ScanProgressLine looks for "lines" that end with `\r` not `\n` like usual
func ScanProgressLine(data []byte, atEOF bool) (advance int, token []byte, err error) {
	if atEOF && len(data) == 0 {
		return 0, nil, nil
	}
	if i := bytes.IndexByte(data, '\n'); i >= 0 {
		fmt.Fprintf(os.Stderr, "%s\n", string(data[0:i]))
		return i + 1, data[0:i], nil
	}
	if i := bytes.IndexByte(data, '\r'); i >= 0 {
		fmt.Fprintf(os.Stderr, "%s\r", string(data[0:i]))
		return i + 1, dropNL(data[0:i]), nil
	}
	return bufio.ScanLines(data, atEOF)
}

// ScanForProgress watches stderr and picks of progress messages
func ScanForProgress(stderrPipe io.Reader, fn func(string)) {
	scanner := bufio.NewScanner(stderrPipe)
	scanner.Split(ScanProgressLine)
	for scanner.Scan() {
		// we've already printed the token
	}
	if err := scanner.Err(); err != nil {
		fmt.Println("TB: Error while reading stderr -- ", err)
	}
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
	return t + notes
}
