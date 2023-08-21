package rpc

import (
	"context"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

func (conn *Connection) ReadTransactions(
	theMap map[types.SimpleAppearance]*types.SimpleTransaction,
	fourBytes []string,
	bar *logger.ProgressBar,
	readTraces bool,
) error {
	iterFunc := func(app types.SimpleAppearance, value *types.SimpleTransaction) error {
		raw := types.RawAppearance{
			Address:          app.Address.Hex(),
			BlockNumber:      uint32(app.BlockNumber),
			TransactionIndex: uint32(app.TransactionIndex),
		}
		if tx, err := conn.GetTransactionByAppearance(&raw, readTraces); err != nil {
			return err
		} else {
			matchesFourByte := len(fourBytes) == 0 // either there is no four bytes...
			for _, fb := range fourBytes {
				if strings.HasPrefix(tx.Input, fb) {
					matchesFourByte = true // ... or the four bytes match
				}
			}
			if matchesFourByte {
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
