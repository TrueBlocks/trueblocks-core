// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were auto generated. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */
package main

// EXISTING_CODE
import (
	"fmt"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/sdk/v3"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// EXISTING_CODE

// DoBlocks tests the Blocks sdk function
func DoBlocks() {
	file.EstablishFolder("sdkFuzzer-output/blocks")
	opts := sdk.BlocksOptions{}
	ShowHeader("DoBlocks", opts)

	globs := globals
	// Option 'flow.enum' is an emum
	emitter := fuzzEmitters
	topic := fuzzTopics
	articulate := []bool{false, true}
	cacheTxs := []bool{false, true}
	cacheTraces := []bool{false, true}
	// blocks is not fuzzed
	// Fuzz Loop
	// EXISTING_CODE
	_ = emitter
	_ = topic
	for _, ctxs := range cacheTxs {
		for _, cts := range cacheTraces {
			for _, g := range globs {
				types := []string{"blocks", "hashes", "uncles", "traces", "uniq", "logs", "withdrawals", "count"}
				for _, t := range types {
					opts := sdk.BlocksOptions{
						BlockIds: fuzzBlocks,
					}
					baseFn := "blocks/blocks"
					if ctxs {
						baseFn += "-cacheTxs"
						opts.CacheTxs = true
					}
					if cts {
						baseFn += "-cacheTraces"
						opts.CacheTraces = true
					}
					opts.Globals = g
					fn := getFilename(baseFn, &opts.Globals)
					fn2 := fn + "-" + t
					if t == "uncles" {
						opts.Cache = false
						opts.Decache = false
					}
					if t == "uniq" {
						flows := []sdk.BlocksFlow{sdk.NoBF, sdk.BFFrom, sdk.BFTo, sdk.BFReward}
						for _, f := range flows {
							fn += "-" + f.String()
							opts.Flow = f
							fn3 := fn2 + "-" + f.String()
							TestBlocks(t, "", fn3, &opts)
						}
					} else if t == "logs" {
						for _, art := range articulate {
							opts.Articulate = art
							if art {
								fn2 += "-articulate"
							}
							TestBlocks(t, "", fn2, &opts)
						}
					} else {
						TestBlocks(t, "", fn2, &opts)
					}
				}
			}
		}
	}

	baseFn := "blocks/blocks"
	opts = sdk.BlocksOptions{
		BlockIds: []string{"4012000-4012001"},
		Emitter:  []string{"0x86fa049857e0209aa7d9e616f7eb3b3b78ecfdb0"},
	}
	fn := getFilename(baseFn+"-emitter", &opts.Globals)
	TestBlocks("logs", "", fn, &opts)

	opts.Emitter = []string{}
	opts.Topic = []string{"0xddf252ad1be2c89b69c2b068fc378daa952ba7f163c4a11628f55a4df523b3ef"}
	fn = getFilename(baseFn+"-topic", &opts.Globals)
	TestBlocks("logs", "", fn, &opts)

	opts.Emitter = []string{"0x86fa049857e0209aa7d9e616f7eb3b3b78ecfdb0"}
	fn = getFilename(baseFn+"-emitter-topic", &opts.Globals)
	TestBlocks("logs", "", fn, &opts)
	// EXISTING_CODE
	Wait()
}

func TestBlocks(which, value, fn string, opts *sdk.BlocksOptions) {
	fn = strings.Replace(fn, ".json", "-"+which+".json", 1)
	// EXISTING_CODE
	// EXISTING_CODE

	switch which {
	case "blocks":
		if blocks, _, err := opts.Blocks(); err != nil {
			ReportError(fn, opts, err)
		} else {
			if err := SaveToFile[types.Block](fn, blocks); err != nil {
				ReportError2(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	case "hashes":
		if hashes, _, err := opts.BlocksHashes(); err != nil {
			ReportError(fn, opts, err)
		} else {
			if err := SaveToFile[types.LightBlock](fn, hashes); err != nil {
				ReportError2(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	case "uncles":
		if uncles, _, err := opts.BlocksUncles(); err != nil {
			ReportError(fn, opts, err)
		} else {
			if err := SaveToFile[types.LightBlock](fn, uncles); err != nil {
				ReportError2(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	case "traces":
		if traces, _, err := opts.BlocksTraces(); err != nil {
			ReportError(fn, opts, err)
		} else {
			if err := SaveToFile[types.Trace](fn, traces); err != nil {
				ReportError2(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	case "uniq":
		if uniq, _, err := opts.BlocksUniq(); err != nil {
			ReportError(fn, opts, err)
		} else {
			if err := SaveToFile[types.Appearance](fn, uniq); err != nil {
				ReportError2(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	case "logs":
		if logs, _, err := opts.BlocksLogs(); err != nil {
			ReportError(fn, opts, err)
		} else {
			if err := SaveToFile[types.Log](fn, logs); err != nil {
				ReportError2(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	case "withdrawals":
		if withdrawals, _, err := opts.BlocksWithdrawals(); err != nil {
			ReportError(fn, opts, err)
		} else {
			if err := SaveToFile[types.Withdrawal](fn, withdrawals); err != nil {
				ReportError2(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	case "count":
		if count, _, err := opts.BlocksCount(); err != nil {
			ReportError(fn, opts, err)
		} else {
			if err := SaveToFile[types.BlockCount](fn, count); err != nil {
				ReportError2(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	default:
		ReportError(fn, opts, fmt.Errorf("unknown which: %s", which))
		logger.Fatal("Quitting...")
		return
	}
}

// EXISTING_CODE
// EXISTING_CODE
