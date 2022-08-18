package chunksPkg

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config/scrape"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
)

type SimpleChunkReport struct {
	Progress index.SimpleProgress
	Config   scrape.ScrapeSettings
}

func (opts *ChunksOptions) HandleStatus(blockNums []uint64) error {
	settings, _ := scrape.GetSettings(opts.Globals.Chain, nil)
	// Let's not expose keys
	settings.Pinata_api_key = ""
	settings.Pinata_secret_api_key = ""
	settings.Pinata_jwt = ""
	settings.Estuary_key = ""
	m := SimpleChunkReport{
		Config: settings,
	}
	if opts.Globals.TestMode {
		m.Progress = index.SimpleProgress{
			Chain: opts.Globals.Chain,
		}
	} else {
		m.Progress, _ = index.GetProgress(opts.Globals.Chain)
	}

	defer opts.Globals.RenderFooter()
	err := opts.Globals.RenderHeader(rpcClient.MetaData{}, &opts.Globals.Writer, opts.Globals.Format, opts.Globals.ApiMode, opts.Globals.NoHeader, true)
	if err != nil {
		return err
	}

	return opts.Globals.RenderObject(m, true)
}
