/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
package cmd

import (
	"bufio"
	"bytes"
	"fmt"
	"io"
	"os"
	"os/user"

	"github.com/spf13/cobra"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	homedir "github.com/mitchellh/go-homedir"
	"github.com/spf13/viper"
)

var cfgFile string

// chifraCmd represents the base command when called without any subcommands
var chifraCmd = &cobra.Command{
	Use:   "chifra [flags] commands",
	Short: "access to all TrueBlocks tools (chifra <cmd> --help for more)",
	Long: `Purpose:
  Access to all TrueBlocks tools (chifra <cmd> --help for more).`,
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
	cobra.OnInitialize(initConfig)
}

// initConfig reads in config file and ENV variables if set.
func initConfig() {
	if cfgFile != "" {
		// Use config file from the flag.
		viper.SetConfigFile(cfgFile)
	} else {
		// Find home directory.
		home, err := homedir.Dir()
		if err != nil {
			fmt.Println(err)
			os.Exit(1)
		}

		// Search config in home directory with name ".chifra" (without extension).
		viper.AddConfigPath(home)
		viper.SetConfigName(".chifra")
	}

	viper.AutomaticEnv() // read in environment variables that match

	// If a config file is found, read it in.
	if err := viper.ReadInConfig(); err == nil {
		fmt.Println("Using config file:", viper.ConfigFileUsed())
	}
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

// getCommandPath returns full path the the given tool
func getCommandPath(cmd string) string {
	usr, _ := user.Current()
	dir := usr.HomeDir
	return dir + "/.local/bin/chifra/" + cmd
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

// TODO: This should be generated from data
var helpText = `  Accounts:
    list          list every appearance of an address anywhere on the chain
    export        export full detail of transactions for one or more addresses
    monitors      add, remove, clean, and list address monitors
    names         query addresses or names of well known accounts
    abis          fetches the ABI for a smart contract
  Chain Data:
    blocks        retrieve one or more blocks from the chain or local cache
    transactions  retrieve one or more transactions from the chain or local cache
    receipts      retrieve receipts for the given transaction(s)
    logs          retrieve logs for the given transaction(s)
    traces        retrieve traces for the given transaction(s)
    when          find block(s) based on date, blockNum, timestamp, or 'special'
  Chain State:
    state         retrieve account balance(s) for one or more addresses at given block(s)
    tokens        retrieve token balance(s) for one or more addresses at given block(s)
  Admin:
    status        report on the status of the TrueBlocks system
    serve         serve the TrueBlocks API using the flame server
    scrape        scan the chain and update the TrueBlocks index of appearances
    chunks        manage and investigate chunks and bloom filters
    init          initialize the TrueBlocks system by downloading from IPFS
    pins          manage pinned index of appearances and associated blooms
  Other:
    quotes        freshen or display Ethereum price data. This tool has been deprecated
    explore       open an explorer for one or more addresses, blocks, or transactions
    slurp         fetch data from EtherScan for any address
  Flags:
    -h, --help    display this help screen

  Use "chifra [command] --help" for more information about a command.
`
