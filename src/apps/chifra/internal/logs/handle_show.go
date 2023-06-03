package logsPkg

import (
	"context"
	"errors"
	"fmt"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/abi"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/articulate"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/ethereum/go-ethereum"
)

func (opts *LogsOptions) HandleShowLogs() (err error) {
	abiMap := make(abi.AbiInterfaceMap)
	loadedMap := make(map[base.Address]bool)
	skipMap := make(map[base.Address]bool)
	chain := opts.Globals.Chain

	ctx, cancel := context.WithCancel(context.Background())
	fetchData := func(modelChan chan types.Modeler[types.RawLog], errorChan chan error) {
		for _, rng := range opts.TransactionIds {
			txIds, err := rng.ResolveTxs(chain)
			if err != nil && !errors.Is(err, ethereum.NotFound) {
				errorChan <- err
				cancel()
			}

			for _, appearance := range txIds {
				tx, err := rpcClient.GetTransactionByAppearance(chain, &appearance, false /* no traces for logs */)
				if err != nil {
					errorChan <- fmt.Errorf("transaction at %s returned an error: %w", strings.Replace(rng.Orig, "-", ".", -1), err)
					continue
				}
				if tx == nil {
					errorChan <- fmt.Errorf("transaction at %s not found", strings.Replace(rng.Orig, "-", ".", -1))
					continue
				}

				for _, log := range tx.Receipt.Logs {
					log := log
					log.Timestamp = tx.Timestamp
					if opts.Articulate {
						address := log.Address
						if !loadedMap[address] && !skipMap[address] {
							if err := abi.LoadAbi(chain, address, abiMap); err != nil {
								skipMap[address] = true
								errorChan <- err // continue even with an error
							} else {
								loadedMap[address] = true
							}
						}

						if !skipMap[address] {
							if log.ArticulatedLog, err = articulate.ArticulateLog(&log, abiMap); err != nil {
								errorChan <- err // continue even with an error
							}
						}
					}

					modelChan <- &log
				}
			}
		}
	}

	extra := map[string]interface{}{
		"articulate": opts.Articulate,
	}
	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOptsWithExtra(extra))
}
