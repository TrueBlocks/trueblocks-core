package transactionsPkg

import (
	"context"
	"errors"

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
			appearances, err := rng.ResolveTxs(chain)
			if err != nil && !errors.Is(err, ethereum.NotFound) {
				errorChan <- err
				cancel()
			}

			for _, appearance := range appearances {
				// log.Println(">>>", appearance.Address)
				// if err = abi.LoadAbi(chain, types.HexToAddress(appearance.Address), abiMap); err != nil {
				// 	errorChan <- err
				// 	cancel()
				// }
				receipt, err := rpcClient.GetTransactionReceipt(
					chain,
					uint64(appearance.BlockNumber),
					uint64(appearance.TransactionIndex),
					nil,
					0,
				)
				if err != nil {
					errorChan <- err
					cancel()
				}

				tx, err := rpcClient.GetTransactionByAppearance(chain, &appearance)
				if err != nil {
					errorChan <- err
					cancel()
				}
				if tx == nil {
					logger.Log(logger.Info, "transaction not found:", appearance.BlockNumber, appearance.TransactionIndex)
					continue
				}
				if err = abi.LoadAbi(chain, tx.To, abiMap); err != nil {
					errorChan <- err
					cancel()
				}

				for _, log := range receipt.Logs {
					if err = abi.LoadAbi(chain, log.Address, abiMap); err != nil {
						errorChan <- err
						cancel()
					}
					log.ArticulatedLog = articulate.ArticulateLog(&log, abiMap)
				}

				for _, trace := range tx.Traces {
					if err = abi.LoadAbi(chain, trace.Action.To, abiMap); err != nil {
						errorChan <- err
						cancel()
					}
					trace.ArticulatedTrace = articulate.ArticulateTrace(&trace, abiMap)
				}

				if len(tx.Input) >= 10 {
					tx.ArticulatedTx = abiMap[tx.Input[0:10]]
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
			"articulate": true,
		},
	})
}
