package transactionsPkg

import (
	"context"
	"errors"
	"sort"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/filter"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/identifiers"
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

	filter := filter.NewFilter(
		false,
		false,
		[]string{},
		base.BlockRange{First: 0, Last: utils.NOPOS},
		base.RecordRange{First: 0, Last: utils.NOPOS},
	)

	ledgers := ledger.NewLedger(
		opts.Conn,
		base.HexToAddress(opts.AccountFor),
		0,
		utils.NOPOS,
		ether,
		testMode,
		false, // opts.Globals.NoZero
		opts.Traces,
		false, /* reversed */
		nil,
	)
	if err = contextsFromIds(ledgers, chain, opts.TransactionIds); err != nil {
		return err
	}

	ctx, cancel := context.WithCancel(context.Background())
	fetchData := func(modelChan chan types.Modeler[types.RawStatement], errorChan chan error) {
		for _, rng := range opts.TransactionIds {
			txIds, err := rng.ResolveTxs(chain)
			if err != nil && !errors.Is(err, ethereum.NotFound) {
				errorChan <- err
				cancel()
			}

			for _, raw := range txIds {
				app := types.SimpleAppearance{
					BlockNumber:      raw.BlockNumber,
					TransactionIndex: raw.TransactionIndex,
				}

				if tx, err := opts.Conn.GetTransactionByAppearance(&app, false); err != nil {
					errorChan <- err
					cancel()

				} else {
					if statements, err := ledgers.GetStatements(opts.Conn, filter, tx); err != nil {
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
	}

	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOpts())
}

func contextsFromIds(l *ledger.Ledger, chain string, txIds []identifiers.Identifier) error {
	apps := make([]types.SimpleAppearance, 0, 200)
	for _, rng := range txIds {
		rawApps, err := rng.ResolveTxs(chain)
		if err != nil && !errors.Is(err, ethereum.NotFound) {
			return err
		}
		for _, app := range rawApps {
			apps = append(apps, types.SimpleAppearance{
				BlockNumber:      app.BlockNumber,
				TransactionIndex: app.TransactionIndex,
			})
		}
	}

	sort.Slice(apps, func(i, j int) bool {
		if apps[i].BlockNumber == apps[j].BlockNumber {
			return apps[i].TransactionIndex < apps[j].TransactionIndex
		}
		return apps[i].BlockNumber < apps[j].BlockNumber
	})

	return l.SetContexts(chain, apps)
}
