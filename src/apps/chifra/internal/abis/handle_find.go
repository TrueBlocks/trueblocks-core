package abisPkg

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
	"bytes"
	"encoding/hex"
	"io"
	"net/http"
	"os"
	"sync"

	"github.com/ethereum/go-ethereum/crypto"
	ants "github.com/panjf2000/ants/v2"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/progress"
)

func (opts *AbisOptions) FindInternal() error {
	scanBar := progress.NewScanBar(uint64(len(opts.Find)) /* wanted */, 139419 /* freq */, 50000000 /* max */)

	var results []Function

	var wg sync.WaitGroup
	checkOne, _ := ants.NewPoolWithFunc(config.ReadBlockScrape().Dev.MaxPoolSize, func(testSig interface{}) {
		defer wg.Done()
		byts := []byte(testSig.(string))
		sigBytes := crypto.Keccak256(byts)
		for _, arg := range opts.Find {
			if !opts.Globals.TestMode {
				scanBar.Report(os.Stderr, "Scanning", testSig.(string))
			}
			str, _ := hex.DecodeString(arg[2:])
			if bytes.Equal(sigBytes[:len(str)], str) {
				scanBar.Found++
				logger.Log(logger.Progress, "Found ", scanBar.Found, " of ", scanBar.Wanted, arg, testSig)
				results = append(results, Function{Encoding: arg, Signature: testSig.(string)})
				return
			}
		}
	})
	defer checkOne.Release()

	sigsFile, err := os.Open(config.GetConfigPath("abis/known-000/uniq_sigs.tab"))
	if err != nil {
		return err
	}
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

		if scanBar.Satisfied() {
			break
		}
	}

	defer wg.Wait()

	if opts.Globals.ApiMode {
		opts.Globals.Respond(opts.Globals.Writer, http.StatusOK, results)

	} else {
		err = opts.Globals.Output(os.Stdout, opts.Globals.Format, results)
		if err != nil {
			logger.Log(logger.Error, err)
		}
	}
	return nil
}

// TODO: These are not implemented
// TODO: if Root.to_file == true, write the output to a filename
// TODO: if Root.output == <fn>, write the output to a <fn>

type Function struct {
	Encoding  string `json:"encoding,omitempty"`
	Signature string `json:"signature,omitempty"`
}
