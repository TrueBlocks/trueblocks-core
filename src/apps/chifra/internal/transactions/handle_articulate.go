package transactionsPkg

import (
	"context"
	"errors"
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/abi"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/articulate"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/ethereum/go-ethereum"
)

func (opts *TransactionsOptions) HandleArticulate() (err error) {
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
				tx, err := rpcClient.GetTransactionByAppearance(chain, &appearance)
				if err != nil {
					errorChan <- err
					continue
				}
				if tx == nil {
					logger.Log(logger.Info, "transaction not found:", appearance.BlockNumber, appearance.TransactionIndex)
					continue
				}
				if err = abi.LoadAbi(chain, tx.To, abiMap); err != nil {
					errorChan <- err
				}

				for index, log := range tx.Receipt.Logs {
					if err = abi.LoadAbi(chain, log.Address, abiMap); err != nil {
						errorChan <- err
						continue
					}

					tx.Receipt.Logs[index].ArticulatedLog, err = articulate.ArticulateLog(&log, abiMap)
					if err != nil {
						errorChan <- err
						continue
					}
				}

				for index, trace := range tx.Traces {
					if err = abi.LoadAbi(chain, trace.Action.To, abiMap); err != nil {
						errorChan <- err
						continue
					}

					tx.Traces[index].ArticulatedTrace, err = articulate.ArticulateTrace(&trace, abiMap)
					if err != nil {
						errorChan <- err
						continue
					}
				}

				if len(tx.Input) >= 10 {
					selector := tx.Input[:10]
					inputData := tx.Input[10:]
					found := abiMap[selector]
					if found != nil {
						tx.ArticulatedTx = found
						var outputData string

						if len(tx.Traces) > 0 && tx.Traces[0].Result != nil && len(tx.Traces[0].Result.Output) > 2 {
							outputData = tx.Traces[0].Result.Output[2:]
						}
						if err = articulate.ArticulateFunction(tx.ArticulatedTx, inputData, outputData); err != nil {
							errorChan <- err
						}
					} else {
						errorChan <- fmt.Errorf("method/event not found: %s", selector)
					}
					if inputData != "" {
						message, ok := articulate.ArticulateString(inputData)
						if ok {
							tx.Message = message
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
			"articulate":               true,
			"traces":                   opts.Traces,
			"tracesTransactionsFormat": true,
		},
	})
}
