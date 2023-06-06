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
					// if (!foundOne(funcy, data, blockNumber, tr, 0, from, "from"))
					from := trans.From.Hex()
					index.StreamAppearance(modelChan, from, bn, txid, traceid, "from", ts, addrMap)

					// if (!foundOne(funcy, data, blockNumber, tr, 0, to, "to"))
					to := trans.To.Hex()
					index.StreamAppearance(modelChan, to, bn, txid, traceid, "to", ts, addrMap)

					// if (!foundOne(funcy, data, blockNumber, tr, 0, recPtr->contractAddress, "creation"))
					if !trans.Receipt.ContractAddress.IsZero() {
						contract := trans.Receipt.ContractAddress.Hex()
						index.StreamAppearance(modelChan, contract, bn, txid, traceid, "creation", ts, addrMap)
					}

					// if (!foundPot(funcy, data, blockNumber, tr, 0, extract(input, 10), "input"))
					if len(trans.Input) > 10 {
						reason := "input"
						inputData := trans.Input[10:]
						for i := 0; i < len(inputData)/64; i++ {
							str := string(inputData[i*64 : (i+1)*64])
							// fmt.Println("checking:", str)
							if index.IsImplicitAddress(str) {
								index.StreamAppearance(modelChan, str, bn, txid, traceid, reason, ts, addrMap)
							}
						}
					}

					if err = index.UniqFromLogsDetails(chain, modelChan, trans.Receipt.Logs, ts, addrMap); err != nil {
						errorChan <- err
					}

					if err = index.UniqFromTracesDetails(chain, trans.Traces, ts, modelChan, addrMap); err != nil {
						errorChan <- err
					}
				}
			}
		}
	}

	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOpts()), true
}

//----------------------------------------------------------------
// bool shouldShow(const COptions* opt, const string_q& val) {
//     switch (opt->flow[0]) {
//         case 't':  // to
//             return contains(substitute(substitute(val, "_topic", ""), "generator", ""), "to");
//         case 'f':  // from
//             return contains(val, "from");
//         case 'r':  // reward
//             return contains(val, "miner") || contains(val, "uncle");
//     }
//     return true;
// }
