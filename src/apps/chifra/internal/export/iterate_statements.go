package exportPkg

import (
	"fmt"
	"log"
	"sort"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/articulate"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/ledger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/monitor"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (opts *ExportOptions) readStatements(
	monitorArray []monitor.Monitor,
	mon *monitor.Monitor,
	filter *monitor.AppearanceFilter,
	errorChan chan error,
	abiCache *articulate.AbiCache,
) ([]*types.SimpleStatement, error) {

	var cnt int
	var err error
	var txMap map[types.SimpleAppearance]*types.SimpleTransaction

	if txMap, cnt, err = monitor.ReadAppearancesToMap[types.SimpleTransaction](mon, filter); err != nil {
		errorChan <- err
		return nil, err
	}

	if opts.NoZero && cnt == 0 {
		errorChan <- fmt.Errorf("no appearances found for %s", mon.Address.Hex())
		return nil, nil
	}

	if err := opts.readTransactions(mon, txMap, false /* readTraces */); err != nil { // calls IterateOverMap
		return nil, err
	}

	txArray := make([]*types.SimpleTransaction, 0, len(txMap))
	for _, tx := range txMap {
		txArray = append(txArray, tx)
	}

	sort.Slice(txArray, func(i, j int) bool {
		itemI := txArray[i]
		itemJ := txArray[j]
		if itemI.BlockNumber == itemJ.BlockNumber {
			return itemI.TransactionIndex < itemJ.TransactionIndex
		}
		return itemI.BlockNumber < itemJ.BlockNumber
	})

	// Sort the items back into an ordered array by block number
	items := make([]*types.SimpleStatement, 0, len(txMap))

	chain := opts.Globals.Chain
	testMode := opts.Globals.TestMode
	ledgers := ledger.NewLedger(
		opts.Conn,
		mon.Address,
		opts.FirstBlock,
		opts.LastBlock,
		opts.Globals.Ether,
		testMode,
		opts.NoZero,
		opts.Traces,
		&opts.Asset,
	)

	if !opts.Accounting {
		log.Fatal("Accounting is not enabled. Implementation error.")
	}

	apps := make([]types.SimpleAppearance, 0, len(txMap))
	for _, tx := range txArray {
		apps = append(apps, types.SimpleAppearance{
			BlockNumber:      uint32(tx.BlockNumber),
			TransactionIndex: uint32(tx.TransactionIndex),
		})
	}
	_ = ledgers.SetContexts(chain, apps, filter.GetOuterBounds())

	// we need them sorted for the following to work
	for _, tx := range txArray {
		ledgers.Tx = tx // we need this below
		if stmts := ledgers.GetStatementsFromTransaction(opts.Conn, tx); len(stmts) > 0 {
			for _, statement := range stmts {
				statement := statement
				items = append(items, statement)
			}
		}
	}

	sort.Slice(items, func(i, j int) bool {
		itemI := items[i]
		itemJ := items[j]
		if itemI.BlockNumber == itemJ.BlockNumber {
			if itemI.TransactionIndex == itemJ.TransactionIndex {
				return itemI.LogIndex < itemJ.LogIndex
			}
			return itemI.TransactionIndex < itemJ.TransactionIndex
		}
		return itemI.BlockNumber < itemJ.BlockNumber
	})

	// Return the array of items
	return items, nil
}
