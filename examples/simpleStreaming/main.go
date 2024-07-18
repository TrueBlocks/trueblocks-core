package main

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/sdk"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// main is a purposefully simple example that uses the TrueBlocks SDK Streaming functions.
// It streams the results based on the options, and prints each block's number and date
// as it is aquired. Note that this example differs from the simple example in that no
// array of data items is created. When dealing with very large smart contracts, streaming
// is greatly preferred. See the simple example for a comparison. This version is
// nearly twice as fast as the simple example.
func main() {
	// Identical to normal usage of the SDK, but with a streaming context
	opts := sdk.WhenOptions{
		BlockIds:  []string{"0-19000000:monthly"},
		RenderCtx: output.NewStreamingContext(),
		Globals: sdk.Globals{
			Cache: true,
		},
	}

	// In order to "hear" the streaming data, we need to start a goroutine that listens
	// for the data and prints it to the console. This is a simple example, but in a real
	// application, you would likely want to do something more useful with the data such
	// as writing it to a file or a database.
	go func() {
		for {
			select {
			case model := <-opts.RenderCtx.ModelChan:
				// We know the type because sdk.When returns a slice of types.NamedBlock
				if block, ok := model.(*types.NamedBlock); ok {
					fmt.Printf("%d\t%s\n", block.BlockNumber, block.Date())
				} else {
					fmt.Println("Should never happen - wrong type")
				}
			case err := <-opts.RenderCtx.ErrorChan:
				logger.Error(err)
				return
			}
		}
	}()

	// Query TrueBlocks core for the desired blocks (monthly blocks between 0 and 19000000).
	// Note that nothing is returned here since all the data is streamed to the RenderCtx.
	_, _, err := opts.When()
	if err != nil {
		fmt.Println(err)
		return
	}

	// We're done. The channels will close naturally.
}
