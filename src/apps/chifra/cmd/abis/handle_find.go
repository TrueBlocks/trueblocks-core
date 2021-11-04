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
package abis

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

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

// HandleFind loads manifest, sorts pins and prints them out
func HandleFind(args []string) {
	testMode := utils.IsTestMode()
	apiMode := utils.IsApiMode()
	if apiMode || output.Format == "" || output.Format == "none" {
		output.Format = "json"
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
				// TODO: see issue #1873
				// wg.Done()
				// return
				os.Exit(0)
			}
		}
	})

	funcsFile, _ := os.Open(config.GetConfigPath("abis/known-000/uniq_funcs.tab"))
	sigsFile, _ := os.Open(config.GetConfigPath("abis/known-000/uniq_sigs.tab"))

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

func prettyPrint(encoding string, signature interface{}) {
	if !utils.IsTestMode() {
		fmt.Fprintf(os.Stderr, "                                                                        \r")
	}
	if output.Format == "json" {
		fmt.Printf("{ \"encoding\": \"0x%s\", \"signature\": \"%s\" }\n", encoding, signature)
	} else if output.Format == "txt" {
		fmt.Printf("0x%s\t%s\n", encoding, signature)
	} else {
		fmt.Printf("\"0x%s\",\"%s\"\n", encoding, signature)
	}
}
