package main

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/sdk"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
)

func TestStreamNames() {
	opts := sdk.NamesOptions{
		RenderCtx: output.WithChannels(),
	}

	go func() {
		for {
			select {
			case name := <-opts.RenderCtx.ModelChan:
				if n, ok := name.(*sdk.Name); !ok {
					logger.Fatal("This should never happen")
				} else {
					fmt.Printf("%s\t%s\t%s\n", n.Tags, n.Address, n.Name)
				}
			case err := <-opts.RenderCtx.ErrorChan:
				fmt.Println("Error returned by fetchData:", err)
			}
		}
	}()

	if _, _, err := opts.Names(); err != nil {
		fmt.Println(err)
	}
}
