package exportPkg

import (
	"context"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/filter"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

func (opts *ExportOptions) readTransactions(
	theMap map[types.SimpleAppearance]*types.SimpleTransaction,
	filt *filter.AppearanceFilter,
	bar *logger.ProgressBar,
	readTraces bool,
) error {
	iterFunc := func(app types.SimpleAppearance, value *types.SimpleTransaction) error {
		raw := types.RawAppearance{
			Address:          app.Address.Hex(),
			BlockNumber:      app.BlockNumber,
			TransactionIndex: app.TransactionIndex,
		}
		if tx, err := opts.Conn.GetTransactionByAppearance(&raw, readTraces); err != nil {
			return err
		} else {
			passes, _ := filt.ApplyTxFilters(tx)
			if passes {
				*value = *tx
			}
			if bar != nil {
				bar.Tick()
			}
			return nil
		}
	}

	// Set up and interate over the map calling iterFunc for each appearance
	ctx, cancel := context.WithCancel(context.Background())
	defer cancel()
	errChan := make(chan error)
	go utils.IterateOverMap(ctx, errChan, theMap, iterFunc)
	if stepErr := <-errChan; stepErr != nil {
		return stepErr
	} else if bar != nil {
		bar.Finish(true)
	}

	return nil
}
