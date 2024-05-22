// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package abisPkg

import (
	"bufio"
	"bytes"
	"context"
	"encoding/hex"
	"io"
	"os"
	"runtime"
	"sort"
	"strings"
	"sync"

	"github.com/ethereum/go-ethereum/crypto"
	ants "github.com/panjf2000/ants/v2"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/progress"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (opts *AbisOptions) HandleAbiFind() error {
	testMode := opts.Globals.TestMode
	/* wanted */ /* freq */ /* max */
	scanBar := progress.NewScanBar(uint64(len(opts.Find)), 13919, 50000000, .5)

	// TODO: we might want to use utils.IterateOver Map here

	ctx, cancel := context.WithCancel(context.Background())
	fetchData := func(modelChan chan types.Modeler[types.Function], errorChan chan error) {
		var results []types.Function
		var wg sync.WaitGroup
		mutex := sync.Mutex{}
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
					logger.Progress(len(opts.Find) < 2, "Found", scanBar.Found, "of", scanBar.Wanted, arg, testSig)
					found := types.Function{Encoding: arg, Signature: testSig.(string)}
					if testMode {
						mutex.Lock()
						results = append(results, found)
						mutex.Unlock()
					} else {
						modelChan <- &found
					}
					return
				}
			}
		})
		defer checkOne.Release()

		// TODO: UnchainedIndex --> This could be part of unchained index
		sigsFile, err := os.OpenFile(config.PathToRootConfig()+"abis/known-000/uniq_sigs.tab", os.O_RDONLY, 0)
		if err != nil {
			errorChan <- err
			cancel()
			return
		}
		defer func() {
			sigsFile.Close()
		}()
		_, _ = sigsFile.Seek(0, io.SeekStart)
		sigsScanner := bufio.NewScanner(sigsFile)
		sigsScanner.Split(bufio.ScanLines)

		// TODO: UnchainedIndex --> This could be part of unchained index
		funcsFile, _ := os.OpenFile(config.PathToRootConfig()+"abis/known-000/uniq_funcs.tab", os.O_RDONLY, 0)
		defer func() {
			funcsFile.Close()
		}()

		for sigsScanner.Scan() {
			s := sigsScanner.Text()
			hitsSignature := opts.hitsHint(s)

			_, _ = funcsFile.Seek(0, io.SeekStart)
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

			if scanBar.Satisfied() && !opts.Globals.Verbose {
				break
			}
		}

		defer wg.Wait()

		if opts.Globals.TestMode {
			// Otherwise the test is not reproducable
			sort.Slice(results, func(i, j int) bool {
				return results[i].Signature < results[j].Signature
			})
			for _, item := range results {
				modelChan <- &item
			}
		}
	}

	extraOpts := map[string]interface{}{
		"encodingSignatureOnly": true,
	}
	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOptsWithExtra(extraOpts))
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
