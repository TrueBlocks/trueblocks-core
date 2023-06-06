package transactionsPkg

import (
	"context"
	"errors"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/ethereum/go-ethereum"
)

func (opts *TransactionsOptions) HandleUniq() (err error, disp bool) {
	chain := opts.Globals.Chain
	opts.Globals.Verbose = true

	ctx, cancel := context.WithCancel(context.Background())
	fetchData := func(modelChan chan types.Modeler[types.RawAppearance], errorChan chan error) {
		for _, rng := range opts.TransactionIds {
			txIds, err := rng.ResolveTxs(opts.Globals.Chain)
			if err != nil && !errors.Is(err, ethereum.NotFound) {
				errorChan <- err
				cancel()
			}

			for _, app := range txIds {
				bn := uint64(app.BlockNumber)
				txid := uint64(app.TransactionIndex)
				traceid := utils.NOPOS
				ts := rpc.GetBlockTimestamp(chain, bn)
				addrMap := make(index.AddressBooleanMap)
				// appMap := make(index.AppearanceMap)
				if trans, err := rpcClient.GetTransactionByAppearance(chain, &app, true); err != nil {
					errorChan <- err
				} else {
					from := trans.From.Hex()
					index.StreamAppearance(modelChan, opts.Flow, "from", from, bn, txid, traceid, ts, addrMap)

					to := trans.To.Hex()
					index.StreamAppearance(modelChan, opts.Flow, "to", to, bn, txid, traceid, ts, addrMap)

					if !trans.Receipt.ContractAddress.IsZero() {
						contract := trans.Receipt.ContractAddress.Hex()
						index.StreamAppearance(modelChan, opts.Flow, "creation", contract, bn, txid, traceid, ts, addrMap)
					}

					if len(trans.Input) > 10 {
						reason := "input"
						inputData := trans.Input[10:]
						for i := 0; i < len(inputData)/64; i++ {
							str := string(inputData[i*64 : (i+1)*64])
							// fmt.Println("checking:", str)
							if index.IsImplicitAddress(str) {
								index.StreamAppearance(modelChan, opts.Flow, str, reason, bn, txid, traceid, ts, addrMap)
							}
						}
					}

					if err = index.UniqFromLogsDetails(chain, modelChan, opts.Flow, trans.Receipt.Logs, ts, addrMap); err != nil {
						errorChan <- err
					}

					if err = index.UniqFromTracesDetails(chain, modelChan, opts.Flow, trans.Traces, ts, addrMap); err != nil {
						errorChan <- err
					}
				}
			}
		}
	}

	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOpts()), true
}
