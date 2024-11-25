package configPkg

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// HandleDump dumps a config to the screen
func (opts *ConfigOptions) HandleDump(rCtx *output.RenderCtx) error {
	fetchData := func(modelChan chan types.Modeler, errorChan chan error) {
		var s types.Config
		s.Config = *config.GetRootConfig()
		modelChan <- &s
	}

	opts.Globals.NoHeader = true
	return output.StreamMany(rCtx, fetchData, opts.Globals.OutputOpts())
}
