// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package blocksPkg

import (
	"context"
	"errors"
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/articulate"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/ethereum/go-ethereum"
)

func (opts *BlocksOptions) HandleLogs() error {
	chain := opts.Globals.Chain
	abiCache := articulate.NewAbiCache(chain, opts.Articulate)

	// TODO: Why does this have to dirty the caller?
	settings := rpcClient.DefaultRpcOptionsSettings{
		Chain: chain,
		Opts:  opts,
	}
	opts.Conn = settings.DefaultRpcOptions()

	ctx, cancel := context.WithCancel(context.Background())
	fetchData := func(modelChan chan types.Modeler[types.RawLog], errorChan chan error) {
		for _, br := range opts.BlockIds {
			blockNums, err := br.ResolveBlocks(chain)
			if err != nil {
				errorChan <- err
				if errors.Is(err, ethereum.NotFound) {
					continue
				}
				cancel()
				return
			}

			for _, bn := range blockNums {
				emitters := []base.Address{}
				for _, e := range opts.Emitter {
					emitters = append(emitters, base.HexToAddress(e))
				}
				topics := []base.Hash{}
				for _, t := range opts.Topic {
					topics = append(topics, base.HexToHash(t))
				}
				logFilter := types.SimpleLogFilter{
					FromBlock: bn,
					ToBlock:   bn,
					Emitters:  emitters,
					Topics:    topics,
				}

				if opts.Globals.TestMode {
					errorChan <- errors.New("TESTING_ONLY_filter" + fmt.Sprintf("%+v", logFilter))
				}

				logs, err := opts.Conn.GetLogsByFilter(logFilter)
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
					if opts.Articulate {
						if err := abiCache.ArticulateLog(chain, &log); err != nil {
							errorChan <- err // continue even on error
						}
					}
					if !opts.shouldShow(&log) {
						continue
					}
					modelChan <- &log
				}
			}
		}
	}

	extra := map[string]interface{}{
		"count":      opts.Count,
		"uncles":     opts.Uncles,
		"logs":       opts.Logs,
		"traces":     opts.Traces,
		"addresses":  opts.Uniq,
		"articulate": opts.Articulate,
	}
	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOptsWithExtra(extra))
}

func (opts *BlocksOptions) shouldShow(log *types.SimpleLog) bool {
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
