package statusPkg

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/types"
)

func (opts *StatusOptions) HandleDiagnose(rCtx *output.RenderCtx) error {
	testMode := opts.Globals.TestMode
	chain := opts.Globals.Chain

	// If chains or caches flags are set, we want to handle them like regular
	// status commands but with the diagnose info included
	if opts.Chains || opts.Caches {
		return opts.HandleChainsAndCaches(rCtx, false)
	}

	// For --diagnose without --chains or --caches, behave like HandleShow
	// but with diagnose=true to get additional diagnostic information
	fetchData := func(modelChan chan types.Modeler, errorChan chan error) {
		s, err := opts.GetStatus(opts.Diagnose)
		if err != nil {
			errorChan <- err
			return
		}

		// We want to short circuit the output in the non-json case
		if toTemplate(s, opts.Globals.Writer, testMode, opts.Diagnose, logger.LogTimerOn(), opts.Globals.Format) {
			return
		}

		modelChan <- s
	}

	extraOpts := map[string]any{
		"chain":    chain,
		"testMode": testMode,
		"chains":   opts.Chains,
		"caches":   opts.Caches,
	}

	return output.StreamMany(rCtx, fetchData, opts.Globals.OutputOptsWithExtra(extraOpts))
}
