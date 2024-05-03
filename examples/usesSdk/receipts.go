package main

import (
	"github.com/TrueBlocks/trueblocks-core/sdk"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// DoReceipts tests the receipts sdk function
func DoReceipts() {
	opts := sdk.ReceiptsOptions{
		TransactionIds: append(firsts, []string{"17100101.1"}...),
	}
	logger.Info("DoReceipts", opts)

	// TransactionIds []string `json:"transactions,omitempty"`
	// Articulate     bool     `json:"articulate,omitempty"`
	// func (opts *ReceiptsOptions) Receipts() ([]types.Receipt, *types.MetaData, error) {

	art := []bool{false, true}

	for _, a := range art {
		baseFn := "receipts/receipts"
		if a {
			baseFn += "-articulate"
		}
		opts.Articulate = a
		globs := noEther(globals)
		for _, g := range globs {
			opts.Globals = g
			fn := getFilename(baseFn, &opts.Globals)
			TestReceipts(fn, &opts)
		}
	}
}

func TestReceipts(fn string, opts *sdk.ReceiptsOptions) {
	if receipts, _, err := opts.Receipts(); err != nil {
		ReportError(fn, err)
	} else {
		if err := SaveToFile[types.Receipt](fn, receipts); err != nil {
			ReportError(fn, err)
		} else {
			ReportOkay(fn)
		}
	}
}
