package transactionsPkg

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

func (opts *TransactionsOptions) HandleShowTxs() (err error) {
	abiMap := make(abi.AbiInterfaceMap)
	loadedMap := make(map[base.Address]bool)
	skipMap := make(map[base.Address]bool)
	chain := opts.Globals.Chain
	cache := opts.Globals.CacheStore(false)

	ctx, cancel := context.WithCancel(context.Background())
	fetchData := func(modelChan chan types.Modeler[types.RawTransaction], errorChan chan error) {
		for _, rng := range opts.TransactionIds {
			txIds, err := rng.ResolveTxs(chain)
			if err != nil && !errors.Is(err, ethereum.NotFound) {
				errorChan <- err
				cancel()
			}

			for _, appearance := range txIds {
				tx, err := rpcClient.GetTransactionByAppearance(chain, &appearance, opts.Traces, cache)
				if err != nil {
					errorChan <- fmt.Errorf("transaction at %s returned an error: %w", strings.Replace(rng.Orig, "-", ".", -1), err)
					continue
				}
				if tx == nil {
					errorChan <- fmt.Errorf("transaction at %s not found", strings.Replace(rng.Orig, "-", ".", -1))
					continue
				}

				if opts.Articulate && tx.ArticulatedTx == nil {
					address := tx.To
					if !loadedMap[address] && !skipMap[address] {
						if err := abi.LoadAbi(chain, address, abiMap); err != nil {
							skipMap[address] = true
							errorChan <- err // continue even with an error
						} else {
							loadedMap[address] = true
						}
					}

					for index, log := range tx.Receipt.Logs {
						log := log
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
							if tx.Receipt.Logs[index].ArticulatedLog, err = articulate.ArticulateLog(&log, abiMap); err != nil {
								errorChan <- err // continue even with an error
							}
						}
					}

					for index, trace := range tx.Traces {
						trace := trace
						address := trace.Action.To
						if !loadedMap[address] && !skipMap[address] {
							if err := abi.LoadAbi(chain, address, abiMap); err != nil {
								skipMap[address] = true
								errorChan <- err // continue even with an error
							} else {
								loadedMap[address] = true
							}
						}
						if !skipMap[address] {
							if tx.Traces[index].ArticulatedTrace, err = articulate.ArticulateTrace(&trace, abiMap); err != nil {
								errorChan <- err // continue even with an error
							}
						}
					}

					var found *types.SimpleFunction
					var selector string
					if len(tx.Input) >= 10 {
						selector = tx.Input[:10]
						inputData := tx.Input[10:]
						found = abiMap[selector]
						if found != nil {
							tx.ArticulatedTx = found.Clone()
							var outputData string
							if len(tx.Traces) > 0 && tx.Traces[0].Result != nil && len(tx.Traces[0].Result.Output) > 2 {
								outputData = tx.Traces[0].Result.Output[2:]
							}
							if err = articulate.ArticulateFunction(tx.ArticulatedTx, inputData, outputData); err != nil {
								errorChan <- err // continue even with an error
							}
						}
					}

					if found == nil && len(tx.Input) > 0 {
						if message, ok := articulate.ArticulateString(tx.Input); ok {
							tx.Message = message
							// } else if len(selector) > 0 {
							// 	// don't report this error
							// 	errorChan <- fmt.Errorf("method/event not found: %s", selector)
						}
					}
				}

				modelChan <- tx
			}
		}
	}

	extra := map[string]interface{}{
		"articulate": opts.Articulate,
		"traces":     opts.Traces,
	}
	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOptsWithExtra(extra))
}
