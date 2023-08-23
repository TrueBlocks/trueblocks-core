package main

import (
	"context"
	"fmt"
	"time"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/identifiers"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

// -----------------------------------------------------------------------------------------------
// The following code shows an example of using one of TrueBlocks' forEvery features. forEvery
// does as it sounds. It visits every item in a given data structure performing a particular
// function. Here, we're showing an example of showing every address appearance in a block.
// We're searching for the first transaction in which the infamous The DAO (0xbb9...)
// appears. There are many forEvery functions such as forEveryTransaction,
// forEveryTrace, forEveryLog, etc.
// -----------------------------------------------------------------------------------------------
func main() {
	chain := "mainnet"
	conn := rpc.TempConnection(chain)
	slowWay(conn)
	fastWay(conn)
}

var chain = "mainnet"

func slowWay(conn *rpc.Connection) {
	start := time.Now()
	bar := logger.NewBar(logger.BarOptions{
		Prefix:  "Getting stuff",
		Enabled: true,
		Start:   40000,
		Total:   60000,
	})
	for i := 40000; i < 60000; i++ {
		if block, err := conn.GetBlockHeaderByNumber(base.Blknum(i)); err != nil {
			fmt.Println(err)
		} else {
			if len(block.Transactions) > 0 {
				bar.Finish(true)
				done := time.Since(start)
				fmt.Println("Time taken:", done, "seconds")
				fmt.Println(i, block.Hash.String(), "in", done, "seconds")
				break
			} else {
				bar.Tick()
			}
		}
	}
}

func fastWay(conn *rpc.Connection) {
	bar := logger.NewBar(logger.BarOptions{
		Type:    logger.Fixed,
		Prefix:  "Getting stuff",
		Enabled: true,
		Start:   40000,
		Total:   60000,
	})

	var BlockIds []identifiers.Identifier
	if err := validate.ValidateIdentifiers(chain, []string{"40000-60000"}, validate.ValidBlockIdWithRange, 1, &BlockIds); err != nil {
		fmt.Println(err)
	}

	var theMap map[identifiers.ResolvedId]*types.SimpleBlock[string]
	var err error

	if theMap, _, err = identifiers.AsMap[types.SimpleBlock[string]](chain, BlockIds); err != nil {
		fmt.Println(err)
	} else {
		ctx, cancel := context.WithCancel(context.Background())
		defer cancel()

		var firstBlock types.SimpleBlock[string]
		firstBlock.BlockNumber = utils.NOPOS
		iterateFunc := func(key identifiers.ResolvedId, value *types.SimpleBlock[string]) error {
			if theBlock, err := conn.GetBlockHeaderByNumber(base.Blknum(key.BlockNumber)); err != nil {
				return err
			} else {
				if len(theBlock.Transactions) > 0 {
					// fmt.Println("Found", theBlock.Hash.String(), ":", len(theBlock.Transactions), "transactions")
					if theBlock.BlockNumber > 0 && key.BlockNumber < firstBlock.BlockNumber {
						firstBlock = theBlock
						fmt.Println("    Set", firstBlock.BlockNumber, firstBlock.Hash.String(), ":", len(firstBlock.Transactions), "transactions             ")
					}
				}
				bar.Tick()
				return nil
			}
		}

		errorChan := make(chan error)
		go utils.IterateOverMap(ctx, errorChan, theMap, iterateFunc)
		for err := range errorChan {
			fmt.Println(err)
		}

		timeTaken := bar.Finish(true)
		fmt.Println(firstBlock.BlockNumber, firstBlock.Hash.String(), "in", timeTaken, "seconds")
	}
}
