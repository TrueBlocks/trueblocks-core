package statusPkg

import (
	"context"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (opts *StatusOptions) HandleStatusTerse() error {
	testMode := opts.Globals.TestMode

	ctx := context.Background()
	fetchData := func(modelChan chan types.Modeler[types.RawModeler], errorChan chan error) {
		s, err := opts.GetSimpleStatus()
		if err != nil {
			errorChan <- err
			return
		}

		// We want to short circuit the output in the non-json case
		if s.toTemplate(opts.Globals.Writer, opts.Globals.TestMode, opts.Globals.Format) {
			return
		}

		modelChan <- s
	}

	extra := map[string]any{
		"testMode": testMode,
	}
	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOptsWithExtra(extra))
}
