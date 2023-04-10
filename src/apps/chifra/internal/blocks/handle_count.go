// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package blocksPkg

import (
	"context"
	"errors"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/ethereum/go-ethereum"
)

func (opts *BlocksOptions) HandleCounts() error {
	// Don't do this in the loop
	meta, err := rpcClient.GetMetaData(opts.Globals.Chain, opts.Globals.TestMode)
	if err != nil {
		return err
	}
	if opts.Globals.TestMode {
		meta.Latest = 2000100
	}

	ctx, cancel := context.WithCancel(context.Background())
	fetchData := func(modelChan chan types.Modeler[types.RawModeler], errorChan chan error) {
		for _, br := range opts.BlockIds {
			blockNums, err := br.ResolveBlocks(opts.Globals.Chain)
			if err != nil {
				errorChan <- err
				if errors.Is(err, ethereum.NotFound) {
					continue
				}
				cancel()
				return
			}

			for _, bn := range blockNums {
				finalized := meta.Age(bn) > 28
				var block types.SimpleBlock[string]
				if block, err = rpcClient.GetBlockByNumber(opts.Globals.Chain, bn, finalized); err != nil {
					errorChan <- err
					if errors.Is(err, ethereum.NotFound) {
						continue
					}
					cancel()
					return
				}

				blockCount := simpleBlockCount{
					BlockNumber:     block.BlockNumber,
					Timestamp:       block.Timestamp,
					TransactionsCnt: uint64(len(block.Transactions)),
				}

				if opts.Uncles {
					if blockCount.UnclesCnt, err = rpcClient.GetUncleCountByNumber(opts.Globals.Chain, bn); err != nil {
						errorChan <- err
						if errors.Is(err, ethereum.NotFound) {
							continue
						}
						cancel()
						return
					}
				}

				if opts.Traces {
					if blockCount.TracesCnt, err = rpcClient.GetTraceCountByBlockNumber(opts.Globals.Chain, bn); err != nil {
						errorChan <- err
						if errors.Is(err, ethereum.NotFound) {
							continue
						}
						cancel()
						return
					}
				}

				if opts.Logs {
					if blockCount.LogsCnt, err = rpcClient.GetLogCountByBlockNumber(opts.Globals.Chain, bn); err != nil {
						errorChan <- err
						if errors.Is(err, ethereum.NotFound) {
							continue
						}
						cancel()
						return
					}
				}

				modelChan <- &blockCount
			}
		}
	}

	extra := map[string]interface{}{
		"count":  opts.Count,
		"uncles": opts.Uncles,
		"logs":   opts.Logs,
		"traces": opts.Traces,
		"apps":   opts.Apps,
		"uniqs":  opts.Uniq,
	}
	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOptsWithExtra(extra))
}

// TODO: BOGUS - MUST DOCUMENT
type simpleBlockCount struct {
	AppsCnt         uint64         `json:"appsCnt,omitempty"`
	BlockNumber     base.Blknum    `json:"blockNumber"`
	LogsCnt         uint64         `json:"logsCnt,omitempty"`
	Timestamp       base.Timestamp `json:"timestamp"`
	TracesCnt       uint64         `json:"tracesCnt,omitempty"`
	TransactionsCnt uint64         `json:"transactionsCnt"`
	UnclesCnt       uint64         `json:"unclesCnt,omitempty"`
	UniqsCnt        uint64         `json:"uniqsCnt,omitempty"`
}

func (s *simpleBlockCount) Raw() *types.RawModeler {
	return nil
}

func (s *simpleBlockCount) Model(showHidden bool, format string, extraOptions map[string]any) types.Model {
	model := map[string]interface{}{
		"blockNumber":     s.BlockNumber,
		"timestamp":       s.Timestamp,
		"transactionsCnt": s.TransactionsCnt,
	}

	order := []string{
		"blockNumber",
		"timestamp",
		"transactionsCnt",
	}

	wantsUncles := extraOptions["uncles"] == true
	wantsLogs := extraOptions["logs"] == true
	wantsTraces := extraOptions["traces"] == true
	wantsApps := extraOptions["apps"] == true
	wantsUniqs := extraOptions["uniqs"] == true

	if format == "json" {
		if wantsUncles && s.UnclesCnt > 0 {
			model["unclesCnt"] = s.UnclesCnt
		}
		if wantsLogs && s.LogsCnt > 0 {
			model["logsCnt"] = s.LogsCnt
		}
		if wantsTraces && s.TracesCnt > 0 {
			model["tracesCnt"] = s.TracesCnt
		}
		if wantsApps && s.AppsCnt > 0 {
			model["appsCnt"] = s.AppsCnt
		}
		if wantsUniqs && s.UniqsCnt > 0 {
			model["uniqsCnt"] = s.UniqsCnt
		}

	} else {
		if wantsUncles {
			model["unclesCnt"] = s.UnclesCnt
			order = append(order, "unclesCnt")
		}
		if wantsLogs {
			model["logsCnt"] = s.LogsCnt
			order = append(order, "logsCnt")
		}
		if wantsTraces {
			model["tracesCnt"] = s.TracesCnt
			order = append(order, "tracesCnt")
		}
		if wantsApps {
			model["appsCnt"] = s.AppsCnt
			order = append(order, "appsCnt")
		}
		if wantsUniqs {
			model["uniqsCnt"] = s.UniqsCnt
			order = append(order, "uniqsCnt")
		}
	}

	return types.Model{
		Data:  model,
		Order: order,
	}
}
