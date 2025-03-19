package ledger

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (r *Reconciler) GetStatements2(node *types.AppNode[types.Transaction]) ([]types.Statement, error) {
	_ = node
	// trans := node.Data()
	// failfail := func(n int) {
	// 	logger.TestLog(true, fmt.Sprintf("~failfail %d~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~", n))
	// }

	// debugHeader(node)

	// results := make([]types.Statement, 0, 20)

	// // Pass 1: Build the statement list
	// // apps := []types.Appearance{
	// // 	{
	// // 		BlockNumber:      uint32(trans.BlockNumber),
	// // 		TransactionIndex: uint32(trans.TransactionIndex),
	// // 	},
	// // }
	// txArray := []*types.Transaction{trans}
	// list, err := r.GetStatementList(txArray)
	// if err != nil {
	// 	failfail(1)
	// 	return nil, err
	// }

	// // Pass 2: Get balances and reconcile statements
	// transfers := make([]types.Transfer, 0)
	// for n := list.Head; n != nil; n = n.Next() {
	// 	stmt := n.Data()
	// 	if stmt != nil {
	// 		transfers = append(transfers, types.Transfer{
	// 			Asset:            stmt.Asset,
	// 			Holder:           stmt.AccountedFor,
	// 			Amount:           *stmt.AmountNet(),
	// 			BlockNumber:      stmt.BlockNumber,
	// 			TransactionIndex: stmt.TransactionIndex,
	// 			LogIndex:         stmt.LogIndex,
	// 			Decimals:         uint64(stmt.Decimals),
	// 		})
	// 	}
	// }
	// balances, err := r.GetBalancesMap(transfers)
	// if err != nil {
	// 	failfail(2)
	// 	return nil, err
	// }

	// reconciledStatements, err := r.ReconcileStatements(list, balances)
	// if err != nil {
	// 	failfail(3)
	// 	return nil, err
	// }

	// for _, stmt := range reconciledStatements {
	// 	// if r.Connection.Store != nil {
	// 	// 	sg := &types.StatementGroup{
	// 	// 		Address:          stmt.Asset,
	// 	// 		BlockNumber:      trans.BlockNumber,
	// 	// 		TransactionIndex: trans.TransactionIndex,
	// 	// 	}
	// 	// 	if err := r.Connection.Store.Read(sg); err == nil {
	// 	// 		results = append(results, sg.Statements...)
	// 	// 		continue
	// 	// 	}
	// 	// }

	// 	stmt.Symbol = stmt.Asset.DefaultSymbol()
	// 	stmt.Decimals = base.Value(18)
	// 	if name, found := r.Names[stmt.Asset]; found {
	// 		if r.RemoveAirdrops && r.SkipAirdrop(stmt.Asset) {
	// 			logger.TestLog(true, "Removing airdrop for", stmt.Asset, "at block", stmt.BlockNumber, ".", stmt.TransactionIndex)
	// 			continue
	// 		}
	// 		if name.Symbol != "" {
	// 			stmt.Symbol = name.Symbol
	// 		}
	// 		if name.Decimals != 0 {
	// 			stmt.Decimals = base.Value(name.Decimals)
	// 		}
	// 	}

	// 	if stmt.IsMaterial() {
	// 		// _ = r.WriteToCache(base.FAKE_ETH_ADDRESS, &stmt, trans.Timestamp)
	// 		ReportProgress(&stmt, true)
	// 		logger.TestLog(true, "Statement is material - appending. reconciled:", stmt.Reconciled())
	// 		results = append(results, stmt)
	// 	} else {
	// 		logger.TestLog(true, "Statement was not material")
	// 	}
	// }

	// logger.TestLog(true, fmt.Sprintf("~~~ Leaving: %d.%d ~~~", node.CurBlock(), node.CurTxId()))
	// logger.TestLog(true, "------------------------------------", len(results), "statements generated.")
	// logger.TestLog(true, "")
	// return results, nil
	return nil, nil
}
