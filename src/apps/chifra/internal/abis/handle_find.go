// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package abisPkg

import (
	"bufio"
	"bytes"
	"encoding/hex"
	"io"
	"os"
	"runtime"
	"sync"

	"github.com/ethereum/go-ethereum/crypto"
	ants "github.com/panjf2000/ants/v2"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/progress"
)

func (opts *AbisOptions) HandleAbiFind() error {
	scanBar := progress.NewScanBar(uint64(len(opts.Find)) /* wanted */, 139419 /* freq */, 50000000 /* max */)

	var results []Function

	var wg sync.WaitGroup
	checkOne, _ := ants.NewPoolWithFunc(runtime.NumCPU()*2, func(testSig interface{}) {
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

	// TODO: UnchainedIndex --> This could be part of unchained index
	sigsFile, err := os.Open(config.GetPathToRootConfig() + "abis/known-000/uniq_sigs.tab")
	if err != nil {
		return err
	}
	defer func() {
		sigsFile.Close()
	}()
	sigsFile.Seek(0, io.SeekStart)
	sigsScanner := bufio.NewScanner(sigsFile)
	sigsScanner.Split(bufio.ScanLines)

	// TODO: UnchainedIndex --> This could be part of unchained index
	funcsFile, _ := os.Open(config.GetPathToRootConfig() + "abis/known-000/uniq_funcs.tab")
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

	// TODO: Fix export without arrays
	b := make([]interface{}, len(results))
	for i := range results {
		b[i] = results[i]
	}
	return opts.Globals.RenderSlice(b)
}

// TODO: These are not implemented
// TODO: if Root.to_file == true, write the output to a filename
// TODO: if Root.output == <fn>, write the output to a <fn>

type Function struct {
	Encoding  string `json:"encoding,omitempty"`
	Signature string `json:"signature,omitempty"`
}
