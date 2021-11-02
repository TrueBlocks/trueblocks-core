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
	"encoding/hex"
	"fmt"
	"log"
	"os"
	"runtime"
	"sync"

	"github.com/ethereum/go-ethereum/crypto"
	ants "github.com/panjf2000/ants/v2"
	"github.com/spf13/cobra"

	homedir "github.com/mitchellh/go-homedir"
)

// OptionsType Structure to carry command line and config file options
type OptionsType struct {
	fmt            string
	signaturesPath string
}

// Options Carries the configuration options (from both command line and config file)
var Options OptionsType

func IsTestMode() bool {
	return os.Getenv("TEST_MODE") == "true"
}

func prettyPrint(encoding string, signature interface{}) {
	if !IsTestMode() {
		fmt.Fprintf(os.Stderr, "                                                                        \r")
	}
	if Options.fmt == "json" {
		fmt.Printf("{ \"encoding\": \"0x%s\", \"signature\": \"%s\" }\n", encoding, signature)
	} else if Options.fmt == "txt" {
		fmt.Printf("0x%s\t%s\n", encoding, signature)
	} else {
		fmt.Printf("\"0x%s\",\"%s\"\n", encoding, signature)
	}
}

func findEncoding(cmd *cobra.Command, args []string) {
	testMode := IsTestMode()
	apiMode := (os.Getenv("API_MODE") == "true")
	if apiMode {
		Options.fmt = "json"
	}

	var arguments [][]byte
	for _, arg := range args {
		if arg[:2] == "0x" {
			asBytes, err := hex.DecodeString(arg[2:])
			if err != nil {
				fmt.Printf("Couldn't parse argument %s\n", arg)
			} else {
				arguments = append(arguments, asBytes)
			}
		}
	}

	var wg sync.WaitGroup
	checkOne, _ := ants.NewPoolWithFunc(runtime.NumCPU()*2/3, func(input interface{}) {
		defer wg.Done()
		str := input.(string)
		byts := []byte(str)
		encBytes := crypto.Keccak256(byts)[:4]
		for _, arg := range arguments {
			if !testMode {
				fmt.Fprintf(os.Stderr, "Scanning: %s\r", input)
			}
			if bytes.Equal(encBytes, arg) {
				prettyPrint(hex.EncodeToString(encBytes), input)
				// wg.Done()
				// return
				os.Exit(0)
			}
		}
	})

	funcsFile, _ := os.Open(Options.signaturesPath + "uniq_funcs.tab")
	sigsFile, _ := os.Open(Options.signaturesPath + "uniq_sigs.tab")

	// TODO: The following closeFunc never runs because of the Exit at line 69. If I comment that out
	// TODO: and return instead, then the speed the tests run 1/2 as fast.
	closeFunc := func() {
		log.Println("Closing data files")
		funcsFile.Close()
		sigsFile.Close()
	}
	defer closeFunc()

	funcsScanner := bufio.NewScanner(funcsFile)
	funcsScanner.Split(bufio.ScanLines)

	sigsScanner := bufio.NewScanner(sigsFile)
	sigsScanner.Split(bufio.ScanLines)

	for sigsScanner.Scan() {
		s := sigsScanner.Text()
		for funcsScanner.Scan() {
			wg.Add(1)
			f := funcsScanner.Text()
			call := f + "(" + s + ")"
			_ = checkOne.Invoke(call)
		}
	}

	defer checkOne.Release()
	defer wg.Wait()

	fmt.Fprintf(os.Stderr, "                                                                        \r")
}

// abiFindCmd represents the base command when called without any subcommands
var abiFindCmd = &cobra.Command{
	Use:   "findSig",
	Short: "Do a brute force search for 4byte and event topics on cross product of names and signatures",
	Long: `
Description:
  This internal-use-only component searches the cross product of
  function and event names with likely signatures looking to match
  previously unmatched four bytes and event topics.`,
	Version: "GHC-TrueBlocks, LLC//0.8.1-alpha",
	Run:     findEncoding,
}

// Execute adds all child commands to the root command and sets flags appropriately.
// This is called by main.main(). It only needs to happen once to the abiFindCmd.
func Execute() {
	err := abiFindCmd.Execute()
	if err != nil {
		fmt.Println(err)
		os.Exit(1)
	}
}

func init() {
	cobra.OnInitialize(initConfig)
	abiFindCmd.PersistentFlags().StringVarP(&Options.fmt, "fmt", "f", "json", "format of the output (one of 'json', 'txt', or 'csv')")
}

func initConfig() {
	homeDir, err := homedir.Dir()
	if err != nil {
		fmt.Println(err)
		os.Exit(1)
	}

	// TODO: We need a better way to do this across mutliple apps. There should be a utility function
	// TODO: to deliver paths. GetConfigPath, GetCachePath, GetIndexPath, etc.
	configPath := "<unset>"
	if runtime.GOOS == "darwin" {
		configPath = homeDir + "/Library/Application Support/TrueBlocks"
	} else if runtime.GOOS == "linux" {
		configPath = homeDir + "/.local/share/trueblocks"
	} else {
		fmt.Println("Windows not supported.")
		os.Exit(1)
	}

	Options.signaturesPath = configPath + "/abis/known-000/"
}
