package main

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

var bar = logger.NewBar(logger.BarOptions{
	Prefix:  "Visiting",
	Enabled: true,
	Total:   int64(17000000),
})

func main() {
	chain := "mainnet"
	conn := rpc.TempConnection(chain)
	latest := conn.GetLatestBlockNumber()
	forEveryTrace(46000, latest, visitTrace)
	bar.Finish(true /* newLine */)
}

func visitTrace(trace *types.Trace, data *any) error {
	if trace.Result != nil && !trace.Result.Address.IsZero() {
		fmt.Println(trace.Result.Address, trace.Result.GasUsed)
	}
	return nil
}

func forEveryTrace(from, to base.Blknum, visitor func(*types.Trace, *any) error) error {
	chain := "mainnet"
	conn := rpc.TempConnection(chain)
	for blknum := from; blknum <= to; blknum++ {
		if block, err := conn.GetBlockHeaderByNumber(blknum); err != nil {
			return err
		} else {
			bar.Tick()
			for _, txHash := range block.Transactions {
				if traces, err := conn.GetTracesByTransactionHash(txHash, nil); err != nil {
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
