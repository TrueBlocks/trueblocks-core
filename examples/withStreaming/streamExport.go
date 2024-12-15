package main

import (
	"fmt"
	"time"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	sdk "github.com/TrueBlocks/trueblocks-sdk/v3"
)

// TestStreamExport demonstrates the SDK streaming feature by creating a Rendering
// Context with both Model and Error channels. It also demonstrates canceling the
// context (and therefore the rendering) by using the Cancel feature.
func TestStreamExport() {
	opts := sdk.ExportOptions{
		Addrs:     []string{"0x1f98431c8ad98523631ae4a59f267346ea31f984"}, // Uniswap V3 Factory
		Unripe:    true,                                                   // to the head of the chain
		RenderCtx: output.NewStreamingContext(),
	}
	opts.Globals.Cache = true // Write to the cache

	go func() {
		// Sleep for 3 seconds and then cancel the rendering context
		time.Sleep(3 * time.Second)
		opts.RenderCtx.Cancel()
	}()

	go func() {
		for {
			select {
			case model := <-opts.RenderCtx.ModelChan:
				// We got a transaction. Show it. Note that the cast will always succeed
				if tx, ok := model.(*types.Transaction); ok {
					fmt.Printf("%d\t%d\n", tx.BlockNumber, tx.TransactionIndex)
				} else {
					fmt.Println("Not a transaction")
				}
			case err := <-opts.RenderCtx.ErrorChan:
				fmt.Println("Error returned by fetchData:", err)
				// os.Exit(1)
			}
		}
	}()

	// Generate the stream...
	_, _, err := opts.Export()
	if err != nil {
		fmt.Println(err)
		return
	}
}
