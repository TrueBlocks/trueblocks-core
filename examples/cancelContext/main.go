package main

import (
	"fmt"
	"os"
	"time"

	"github.com/TrueBlocks/trueblocks-core/sdk"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
)

func main() {
	cancelAfter := 1
	if len(os.Args) > 1 {
		cancelAfter = int(base.MustParseInt64(os.Args[1]))
	}

	opts := sdk.ExportOptions{
		Addrs: []string{"0x1f98431c8ad98523631ae4a59f267346ea31f984"}, // Uniswap V3 Factory
	}

	go func() {
		secondsElapsed := 0
		for {
			// Spin until the cancelAfter time has elapsed
			secondsElapsed++
			fmt.Printf("Canceling in %d seconds...\n", cancelAfter-secondsElapsed+1)
			if opts.Globals.RenderCtx != nil && secondsElapsed >= cancelAfter {
				opts.Globals.RenderCtx.Cancel()
				fmt.Println("Rendering txs...")
				return
			}
			time.Sleep(1 * time.Second)
		}
	}()

	// The Uniswap V3 Factory takes a very long time
	txs, _, err := opts.Export()
	if err != nil {
		fmt.Println(err)
		return
	}

	// We have some txs, so let's print them to the console
	for _, tx := range txs {
		fmt.Printf("%d.%d\t%s\n", tx.BlockNumber, tx.TransactionIndex, tx.Date())
	}
}
