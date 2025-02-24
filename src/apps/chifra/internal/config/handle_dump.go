package configPkg

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// HandleDump dumps a config to the screen
func (opts *ConfigOptions) HandleDump(rCtx *output.RenderCtx) error {
	config.ReloadConfig()
	fetchData := func(modelChan chan types.Modeler, errorChan chan error) {
		_ = errorChan
		config := *config.GetRootConfig()
		s := types.Config{
			Version:   config.Version,
			Settings:  config.Settings,
			Keys:      config.Keys,
			Pinning:   config.Pinning,
			Unchained: config.Unchained,
			Chains:    config.Chains,
		}
		modelChan <- &s
	}

	opts.Globals.NoHeader = true
	return output.StreamMany(rCtx, fetchData, opts.Globals.OutputOpts())
}
