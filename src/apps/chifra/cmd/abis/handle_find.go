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
	"io"
	"os"
	"sync"

	"github.com/ethereum/go-ethereum/crypto"
	ants "github.com/panjf2000/ants/v2"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

/*
   string_q name;
   string_q type;
   string_q abi_source;
   bool anonymous;
   bool constant;
   string_q stateMutability;
   string_q signature;
   string_q encoding;
   string_q message;
   CParameterArray inputs;
   CParameterArray outputs;
   Value inputs_dict;
   Value outputs_dict;
*/
type Function struct {
	Encoding  string `json:"encoding,omitempty"`
	Signature string `json:"signature,omitempty"`
}

type ScanCounter struct {
	Visited uint64
	Found   uint64
	Wanted  uint64
	Max     uint64
	Freq    uint64
}

func (v *ScanCounter) Satisfied() bool {
	return v.Visited > v.Max || v.Found == v.Wanted
}

func (v *ScanCounter) Report(target *os.File, action, msg string) {
	v.Visited++
	if v.Visited%v.Freq != 0 {
		return
	}
	fmt.Fprintf(target, "%s[%d-%d-%d] %s                                            \r", action, v.Visited, v.Max, (v.Max - v.Visited), msg)
}

// Find tries to find matching ABIs
func Find(arguments []string) []Function {
	visits := ScanCounter{}
	visits.Wanted = uint64(len(arguments))
	visits.Freq = 139419
	visits.Max = 50000000

	var results []Function

	testMode := utils.IsTestMode()

	var wg sync.WaitGroup
	checkOne, _ := ants.NewPoolWithFunc(config.ReadBlockScrape().Dev.MaxPoolSize, func(testSig interface{}) {
		defer wg.Done()
		byts := []byte(testSig.(string))
		sigBytes := crypto.Keccak256(byts)
		for _, arg := range arguments {
			if !testMode {
				visits.Report(os.Stderr, "Scanning", testSig.(string))
			}
			str, _ := hex.DecodeString(arg[2:])
			if bytes.Equal(sigBytes[:len(str)], str) {
				visits.Found++
				logger.Log(logger.Progress, "Found ", visits.Found, " of ", visits.Wanted, arg, testSig)
				results = append(results, Function{Encoding: arg, Signature: testSig.(string)})
				return
			}
		}
	})
	defer checkOne.Release()

	sigsFile, _ := os.Open(config.GetConfigPath("abis/known-000/uniq_sigs.tab"))
	defer func() {
		sigsFile.Close()
	}()
	sigsFile.Seek(0, io.SeekStart)
	sigsScanner := bufio.NewScanner(sigsFile)
	sigsScanner.Split(bufio.ScanLines)

	funcsFile, _ := os.Open(config.GetConfigPath("abis/known-000/uniq_funcs.tab"))
	defer func() {
		funcsFile.Close()
	}()

	for sigsScanner.Scan() {
		s := sigsScanner.Text()

		funcsFile.Seek(0, io.SeekStart)
		funcsScanner := bufio.NewScanner(funcsFile)
		funcsScanner.Split(bufio.ScanLines)

		for funcsScanner.Scan() {
			wg.Add(1)
			f := funcsScanner.Text()
			call := f + "(" + s + ")"
			_ = checkOne.Invoke(call)
		}

		if visits.Satisfied() {
			break
		}
	}

	defer wg.Wait()
	return results
}

func HandleFind(arguments []string) {
	results := Find(arguments)

	// TODO: if Root.to_file == true, write the output to a filename
	// TODO: if Root.output == <fn>, write the output to a <fn>
	err := output.Output(os.Stdout, output.Format, results)
	if err != nil {
		logger.Log(logger.Error, err)
	}
}
