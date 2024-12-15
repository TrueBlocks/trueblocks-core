package main

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	sdk "github.com/TrueBlocks/trueblocks-sdk/v3"
)

// TestStreamNames demonstrates the SDK streaming feature for names by
// creating a Rendering Context with channels.
func TestStreamNames() {
	opts := sdk.NamesOptions{
		RenderCtx: output.NewStreamingContext(),
	}

	go func() {
		// start a go routine that listens to the model and error channels
		// and prints the names as they stream in
		for {
			select {
			case name := <-opts.RenderCtx.ModelChan:
				if n, ok := name.(*types.Name); ok {
					fmt.Printf("%s\t%s\t%s\n", n.Tags, n.Address, n.Name)
				} else {
					logger.Fatal("This should never happen")
				}
			case err := <-opts.RenderCtx.ErrorChan:
				fmt.Println("Error returned by fetchData:", err)
				// os.Exit(1)
			}
		}
	}()

	// Generate the stream...
	if _, _, err := opts.Names(); err != nil {
		fmt.Println(err)
	}
}
