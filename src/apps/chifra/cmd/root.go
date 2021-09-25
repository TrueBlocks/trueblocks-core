package cmd

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

import (
	"bufio"
	"fmt"
	"io"
	"os"
	"os/exec"
	"strconv"
	"strings"

	"github.com/spf13/cobra"

	homedir "github.com/mitchellh/go-homedir"
	"github.com/spf13/viper"
)

var cfgFile string

// rootOptionsType Structure to carry command line and config file options
type rootOptTypes struct {
	fmt     string
	verbose uint
	help    bool
	raw     bool
}

var RootOpts rootOptTypes

// rootCmd represents the base command when called without any subcommands
var rootCmd = &cobra.Command{
	Use:   "chifra command [flags] arguments",
	Short: "access to all TrueBlocks tools (chifra <cmd> --help for more)",
	Long: `Purpose:
  Access to all TrueBlocks tools (chifra <cmd> --help for more).`,
	Version: "Powered by TrueBlocks (GHC-TrueBlocks//0.12.1-alpha-7c5fb3f2a-20210923)",
}

var PostNotes = ""

// Execute adds all child commands to the root command and sets flags appropriately.
func Execute() {
	if err := rootCmd.Execute(); err != nil {
		fmt.Fprintf(os.Stderr, "%s", PostNotes)
		os.Exit(1)
	}
	if RootOpts.help {
		fmt.Fprintf(os.Stderr, "%s", PostNotes)
	}
}

func init() {
	rootCmd.SetOut(os.Stderr)
	rootCmd.SetFlagErrorFunc(ErrFunc)

	rootCmd.Flags().SortFlags = false
	rootCmd.PersistentFlags().SortFlags = false
	rootCmd.PersistentFlags().BoolVarP(&RootOpts.raw, "raw", "", false, "report JSON data from the node with minimal processing")
	rootCmd.PersistentFlags().StringVarP(&RootOpts.fmt, "fmt", "x", "", "export format, one of [none|json*|txt|csv|api]")
	rootCmd.PersistentFlags().UintVarP(&RootOpts.verbose, "verbose", "v", 0, "set verbose level (optional level defaults to 1)")
	rootCmd.PersistentFlags().BoolVarP(&RootOpts.help, "help", "h", false, "display this help screen")
	rootCmd.Flags().SortFlags = false
	rootCmd.PersistentFlags().SortFlags = false

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

func ValidatePositionals(funcs ...cobra.PositionalArgs) cobra.PositionalArgs {
	return func(cmd *cobra.Command, args []string) error {
		for _, f := range funcs {
			err := f(cmd, args)
			if err != nil {
				return err
			}
		}
		return nil
	}
}

func ErrFunc(cmd *cobra.Command, errMsg error) error {
	msg := fmt.Sprintf("%s", errMsg)
	if IsTestMode() {
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
	// if len(data) > 0 && data[len(data)-1] == '\n' {
	// 	return data[0 : len(data)-1]
	// }
	return data
}

// ScanProgressLine looks for "lines" that end with `\r` not `\n` like usual
func ScanProgressLine(data []byte, atEOF bool) (advance int, token []byte, err error) {
	if atEOF && len(data) == 0 {
		return 0, nil, nil
	}
	// if i := bytes.IndexByte(data, '\r'); i >= 0 {
	// 	return i + 1, dropNL(data[0:i]), nil
	// }
	return bufio.ScanLines(data, atEOF)
}

// ScanForProgress watches stderr and picks of progress messages
func ScanForProgress(stderrPipe io.Reader, fn func(string)) {
	scanner := bufio.NewScanner(stderrPipe)
	scanner.Split(ScanProgressLine)
	for scanner.Scan() {
		text := scanner.Text()
		if len(text) > 0 {
			fmt.Fprintf(os.Stderr, "\n%s\n\n", text)
			if strings.Contains(text, "<PROG>") {
				fn(strings.SplitAfter(text, ":")[1])
			}
		}
	}

	if err := scanner.Err(); err != nil {
		fmt.Println("Error while reading stderr:", err)
	}
}

func PassItOn(path string, flags, arguments string) {
	options := flags
	if RootOpts.raw {
		options += " --raw"
	}
	if len(RootOpts.fmt) > 0 {
		options += " --fmt " + RootOpts.fmt
	}
	if RootOpts.verbose > 0 {
		options += " --verbose " + strconv.FormatUint(uint64(RootOpts.verbose), 10)
	}
	options += arguments

	cmd := exec.Command(path, options)

	stderrPipe, err := cmd.StderrPipe()
	if err != nil {
		fmt.Fprintf(os.Stderr, "%s", err)
	} else {
		go func() {
			ScanForProgress(stderrPipe, func(msg string) {
			})
		}()
	}

	stdout, _ := cmd.StdoutPipe()
	cmd.Start()
	scanner := bufio.NewScanner(stdout)
	for scanner.Scan() {
		m := scanner.Text()
		fmt.Println(m)
	}
	cmd.Wait()
}

func IsTestMode() bool {
	return os.Getenv("TEST_MODE") == "true"
}
