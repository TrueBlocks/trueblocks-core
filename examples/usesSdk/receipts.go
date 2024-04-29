package main

import (
	"github.com/TrueBlocks/trueblocks-core/sdk"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// DoReceipts tests the When sdk function
func DoReceipts() {
	logger.Info("DoReceipts")

	opts := sdk.ReceiptsOptions{
		TransactionIds: testTransactions,
	}

	if receipts, _, err := opts.Receipts(); err != nil {
		logger.Error(err)
	} else {
		if err := SaveAndClean[types.Receipt]("usesSDK/receipts.json", receipts, &opts, func() error {
			_, _, err := opts.Receipts()
			return err
		}); err != nil {
			logger.Error(err)
		}
	}
}
