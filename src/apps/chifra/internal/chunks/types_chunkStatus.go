package chunksPkg

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config/scrapeCfg"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// TODO: BOGUS2 - MUST DOCUMENT
type simpleChunkStatus struct {
	Progress index.FolderProgress
	Config   scrapeCfg.ScrapeSettings
}

func (s *simpleChunkStatus) Raw() *types.RawModeler {
	return nil
}

func (s *simpleChunkStatus) Model(showHidden bool, format string, extraOptions map[string]any) types.Model {
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
