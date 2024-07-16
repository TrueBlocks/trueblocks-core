package main

import (
	"fmt"
	"os"

	"github.com/TrueBlocks/trueblocks-core/sdk"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/schollz/progressbar/v3"
)

// TestProgressReporting is a simple example just demonstrating how to use
// the progress bar to report progress of the stream.
func TestProgressReporting() {
	nBlocks := 1000
	bar := newProgressBar(nBlocks)

	opts := sdk.BlocksOptions{
		BlockIds:  []string{fmt.Sprintf("0-%d", nBlocks)},
		RenderCtx: output.WithChannels(),
	}

	go func() {
		for {
			select {
			case block := <-opts.RenderCtx.ModelChan:
				if _, ok := block.(*types.LightBlock); !ok {
					panic("This should never happen")
				} else {
					bar.Add(1)
				}
			case err := <-opts.RenderCtx.ErrorChan:
				panic(err)
			}
		}
	}()

	// This will stream the blocks. Note that they are LightBlocks
	// which is what BlocksHashes returns.
	if _, _, err := opts.BlocksHashes(); err != nil {
		panic(err)
	}
	bar.Finish()
}

// newProgressBar creates a new progress bar with the given count
func newProgressBar(cnt int) *progressbar.ProgressBar {
	return progressbar.NewOptions(cnt,
		progressbar.OptionSetWriter(os.Stdout), //you should install "github.com/k0kubun/go-ansi"
		progressbar.OptionEnableColorCodes(true),
		progressbar.OptionSetWidth(100),
		progressbar.OptionSetDescription("[cyan][1/3][reset] Streaming names..."),
		progressbar.OptionSetTheme(progressbar.Theme{
			Saucer:        "[yellow]=[reset]",
			SaucerHead:    "[yellow]>[reset]",
			SaucerPadding: " ",
			BarStart:      "[",
			BarEnd:        "]",
		}))
}
