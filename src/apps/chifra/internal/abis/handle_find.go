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
	"sort"
	"strings"
	"sync"

	"github.com/ethereum/go-ethereum/crypto"
	ants "github.com/panjf2000/ants/v2"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/progress"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (opts *AbisOptions) HandleAbiFind() error {
	scanBar := progress.NewScanBar(uint64(len(opts.Find)) /* wanted */, 13919 /* freq */, 50000000 /* max */, .5)

	var results []types.SimpleFunction

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
				if len(opts.Find) < 2 || !opts.Globals.TestMode {
					logger.Log(logger.Progress, "Found ", scanBar.Found, " of ", scanBar.Wanted, arg, testSig)
				}
				results = append(results, types.SimpleFunction{Encoding: arg, Signature: testSig.(string)})
				return
			}
		}
	})
	defer checkOne.Release()

	// TODO: UnchainedIndex --> This could be part of unchained index
	sigsFile, err := os.OpenFile(config.GetPathToRootConfig()+"abis/known-000/uniq_sigs.tab", os.O_RDONLY, 0)
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
	funcsFile, _ := os.OpenFile(config.GetPathToRootConfig()+"abis/known-000/uniq_funcs.tab", os.O_RDONLY, 0)
	defer func() {
		funcsFile.Close()
	}()

	for sigsScanner.Scan() {
		s := sigsScanner.Text()
		hitsSignature := opts.hitsHint(s)

		funcsFile.Seek(0, io.SeekStart)
		funcsScanner := bufio.NewScanner(funcsFile)
		funcsScanner.Split(bufio.ScanLines)

		for funcsScanner.Scan() {
			f := funcsScanner.Text()
			hitsFunction := opts.hitsHint(f)
			if hitsSignature || hitsFunction {
				wg.Add(1)
				call := f + "(" + s + ")"
				_ = checkOne.Invoke(call)
			}
		}

		if scanBar.Satisfied() {
			break
		}
	}

	defer wg.Wait()

	if opts.Globals.TestMode {
		// Otherwise the test is not reproducable
		sort.Slice(results, func(i, j int) bool {
			return results[i].Signature < results[j].Signature
		})
	}

	// TODO: Fix export without arrays
	return globals.RenderSlice(&opts.Globals, results)
}

func (opts *AbisOptions) hitsHint(test string) bool {
	hits := len(opts.Hint) == 0
	if !hits {
		// test = strings.ToLower(test)
		for _, hint := range opts.Hint {
			if strings.Contains(test, hint) {
				return true
			}
		}
	}
	return hits
}
