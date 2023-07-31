package main

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

var bar = logger.NewBar("Visiting", true, int64(17000000))

func main() {
	latest := rpcClient.GetLatestBlockNumber("mainnet")
	forEveryTrace(46000, latest, visitTrace)
	bar.Finish(true)
}

func visitTrace(trace *types.SimpleTrace, data *any) error {
	if trace.Result != nil && !trace.Result.Address.IsZero() {
		fmt.Println(trace.Result.Address, trace.Result.GasUsed)
	}
	return nil
}

func forEveryTrace(from, to base.Blknum, visitor func(*types.SimpleTrace, *any) error) error {
	for blknum := from; blknum <= to; blknum++ {
		if block, err := rpcClient.GetBlockHeaderByNumber("mainnet", blknum, rpcClient.NoOptions); err != nil {
			return err
		} else {
			bar.Tick()
			for _, txHash := range block.Transactions {
				if traces, err := rpcClient.GetTracesByTransactionHash("mainnet", txHash, nil, rpcClient.NoOptions); err != nil {
					return err
				} else {
					for _, trace := range traces {
						if err := visitor(&trace, nil); err != nil {
							return err
						}
					}
				}
			}
		}
	}
	return nil
}
