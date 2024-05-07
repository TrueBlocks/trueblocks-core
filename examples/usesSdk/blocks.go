package main

import (
	"github.com/TrueBlocks/trueblocks-core/sdk"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// DoBlocks tests the Blocks sdk function
func DoBlocks() {
	file.EstablishFolder("usesSdk-output/blocks")
	opts := sdk.BlocksOptions{
		BlockIds: testBlocks,
	}
	ShowHeader("DoReceipts", opts)

	// Emitter     []string    `json:"emitter,omitempty"`
	// Topic       []string    `json:"topic,omitempty"`

	cacheTxs := []bool{false, true}
	cacheTraces := []bool{false, true}
	globs := noRaw(globals)

	for _, ctxs := range cacheTxs {
		for _, cts := range cacheTraces {
			for _, g := range globs {
				types := []string{"txs", "hashes", "uncles", "traces", "uniq", "logs", "withdrawals", "count"}
				for _, t := range types {
					opts := sdk.BlocksOptions{
						BlockIds: testBlocks,
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
							TestBlocks(t, fn3, &opts)
						}
					} else if t == "logs" {
						art := []bool{false, true}
						for _, a := range art {
							opts.Articulate = a
							if a {
								fn2 += "-articulate"
							}
							TestBlocks(t, fn2, &opts)
						}
					} else {
						TestBlocks(t, fn2, &opts)
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
	TestBlocks("logs", fn, &opts)

	opts.Emitter = []string{}
	opts.Topic = []string{"0xddf252ad1be2c89b69c2b068fc378daa952ba7f163c4a11628f55a4df523b3ef"}
	fn = getFilename(baseFn+"-topic", &opts.Globals)
	TestBlocks("logs", fn, &opts)

	opts.Emitter = []string{"0x86fa049857e0209aa7d9e616f7eb3b3b78ecfdb0"}
	fn = getFilename(baseFn+"-emitter-topic", &opts.Globals)
	TestBlocks("logs", fn, &opts)
}

func TestBlocks(which, fn string, opts *sdk.BlocksOptions) {
	switch which {
	case "txs":
		if blocks, _, err := opts.Blocks(); err != nil {
			ReportError(fn, err)
		} else {
			if err := SaveToFile[types.Block[types.Transaction]](fn, blocks); err != nil {
				ReportError(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	case "hashes":
		if blocks, _, err := opts.BlocksHashes(); err != nil {
			ReportError(fn, err)
		} else {
			if err := SaveToFile[types.Block[string]](fn, blocks); err != nil {
				ReportError(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	case "uncles":
		if uncles, _, err := opts.BlocksUncles(); err != nil {
			ReportError(fn, err)
		} else {
			if err := SaveToFile[types.Block[string]](fn, uncles); err != nil {
				ReportError(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	case "traces":
		if traces, _, err := opts.BlocksTraces(); err != nil {
			ReportError(fn, err)
		} else {
			if err := SaveToFile[types.Trace](fn, traces); err != nil {
				ReportError(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	case "uniq":
		if apps, _, err := opts.BlocksUniq(); err != nil {
			ReportError(fn, err)
		} else {
			if err := SaveToFile[types.Appearance](fn, apps); err != nil {
				ReportError(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	case "logs":
		if logs, _, err := opts.BlocksLogs(); err != nil {
			ReportError(fn, err)
		} else {
			if err := SaveToFile[types.Log](fn, logs); err != nil {
				ReportError(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	case "withdrawals":
		if withdrawals, _, err := opts.BlocksWithdrawals(); err != nil {
			ReportError(fn, err)
		} else {
			if err := SaveToFile[types.Withdrawal](fn, withdrawals); err != nil {
				ReportError(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	case "count":
		if counts, _, err := opts.BlocksCount(); err != nil {
			ReportError(fn, err)
		} else {
			if err := SaveToFile[types.BlockCount](fn, counts); err != nil {
				ReportError(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	}
}
