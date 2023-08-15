package transactionsPkg

import (
	"context"
	"errors"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/ledger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/ethereum/go-ethereum"
)

func (opts *TransactionsOptions) HandleAccounting() (err error) {
	chain := opts.Globals.Chain
	testMode := opts.Globals.TestMode
	ether := opts.Globals.Ether
	noZero := false // opts.Globals.NoZero

	ledgers := ledger.NewLedger(
		opts.Conn,
		base.HexToAddress(opts.AccountFor),
		0,
		utils.NOPOS,
		ether,
		testMode,
		noZero,
		opts.Traces,
		nil,
	)
	_ = ledgers.SetContextsFromIds(chain, opts.TransactionIds)

	ctx, cancel := context.WithCancel(context.Background())
	fetchData := func(modelChan chan types.Modeler[types.RawStatement], errorChan chan error) {
		for _, rng := range opts.TransactionIds {
			txIds, err := rng.ResolveTxs(chain)
			if err != nil && !errors.Is(err, ethereum.NotFound) {
				errorChan <- err
				cancel()
			}

			for _, app := range txIds {
				if statements, err := ledgers.GetStatementsFromAppearance(opts.Conn, &app); err != nil {
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
