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
				bar.Finish(true /* newLine */)
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
	var BlockIds []identifiers.Identifier
	if err := validate.ValidateIdentifiers(chain, []string{"40000-60000"}, validate.ValidBlockIdWithRange, 1, &BlockIds); err != nil {
		fmt.Println(err)
	}

	apps, _, err := identifiers.IdsToApps(chain, BlockIds)
	if err != nil {
		fmt.Println(err)
	}

	if sliceOfMaps, cnt, err := types.AsSliceOfMaps[types.Block[string]](apps, false); err != nil {
		fmt.Println(err)

	} else if cnt != 0 {
		fmt.Println("Expected a block")

	} else {
		bar := logger.NewBar(logger.BarOptions{
			Type:    logger.Fixed,
			Prefix:  "Getting stuff",
			Enabled: true,
			Start:   40000,
			Total:   60000,
		})

		for _, thisMap := range sliceOfMaps {
			for app := range thisMap {
				thisMap[app] = new(types.Block[string])
			}

			var firstBlock types.Block[string]
			firstBlock.BlockNumber = utils.NOPOS
			iterateFunc := func(key types.Appearance, value *types.Block[string]) error {
				bn := uint64(key.BlockNumber)
				if theBlock, err := conn.GetBlockHeaderByNumber(base.Blknum(key.BlockNumber)); err != nil {
					return err
				} else {
					if len(theBlock.Transactions) > 0 {
						// fmt.Println("Found", theBlock.Hash.String(), ":", len(theBlock.Transactions), "transactions")
						if theBlock.BlockNumber > 0 && bn < firstBlock.BlockNumber {
							firstBlock = theBlock
							fmt.Println("    Set", firstBlock.BlockNumber, firstBlock.Hash.String(), ":", len(firstBlock.Transactions), "transactions             ")
						}
					}
					bar.Tick()
					return nil
				}
			}

			errorChan := make(chan error)
			ctx, cancel := context.WithCancel(context.Background())
			defer cancel()
			go utils.IterateOverMap(ctx, errorChan, thisMap, iterateFunc)
			for err := range errorChan {
				fmt.Println(err)
			}

			timeTaken := bar.Finish(true /* newLine */)
			fmt.Println(firstBlock.BlockNumber, firstBlock.Hash.String(), "in", timeTaken, "seconds")
		}
		bar.Finish(true /* newLine */)
	}
}
