package chunksPkg

import (
	"context"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config/scrapeCfg"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (opts *ChunksOptions) HandleStatus(blockNums []uint64) error {
	ctx := context.Background()
	fetchData := func(modelChan chan types.Modeler[RawChunkStatus], errorChan chan error) {
		settings, _ := scrapeCfg.GetSettings(opts.Globals.Chain, "blockScrape.toml", nil)
		s := SimpleChunkStatus{
			Config: settings,
		}
		if opts.Globals.TestMode {
			s.Progress = index.SimpleProgress{
				Chain: opts.Globals.Chain,
			}
		} else {
			s.Progress, _ = index.GetProgress(opts.Globals.Chain)
		}

		modelChan <- &s
	}

	return output.StreamMany(ctx, fetchData, output.OutputOptions{
		Writer:     opts.Globals.Writer,
		Chain:      opts.Globals.Chain,
		TestMode:   opts.Globals.TestMode,
		NoHeader:   opts.Globals.NoHeader,
		ShowRaw:    opts.Globals.ShowRaw,
		Verbose:    opts.Globals.Verbose,
		LogLevel:   opts.Globals.LogLevel,
		Format:     opts.Globals.Format,
		OutputFn:   opts.Globals.OutputFn,
		Append:     opts.Globals.Append,
		JsonIndent: "  ",
	})
}

type RawChunkStatus interface {
}

type SimpleChunkStatus struct {
	Progress index.SimpleProgress
	Config   scrapeCfg.ScrapeSettings
}

func (s *SimpleChunkStatus) Raw() *RawChunkStatus {
	return nil
}

func (s *SimpleChunkStatus) Model(showHidden bool, format string, extraOptions map[string]any) types.Model {
	model := map[string]any{
		"config":   s.Config,
		"progress": s.Progress,
	}
	order := []string{
		"config",
		"progress",
	}
	return types.Model{
		Data:  model,
		Order: order,
	}
}
