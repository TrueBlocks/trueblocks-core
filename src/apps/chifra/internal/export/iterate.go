package exportPkg

import (
	"context"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/monitor"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

func (opts *ExportOptions) readTransactions(mon *monitor.Monitor, theMap map[types.SimpleAppearance]*types.SimpleTransaction, readTraces bool) error {
	chain := opts.Globals.Chain
	showProgress := !opts.Globals.TestMode
	var bar = logger.NewBar(mon.Address.Hex(), showProgress, mon.Count())

	// This is called concurrently, once for each appearance
	iterFunc := func(app types.SimpleAppearance, value *types.SimpleTransaction) error {
		raw := types.RawAppearance{
			Address:          app.Address.Hex(),
			BlockNumber:      uint32(app.BlockNumber),
			TransactionIndex: uint32(app.TransactionIndex),
		}
		if tx, err := rpcClient.GetTransactionByAppearance(chain, &raw, readTraces); err != nil {
			return err
		} else {
			matchesFourByte := len(opts.Fourbytes) == 0 // either there is no four bytes...
			for _, fb := range opts.Fourbytes {
				if strings.HasPrefix(tx.Input, fb) {
					matchesFourByte = true // ... or the four bytes match
				}
			}
			if matchesFourByte {
				*value = *tx
			}
			bar.Tick()
			return nil
		}
	}

	// Set up and interate over the map calling iterFunc for each appearance
	errChan := make(chan error)
	ctx := context.Background()
	utils.IterateOverMap(ctx, errChan, theMap, iterFunc)
	if stepErr := <-errChan; stepErr != nil {
		return stepErr
	} else {
		bar.Finish(true)
	}
	return nil
}
