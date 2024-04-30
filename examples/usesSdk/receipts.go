package main

import (
	"github.com/TrueBlocks/trueblocks-core/sdk"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// DoReceipts tests the When sdk function
func DoReceipts() {
	logger.Info("DoReceipts")

	opts := sdk.ReceiptsOptions{
		TransactionIds: append(firsts, []string{"17100101.1"}...),
	}

	art := []bool{false, true}

	for _, a := range art {
		baseFn := "receipts/receipts"
		if a {
			baseFn += "-articulate"
		}
		opts.Articulate = a
		states := noRaw(noEther(globals))
		for _, g := range states {
			opts.Globals = g
			fn := getFilename(baseFn, &opts.Globals)
			TestReceipts(fn, &opts)
		}
	}
}

func TestReceipts(fn string, opts *sdk.ReceiptsOptions) {
	Report(fn) //, opts)
	if receipts, _, err := opts.Receipts(); err != nil {
		logger.Error(err)
	} else {
		if err := SaveToFile[types.Receipt](fn, receipts); err != nil {
			logger.Error(err)
		} else {
			logger.Info(colors.Green, "Ok", colors.Off)
		}
	}
	logger.Info()
}
