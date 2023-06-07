package blocksPkg

import (
	"context"
	"errors"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/ethereum/go-ethereum"
)

func (opts *BlocksOptions) HandleUniq() (err error) {
	chain := opts.Globals.Chain

	ctx, cancel := context.WithCancel(context.Background())
	fetchData := func(modelChan chan types.Modeler[types.RawAppearance], errorChan chan error) {
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
				addrMap := make(index.AddressBooleanMap)
				ts := rpc.GetBlockTimestamp(chain, bn)
				if block, err := rpcClient.GetBlockByNumberWithTxsAndTraces(chain, bn, true /* isFinal doesn't matter */); err != nil {
					errorChan <- err
				} else {
					miner := block.Miner.Hex()
					index.StreamAppearance(modelChan, opts.Flow, "miner", miner, bn, 99999, utils.NOPOS, ts, addrMap)

					if uncles, err := rpcClient.GetUnclesByNumber(chain, bn); err != nil {
						errorChan <- err
					} else {
						for _, uncle := range uncles {
							unc := uncle.Miner.Hex()
							index.StreamAppearance(modelChan, opts.Flow, "uncle", unc, bn, 99998, utils.NOPOS, ts, addrMap)
						}
					}

					for _, trans := range block.Transactions {
						if err = index.UniqFromTransDetails2(chain, modelChan, opts.Flow, &trans, ts, addrMap); err != nil {
							errorChan <- err
						}
					}

					if traces, err := rpcClient.GetTracesByBlockNumber(chain, bn); err != nil {
						errorChan <- err
					} else {
						for _, trace := range traces {
							logger.Info(trace.BlockNumber, trace.TransactionIndex, trace.TraceIndex)
						}
						if err = index.UniqFromTracesDetails(chain, modelChan, opts.Flow, traces, ts, addrMap); err != nil {
							errorChan <- err
						}
					}

					for _, trans := range block.Transactions {
						if err := index.UniqFromLogsDetails(chain, modelChan, opts.Flow, trans.Receipt.Logs, ts, addrMap); err != nil {
							errorChan <- err
						}
					}
				}
			}
		}
	}

	extra := map[string]interface{}{
		"uniq": true,
	}
	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOptsWithExtra(extra))
}

/*

//-----------------------------------------------------------------------
bool visit Prefund(const Allocation& prefund, void* data) {
    CAppearance item;
    item.blockNumber = 0;
    item.transactionIndex = ((COptions*)data)->nPrefunds++;
    item.address = prefund.address;
    item.reason = "genesis";
    oneAppearance(item, data);
    return true;
}


*/
