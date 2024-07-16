package main

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/sdk"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func TestStreamNames() {
	opts := sdk.NamesOptions{
		RenderCtx: output.WithChannels(),
	}

	go func() {
		for {
			select {
			case name := <-opts.RenderCtx.ModelChan:
				if n, ok := name.(*types.Name); !ok {
					fmt.Println("Not a name")
				} else {
					fmt.Printf("%s,%s,%s\n", n.Tags, n.Address, n.Name)
				}
			case err := <-opts.RenderCtx.ErrorChan:
				fmt.Println("Error returned by fetchData:", err)
			case i := <-opts.RenderCtx.ProgressChan:
				fmt.Println(i)
				return
			}
		}
	}()

	if _, _, err := opts.Names(); err != nil {
		fmt.Println(err)
		// } else {
		// 	for _, name := range names {
		// 		fmt.Printf("%s,%s,%s\n", name.Address, name.Name, name.Symbol)
		// 	}
	}
}
