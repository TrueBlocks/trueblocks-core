package namesPkg

import (
	"context"
	"os"
	"path/filepath"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/names"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (opts *NamesOptions) HandleShow() error {
	if opts.anyCrud() {
		return opts.HandleCrud()
	}

	chain := opts.Globals.Chain
	var fetchData func(modelChan chan types.Modeler, errorChan chan error)

	testMode := opts.Globals.TestMode
	namesArray, err := names.LoadNamesArray(chain, opts.getType(), names.SortByAddress, opts.Terms)
	if err != nil {
		return err
	}
	if len(namesArray) == 0 {
		logger.Warn("No known names found for", opts.Terms)
		if !testMode {
			args := os.Args
			args[0] = filepath.Base(args[0])
			logger.Warn("Original command:", args)
		}
		return nil
	}

	fetchData = func(modelChan chan types.Modeler, errorChan chan error) {
		for _, name := range namesArray {
			modelChan <- &name
		}
	}

	extraOpts := map[string]any{
		"expand":  opts.Expand,
		"prefund": opts.Prefund,
	}
	if opts.Addr {
		extraOpts["single"] = "address"
		opts.Globals.NoHeader = true
	}
	ctx := context.Background()
	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOptsWithExtra(extraOpts))
}
