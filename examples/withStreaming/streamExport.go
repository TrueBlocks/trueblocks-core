package main

import (
	"fmt"
	"os"
	"time"

	"github.com/TrueBlocks/trueblocks-core/sdk"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func TestStreamExport() {
	opts := sdk.ExportOptions{
		Addrs:     []string{"0x1f98431c8ad98523631ae4a59f267346ea31f984"}, // Uniswap V3 Factory
		Unripe:    true,
		RenderCtx: output.WithChannels(),
	}
	opts.Globals.Cache = true

	go func() {
		time.Sleep(3 * time.Second)
		opts.RenderCtx.Cancel()
	}()

	if len(os.Args) > 1 {
		opts.RenderCtx = nil
		txs, _, err := opts.Export()
		if err != nil {
			fmt.Println(err)
			return
		}

		for _, tx := range txs {
			fmt.Printf("%d\t%d\n", tx.BlockNumber, tx.TransactionIndex)
		}
	} else {
		go func() {
			for {
				select {
				case model := <-opts.RenderCtx.ModelChan:
					if tx, ok := model.(*types.Transaction); !ok {
						fmt.Println("Not a transaction")
					} else {
						fmt.Printf("%d\t%d\n", tx.BlockNumber, tx.TransactionIndex)
					}
				case err := <-opts.RenderCtx.ErrorChan:
					fmt.Println("Error returned by fetchData:", err)
				}
			}
		}()
		_, _, err := opts.Export()
		if err != nil {
			fmt.Println(err)
			return
		}
	}
}
