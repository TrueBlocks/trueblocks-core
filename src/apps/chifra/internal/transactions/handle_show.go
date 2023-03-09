package transactionsPkg

import (
	"context"
	"errors"
	"fmt"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/abi"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/articulate"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/ethereum/go-ethereum"
)

func (opts *TransactionsOptions) HandleShow() (err error) {
	abiMap := make(abi.AbiInterfaceMap)
	chain := opts.Globals.Chain

	ctx, cancel := context.WithCancel(context.Background())
	fetchData := func(modelChan chan types.Modeler[types.RawTransaction], errorChan chan error) {
		for _, rng := range opts.TransactionIds {
			txIds, err := rng.ResolveTxs(chain)
			if err != nil && !errors.Is(err, ethereum.NotFound) {
				errorChan <- err
				cancel()
			}

			for _, appearance := range txIds {
				tx, err := rpcClient.GetTransactionByAppearance(chain, &appearance, opts.Traces)
				if err != nil {
					errorChan <- fmt.Errorf("transaction at %s returned an error: %w", strings.Replace(rng.Orig, "-", ".", -1), err)
					continue
				}
				if tx == nil {
					errorChan <- fmt.Errorf("transaction at %s not found", strings.Replace(rng.Orig, "-", ".", -1))
					continue
				}

				if opts.Articulate {
					if err = abi.LoadAbi(chain, tx.To, abiMap); err != nil {
						// continue processing even with an error
						errorChan <- err
					}

					for index, log := range tx.Receipt.Logs {
						if err = abi.LoadAbi(chain, log.Address, abiMap); err != nil {
							// continue processing even with an error
							errorChan <- err
						} else {
							tx.Receipt.Logs[index].ArticulatedLog, err = articulate.ArticulateLog(&log, abiMap)
							if err != nil {
								// continue processing even with an error
								errorChan <- err
							}
						}
					}

					for index, trace := range tx.Traces {
						if err = abi.LoadAbi(chain, trace.Action.To, abiMap); err != nil {
							// continue processing even with an error
							errorChan <- err
						} else {
							tx.Traces[index].ArticulatedTrace, err = articulate.ArticulateTrace(&trace, abiMap)
							if err != nil {
								// continue processing even with an error
								errorChan <- err
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
							tx.ArticulatedTx = found
							var outputData string

							if len(tx.Traces) > 0 && tx.Traces[0].Result != nil && len(tx.Traces[0].Result.Output) > 2 {
								outputData = tx.Traces[0].Result.Output[2:]
							}
							if err = articulate.ArticulateFunction(tx.ArticulatedTx, inputData, outputData); err != nil {
								errorChan <- err
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

	return output.StreamMany(ctx, fetchData, output.OutputOptions{
		Writer:     opts.Globals.Writer,
		Chain:      opts.Globals.Chain,
		TestMode:   opts.Globals.TestMode,
		NoHeader:   opts.Globals.NoHeader,
		ShowRaw:    opts.Globals.ShowRaw,
		Verbose:    opts.Globals.Verbose,
		LogLevel:   opts.Globals.LogLevel,
		Format:     opts.Globals.Format,
		OutputFn:   opts.Globals.OutputFn,
		Append:     opts.Globals.Append,
		JsonIndent: "  ",
		Extra: map[string]interface{}{
			"articulate": opts.Articulate,
			"traces":     opts.Traces,
		},
	})
}
