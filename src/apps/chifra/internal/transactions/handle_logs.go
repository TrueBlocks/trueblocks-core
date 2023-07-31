// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package transactionsPkg

import (
	"context"
	"errors"
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/ethereum/go-ethereum"
)

func (opts *TransactionsOptions) HandleLogs() error {
	chain := opts.Globals.Chain
	rpcOptions := rpcClient.DefaultRpcOptions(&rpcClient.DefaultRpcOptionsSettings{
		Chain: opts.Globals.Chain,
	})

	ctx, cancel := context.WithCancel(context.Background())
	fetchData := func(modelChan chan types.Modeler[types.RawLog], errorChan chan error) {
		for _, ids := range opts.TransactionIds {
			txIds, err := ids.ResolveTxs(opts.Globals.Chain)
			if err != nil {
				errorChan <- err
				if errors.Is(err, ethereum.NotFound) {
					continue
				}
				cancel()
				return
			}

			// Timestamp is not part of the raw trace data so we need to get it separately
			// TxIds don't span blocks, so we can use the first one outside the loop to find timestamp
			for _, id := range txIds {
				emitters := []base.Address{}
				for _, e := range opts.Emitter {
					emitters = append(emitters, base.HexToAddress(e))
				}
				topics := []base.Hash{}
				for _, t := range opts.Topic {
					topics = append(topics, base.HexToHash(t))
				}
				logFilter := types.SimpleLogFilter{
					FromBlock: uint64(id.BlockNumber),
					ToBlock:   uint64(id.BlockNumber),
					Emitters:  emitters,
					Topics:    topics,
				}

				if opts.Globals.TestMode {
					errorChan <- errors.New("TESTING_ONLY_filter" + fmt.Sprintf("%+v", logFilter))
				}

				logs, err := rpcOptions.GetLogsByFilter(chain, logFilter)
				if err != nil {
					errorChan <- err
					if errors.Is(err, ethereum.NotFound) {
						continue
					}
					cancel()
					return
				}

				for _, log := range logs {
					// Note: This is needed because of a GoLang bug when taking the pointer of a loop variable
					log := log
					if !opts.shouldShow(&id, &log) {
						continue
					}
					modelChan <- &log
				}
			}
		}
	}

	extra := map[string]interface{}{
		"logs":      opts.Logs,
		"traces":    opts.Traces,
		"addresses": opts.Uniq,
	}
	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOptsWithExtra(extra))
}

func (opts *TransactionsOptions) shouldShow(app *types.RawAppearance, log *types.SimpleLog) bool {
	if log.BlockNumber != uint64(app.BlockNumber) || log.TransactionIndex != uint64(app.TransactionIndex) {
		return false
	}

	if len(opts.Emitter) == 0 {
		return true
	}

	for _, e := range opts.Emitter {
		if e == log.Address.Hex() {
			return true
		}
	}
	return false
}
