package statusPkg

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (opts *StatusOptions) HandleDiagnose(rCtx *output.RenderCtx) error {
	testMode := opts.Globals.TestMode

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

	return output.StreamMany(rCtx.Ctx, fetchData, opts.Globals.OutputOpts())
}
