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
	"os/exec"
	"os/user"
	"sync"

	"github.com/spf13/cobra"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/cmd/root"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	homedir "github.com/mitchellh/go-homedir"
	"github.com/spf13/viper"
)

var cfgFile string

// rootCmd represents the base command when called without any subcommands
var rootCmd = &cobra.Command{
	Use:   "chifra [flags] commands",
	Short: "access to all TrueBlocks tools (chifra <cmd> --help for more)",
	Long: `Purpose:
  Access to all TrueBlocks tools (chifra <cmd> --help for more).`,
	Version: "Powered by TrueBlocks (GHC-TrueBlocks//0.15.0-alpha-44042b8cf-20211025)",
}

// Execute adds all child commands to the root command and sets flags appropriately.
func Execute() {
	if err := rootCmd.Execute(); err != nil {
		// fmt.Fprintf(os.Stderr, "%s", err)
		os.Exit(1)
	}
}

func init() {
	if utils.IsApiMode() {
		rootCmd.SetOut(os.Stderr)
		rootCmd.SetErr(os.Stdout)
	} else {
		rootCmd.SetOut(os.Stderr)
	}

	rootCmd.SetFlagErrorFunc(ErrFunc)

	rootCmd.Flags().SortFlags = false

	rootCmd.PersistentFlags().BoolVarP(&root.Options.Raw, "raw", "", false, "report JSON data from the node with minimal processing")
	rootCmd.PersistentFlags().MarkHidden("raw")

	rootCmd.PersistentFlags().BoolVarP(&root.Options.Version, "version", "", false, "display the current version of the tool")
	rootCmd.PersistentFlags().MarkHidden("version")

	rootCmd.PersistentFlags().BoolVarP(&root.Options.Noop, "noop", "", false, "")
	rootCmd.PersistentFlags().MarkHidden("noop")

	rootCmd.PersistentFlags().UintVarP(&root.Options.LogLevel, "log_level", "", 0, "")
	rootCmd.PersistentFlags().MarkHidden("log_level")

	rootCmd.PersistentFlags().BoolVarP(&root.Options.NoHeader, "no_header", "", false, "supress export of header row for csv and txt exports")
	rootCmd.PersistentFlags().MarkHidden("no_header")

	rootCmd.PersistentFlags().BoolVarP(&root.Options.Wei, "wei", "", false, "specify value in wei (the default)")
	rootCmd.PersistentFlags().MarkHidden("wei")

	rootCmd.PersistentFlags().BoolVarP(&root.Options.Ether, "ether", "", false, "specify value in ether")
	rootCmd.PersistentFlags().MarkHidden("ether")

	rootCmd.PersistentFlags().BoolVarP(&root.Options.Dollars, "dollars", "", false, "specify value in US dollars")
	rootCmd.PersistentFlags().MarkHidden("dollars")

	rootCmd.PersistentFlags().BoolVarP(&root.Options.ToFile, "to_file", "", false, "write the results to a temporary file and return the filename")
	rootCmd.PersistentFlags().MarkHidden("to_file")

	rootCmd.PersistentFlags().StringVarP(&root.Options.File, "file", "", "", "specify multiple sets of command line options in a file")
	rootCmd.PersistentFlags().MarkHidden("file")

	rootCmd.PersistentFlags().StringVarP(&output.OutputFn, "output", "", "", "write the results to file 'fn' and return the filename")
	rootCmd.PersistentFlags().MarkHidden("output")

	rootCmd.PersistentFlags().StringVarP(&output.Format, "fmt", "x", "", "export format, one of [none|json*|txt|csv|api]")
	rootCmd.PersistentFlags().BoolVarP(&root.Options.Verbose, "verbose", "v", false, "enable verbose (increase detail with --log_level)")
	rootCmd.PersistentFlags().BoolVarP(&root.Options.Help, "help", "h", false, "display this help screen")

	if (output.Format == "" || output.Format == "none") && utils.IsApiMode() {
		output.Format = "api"
	}

	rootCmd.Flags().SortFlags = false

	rootCmd.SetUsageTemplate(helpText)
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
	if utils.IsTestMode() {
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

func PassItOn(path string, flags, arguments string) {
	options := flags
	if root.Options.Raw {
		options += " --raw"
	}
	// if root.Options.Noop {
	// 	options += " --noop"
	// }
	if root.Options.Version {
		options += " --version"
	}
	if len(output.Format) > 0 {
		options += " --fmt " + output.Format
	}
	if root.Options.Verbose || root.Options.LogLevel > 0 {
		level := root.Options.LogLevel
		if level == 0 {
			level = 1
		}
		options += " --verbose " + fmt.Sprintf("%d", level)
	}
	if len(output.OutputFn) > 0 {
		options += " --output " + output.OutputFn
	}
	if root.Options.NoHeader {
		options += " --no_header"
	}
	if root.Options.Wei {
		options += " --wei"
	}
	if root.Options.Ether {
		options += " --ether"
	}
	if root.Options.Dollars {
		options += " --dollars"
	}
	if root.Options.ToFile {
		options += " --to_file"
	}
	if len(root.Options.File) > 0 {
		// TODO: one of the problems with this is that if the file contains invalid commands,
		// TODO: because we don't see those commands until we're doing into the tool, we
		// TODO: can't report on the 'bad command' in Cobra format. This will require us to
		// TODO: keep validation code down in the tools which we want to avoid. To fix this
		// TODO: the code below should open the file, read each command, and recursively call
		// TODO: into chifra here.
		options += " --file:" + root.Options.File
	}
	options += arguments

	var wg sync.WaitGroup
	wg.Add(2)

	// fmt.Fprintf(os.Stderr, "Calling: %s %s\n", path, options)
	cmd := exec.Command(getCommandPath(path), options)
	if utils.IsTestMode() {
		cmd.Env = append(os.Environ(), "TEST_MODE=true")
	}

	stderrPipe, err := cmd.StderrPipe()
	if err != nil {
		fmt.Fprintf(os.Stderr, "%s", err)
	} else {
		go func() {
			ScanForProgress(stderrPipe, func(msg string) {
			})
			wg.Done()
		}()
	}

	stdout, err := cmd.StdoutPipe()
	if err != nil {
		fmt.Fprintf(os.Stderr, "%s", err)
	} else {
		go func() {
			cmd.Start()
			scanner := bufio.NewScanner(stdout)
			buf := make([]byte, 1024*1024)
			scanner.Buffer(buf, 1024*1024)
			for scanner.Scan() {
				m := scanner.Text()
				fmt.Println(m)
			}
			wg.Done()
		}()
	}
	wg.Wait()
	cmd.Wait()
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
