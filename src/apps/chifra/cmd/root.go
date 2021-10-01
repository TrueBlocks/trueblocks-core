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

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/utils"
	homedir "github.com/mitchellh/go-homedir"
	"github.com/spf13/viper"
)

var cfgFile string

// rootOptionsType Structure to carry command line and config file options
type rootOptTypes struct {
	fmt        string
	verbose    bool
	logLevel   uint
	noHeader   bool
	wei        bool
	ether      bool
	dollars    bool
	help       bool
	raw        bool
	outputFile string
	toFile     bool
	file       string
	version    bool
	noop       bool
	create     bool
	delete     bool
	update     bool
	remove     bool
	undelete   bool
}

var RootOpts rootOptTypes

// rootCmd represents the base command when called without any subcommands
var rootCmd = &cobra.Command{
	Use:   "chifra [flags] commands",
	Short: "access to all TrueBlocks tools (chifra <cmd> --help for more)",
	Long: `Purpose:
  Access to all TrueBlocks tools (chifra <cmd> --help for more).`,
	Version: "Powered by TrueBlocks (GHC-TrueBlocks//0.12.1-alpha-7c5fb3f2a-20210923)",
}

// Execute adds all child commands to the root command and sets flags appropriately.
func Execute() {
	if err := rootCmd.Execute(); err != nil {
		// fmt.Fprintf(os.Stderr, "%s", err)
		os.Exit(1)
	}
}

func init() {
	rootCmd.SetOut(os.Stderr)
	rootCmd.SetFlagErrorFunc(ErrFunc)
	// rootCmd.SetPersistentFlagErrorFunc(ErrFunc)
	rootCmd.Flags().SortFlags = false
	rootCmd.PersistentFlags().SortFlags = false
	rootCmd.PersistentFlags().BoolVarP(&RootOpts.raw, "raw", "", false, "report JSON data from the node with minimal processing")
	rootCmd.PersistentFlags().BoolVarP(&RootOpts.version, "version", "", false, "display the current version of the tool")
	rootCmd.PersistentFlags().BoolVarP(&RootOpts.noop, "noop", "", false, "")
	rootCmd.PersistentFlags().BoolVarP(&RootOpts.create, "create", "", false, "")
	rootCmd.PersistentFlags().BoolVarP(&RootOpts.delete, "delete", "", false, "")
	rootCmd.PersistentFlags().BoolVarP(&RootOpts.update, "update", "", false, "")
	rootCmd.PersistentFlags().BoolVarP(&RootOpts.remove, "remove", "", false, "")
	rootCmd.PersistentFlags().BoolVarP(&RootOpts.undelete, "undelete", "", false, "")

	rootCmd.PersistentFlags().StringVarP(&RootOpts.fmt, "fmt", "x", "", "export format, one of [none|json*|txt|csv|api]")
	rootCmd.PersistentFlags().BoolVarP(&RootOpts.verbose, "verbose", "v", false, "enable verbose (increase detail with --log_level)")
	rootCmd.PersistentFlags().UintVarP(&RootOpts.logLevel, "log_level", "", 0, "")
	rootCmd.PersistentFlags().BoolVarP(&RootOpts.noHeader, "no_header", "", false, "supress export of header row for csv and txt exports")
	rootCmd.PersistentFlags().BoolVarP(&RootOpts.wei, "wei", "", false, "specify value in wei (the default)")
	rootCmd.PersistentFlags().BoolVarP(&RootOpts.ether, "ether", "", false, "specify value in ether")
	rootCmd.PersistentFlags().BoolVarP(&RootOpts.dollars, "dollars", "", false, "specify value in US dollars")
	rootCmd.PersistentFlags().BoolVarP(&RootOpts.toFile, "to_file", "", false, "write the results to a temporary file and return the filename")
	rootCmd.PersistentFlags().StringVarP(&RootOpts.file, "file", "", "", "specify multiple sets of command line options in a file")
	rootCmd.PersistentFlags().StringVarP(&RootOpts.outputFile, "output", "", "", "write the results to file 'fn' and return the filename")
	rootCmd.PersistentFlags().BoolVarP(&RootOpts.help, "help", "h", false, "display this help screen")
	rootCmd.PersistentFlags().MarkHidden("log_level")
	rootCmd.PersistentFlags().MarkHidden("output")
	rootCmd.PersistentFlags().MarkHidden("raw")
	rootCmd.PersistentFlags().MarkHidden("create")
	rootCmd.PersistentFlags().MarkHidden("delete")
	rootCmd.PersistentFlags().MarkHidden("update")
	rootCmd.PersistentFlags().MarkHidden("remove")
	rootCmd.PersistentFlags().MarkHidden("undelete")

	rootCmd.PersistentFlags().MarkHidden("noop")
	rootCmd.PersistentFlags().MarkHidden("version")
	rootCmd.PersistentFlags().MarkHidden("wei")
	rootCmd.PersistentFlags().MarkHidden("ether")
	rootCmd.PersistentFlags().MarkHidden("dollars")
	rootCmd.PersistentFlags().MarkHidden("file")
	rootCmd.PersistentFlags().MarkHidden("to_file")
	rootCmd.PersistentFlags().MarkHidden("no_header")
	rootCmd.Flags().SortFlags = false
	rootCmd.PersistentFlags().SortFlags = false

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

// GetCommandPath returns full path the the given tool
func GetCommandPath(cmd string) string {
	usr, _ := user.Current()
	dir := usr.HomeDir
	return dir + "/.local/bin/chifra/" + cmd
}

func PassItOn(path string, flags, arguments string) {
	options := flags
	if RootOpts.raw {
		options += " --raw"
	}
	if RootOpts.create {
		options += " --create"
	}
	if RootOpts.delete {
		options += " --delete"
	}
	if RootOpts.update {
		options += " --update"
	}
	if RootOpts.remove {
		options += " --remove"
	}
	if RootOpts.undelete {
		options += " --undelete"
	}
	// if RootOpts.noop {
	// 	options += " --noop"
	// }
	if RootOpts.version {
		options += " --version"
	}
	if len(RootOpts.fmt) > 0 {
		options += " --fmt " + RootOpts.fmt
	}
	if RootOpts.verbose || RootOpts.logLevel > 0 {
		level := RootOpts.logLevel
		if level == 0 {
			level = 1
		}
		options += " --verbose " + fmt.Sprintf("%d", level)
	}
	if len(RootOpts.outputFile) > 0 {
		options += " --output " + RootOpts.outputFile
	}
	if RootOpts.noHeader {
		options += " --no_header"
	}
	if RootOpts.wei {
		options += " --wei"
	}
	if RootOpts.ether {
		options += " --ether"
	}
	if RootOpts.dollars {
		options += " --dollars"
	}
	if RootOpts.toFile {
		options += " --to_file"
	}
	if len(RootOpts.file) > 0 {
		options += " --file:" + RootOpts.file
	}
	options += arguments

	var wg sync.WaitGroup
	wg.Add(2)

	cmd := exec.Command(path, options)
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
    init          initialize the index of appearances by downloading Bloom filters
    pins          manage pinned index of appearances and associated Bloom filters
  Other:
    quotes        freshen and/or display Ethereum price data
    explore       open an explorer for a given address, block, or transaction
    slurp         fetch data from EtherScan for any address
  Flags:
    -h, --help    display this help screen

  Use "chifra [command] --help" for more information about a command.
`
