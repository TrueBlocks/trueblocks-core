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
		Addrs:     []string{"0x1f98431c8ad98523631ae4a59f267346ea31f984"}, // Uniswap V3 Factory
		RenderCtx: sdk.NewRenderContext(),
	}

	go func() {
		cnt := 0
		for i := 0; i < cancelAfter; i++ {
			fmt.Printf("Sleeping for %d seconds...\n", cancelAfter-cnt)
			time.Sleep(1 * time.Second)
			cnt++
		}
		fmt.Println("Canceling context...")
		opts.RenderCtx.Cancel()
		fmt.Println("Rendering txs...")
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
