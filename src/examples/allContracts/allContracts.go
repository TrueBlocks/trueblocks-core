package main

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cacheNew"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

var bar = logger.NewBar("Visiting", true, int64(17000000))

func main() {
	latest := rpcClient.BlockNumber("http://localhost:23456")
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
		if block, err := rpcClient.GetBlockByNumber("mainnet", blknum, cacheNew.NoCache); err != nil {
			return err
		} else {
			bar.Tick()
			for _, txHash := range block.Transactions {
				if traces, err := rpcClient.GetTracesByTransactionHash("mainnet", txHash, nil, cacheNew.NoCache); err != nil {
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
