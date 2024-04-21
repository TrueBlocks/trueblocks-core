package main

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/sdk"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

func main() {
	// logger.ToggleProgress(true)
	// meta, err := sdk.MetaData()
	// if err != nil {
	// 	logger.Fatal("Error getting metadata", "err", err)
	// }
	start := 105233000 // 0
	stop := 105236000  // int(meta.Latest)
	step := 1          // int(meta.Latest)/1000000
	// blockIds := make([]string, 0, ((stop-start)/step)+1)
	// for i := start; i < stop; i += step {
	// 	blockIds = append(blockIds, fmt.Sprintf("%d", i))
	// }
	blockIds := []string{fmt.Sprintf("%d-%d:%d", start, stop, step)}
	logger.Info("Getting blocks", blockIds)

	lastTs := base.Timestamp(0)

	whenOpts := sdk.WhenOptions{
		BlockIds: blockIds,
		Globals:  sdk.Globals{Cache: true},
	}
	blocks, _, err := whenOpts.When()
	if err != nil {
		logger.Fatal("Error querying blocks with WhenOptions", "err", err)
	}

	fmt.Println("bn,timestamp,diff,secs")
	for _, block := range blocks {
		diff := block.Timestamp - lastTs
		secsPerBlock := float64(diff) / float64(step)
		fmt.Printf("%d,%d,%d,%f\n", block.BlockNumber, block.Timestamp, diff, secsPerBlock)
		lastTs = block.Timestamp
	}
}
