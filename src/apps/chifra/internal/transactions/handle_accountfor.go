package transactionsPkg

import (
	"context"
	"errors"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/transactions/ledger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/ethereum/go-ethereum"
)

func (opts *TransactionsOptions) HandleAccounting() (err error) {
	chain := opts.Globals.Chain
	testMode := opts.Globals.TestMode
	ether := opts.Globals.Ether
	acctFor := base.HexToAddress(opts.AccountFor)
	noZero := false // opts.Globals.NoZero

	ledgers := ledger.NewLedger(
		chain,
		base.HexToAddress(opts.AccountFor),
		ether,
		testMode,
		noZero,
		opts.Traces,
	)
	ledgers.SetContexts(chain, opts.TransactionIds)

	ctx, cancel := context.WithCancel(context.Background())
	fetchData := func(modelChan chan types.Modeler[types.RawStatement], errorChan chan error) {
		for _, rng := range opts.TransactionIds {
			txIds, err := rng.ResolveTxs(chain)
			if err != nil && !errors.Is(err, ethereum.NotFound) {
				errorChan <- err
				cancel()
			}

			for _, app := range txIds {
				if statements, err := ledgers.GetStatementsFromAppearance(chain, acctFor, &app); err != nil {
					errorChan <- err
				} else {
					for _, statement := range statements {
						statement := statement
						modelChan <- &statement
					}
				}
			}
		}
	}

	extra := map[string]interface{}{
		"ether": opts.Globals.Ether,
	}
	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOptsWithExtra(extra))
}
