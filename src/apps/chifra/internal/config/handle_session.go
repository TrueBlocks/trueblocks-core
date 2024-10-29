package configPkg

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// HandleSession returns an empty session
func (opts *ConfigOptions) HandleSession(rCtx *output.RenderCtx) error {
	fetchData := func(modelChan chan types.Modeler, errorChan chan error) {
		modelChan <- &types.Session{}
	}
	return output.StreamMany(rCtx, fetchData, opts.Globals.OutputOpts())
}
