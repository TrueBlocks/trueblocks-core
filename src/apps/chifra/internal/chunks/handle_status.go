package chunksPkg

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config/scrape"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
)

type SimpleChunkStatus struct {
	Progress rpcClient.MetaData
	Config   scrape.ScrapeSettings
}

func (opts *ChunksOptions) HandleStatus(blockNums []uint64) error {
	meta, _ := rpcClient.GetMetaData(opts.Globals.Chain, opts.Globals.TestMode)
	settings, _ := scrape.GetSettings(opts.Globals.Chain, nil)
	m := SimpleChunkStatus{
		Progress: *meta,
		Config:   settings,
	}
	if opts.Globals.TestMode {
		m.Progress = rpcClient.MetaData{
			ChainId:   meta.ChainId,
			NetworkId: meta.NetworkId,
			Chain:     meta.Chain,
		}
	}
	defer opts.Globals.RenderFooter()
	err := opts.Globals.RenderHeader(rpcClient.MetaData{}, &opts.Globals.Writer, opts.Globals.Format, opts.Globals.ApiMode, opts.Globals.NoHeader, true)
	if err != nil {
		return err
	}

	return opts.Globals.RenderObject(m, true)
}
