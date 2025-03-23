package ledger

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/pricing"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// -----------------------------------------------------------------
func (r *Reconciler) trialBalance(reason string, stmt *types.Statement, node *types.AppNode[types.Transaction], correct bool, balances map[BalanceOptions]*base.Wei) (bool, error) {
	trans := node.Data()
	key := NewAssetHolderKey(stmt.Asset, stmt.AccountedFor)
	running, found := r.Running[key]
	isFirstInBlock := !found || running.stmt == nil || running.stmt.BlockNumber != trans.BlockNumber
	isLastInBlock := node.IsLastInBlock()

	if reason == "token" {
		if !stmt.BegSentinel { // Not the first log statement
			isFirstInBlock = false // Force Case 1 or 3
		}
		if !stmt.EndSentinel { // Not the last log statement
			isLastInBlock = false // Force Case 2 or 3, depending on isFirstInBlock
		}
		// If Sentinel = true, retain node.IsLastInBlock() to determine Case 4
	}

	switch {
	case isFirstInBlock && isLastInBlock:
		return r.case1FirstLast(key, reason, stmt, node, correct, balances)

	case !isFirstInBlock && !isLastInBlock:
		return r.case2MidMid(key, reason, stmt, node, balances)

	case isFirstInBlock && !isLastInBlock:
		return r.case3FirstMid(key, reason, stmt, node, correct, balances)

	case !isFirstInBlock && isLastInBlock:
		return r.case4MidLast(key, reason, stmt, node, correct, balances)

	default:
		panic("unhandled case")
	}
}

// -----------------------------------------------------------------
func (r *Reconciler) case1FirstLast(key AssetHolder, reason string, stmt *types.Statement, node *types.AppNode[types.Transaction], correct bool, balances map[BalanceOptions]*base.Wei) (bool, error) {
	trans := node.Data()
	logger.TestLog(true, "XXX =============[ "+reason+" ]======================")
	logger.TestLog(true, "XXX Case 1: Only statement in block")
	logger.TestLog(true, "XXX Block:", trans.BlockNumber, "Tx:", trans.TransactionIndex, "Asset:", stmt.Asset.Hex())
	logger.TestLog(true, "XXX FirstInBlock: true, LastInBlock: true")
	logger.TestLog(true, "XXX ================================================")

	balOpts := BalanceOptions{
		BlockNumber: trans.BlockNumber,
		Asset:       stmt.Asset,
		Holder:      stmt.AccountedFor,
	}
	if b, e, err := r.GetReconBalances(&balOpts, balances); err != nil {
		return false, err
	} else {
		stmt.BegBal = *b
		stmt.EndBal = *e
	}

	running, found := r.Running[key]
	if found && running.stmt.BlockNumber != trans.BlockNumber {
		if running.amt.Cmp(&stmt.BegBal) != 0 {
			logger.TestLog(true, "Discrepancy detected for", stmt.Asset, "at block", trans.BlockNumber,
				"running:", running.amt.Text(10), "chain:", stmt.BegBal.Text(10))
			running.amt = stmt.BegBal
		}
	}

	stmt.PrevBal = stmt.BegBal
	newEndBal := stmt.EndBalCalc()
	r.Running[key] = Running{
		amt:  *newEndBal,
		stmt: stmt,
	}

	reconciled := newEndBal.Cmp(&stmt.EndBal) == 0
	if !reconciled && correct {
		if trans.Receipt != nil && stmt.IsNullTransfer(len(trans.Receipt.Logs), trans.To) {
			stmt.CorrectForNullTransfer()
		}
		if !stmt.Reconciled() {
			stmt.CorrectBeginBalance()
			stmt.CorrectEndBalance()
		}
		reconciled = stmt.EndBalCalc().Cmp(&stmt.EndBal) == 0
	}

	stmt.SpotPrice, stmt.PriceSource, _ = pricing.PriceUsd(r.Connection, stmt)
	if r.ShowDebugging {
		stmt.DebugStatement(node)
	}
	return reconciled, nil
}

// -----------------------------------------------------------------
func (r *Reconciler) case2MidMid(key AssetHolder, reason string, stmt *types.Statement, node *types.AppNode[types.Transaction], balances map[BalanceOptions]*base.Wei) (bool, error) {
	_ = balances
	trans := node.Data()
	logger.TestLog(true, "XXX =============[ "+reason+" ]======================")
	logger.TestLog(true, "XXX Case 2: Middle statement in block")
	logger.TestLog(true, "XXX Block:", trans.BlockNumber, "Tx:", trans.TransactionIndex, "Asset:", stmt.Asset.Hex())
	logger.TestLog(true, "XXX FirstInBlock: false, LastInBlock: false")
	logger.TestLog(true, "XXX ================================================")

	running := r.Running[key]
	stmt.BegBal = running.amt
	stmt.PrevBal = stmt.BegBal
	newEndBal := stmt.EndBalCalc()
	stmt.EndBal = *newEndBal
	r.Running[key] = Running{
		amt:  *newEndBal,
		stmt: stmt,
	}

	stmt.SpotPrice, stmt.PriceSource, _ = pricing.PriceUsd(r.Connection, stmt)
	if r.ShowDebugging {
		stmt.DebugStatement(node)
	}

	return true, nil
}

// -----------------------------------------------------------------
func (r *Reconciler) case3FirstMid(key AssetHolder, reason string, stmt *types.Statement, node *types.AppNode[types.Transaction], correct bool, balances map[BalanceOptions]*base.Wei) (bool, error) {
	_ = correct
	trans := node.Data()
	logger.TestLog(true, "XXX =============[ "+reason+" ]======================")
	logger.TestLog(true, "XXX Case 3: First statement in block with more")
	logger.TestLog(true, "XXX Block:", trans.BlockNumber, "Tx:", trans.TransactionIndex, "Asset:", stmt.Asset.Hex())
	logger.TestLog(true, "XXX FirstInBlock: true, LastInBlock: false")
	logger.TestLog(true, "XXX ================================================")

	running, found := r.Running[key]
	if found && running.stmt.BlockNumber == trans.BlockNumber {
		stmt.BegBal = running.amt
	} else {
		balOpts := BalanceOptions{
			BlockNumber: trans.BlockNumber,
			Asset:       stmt.Asset,
			Holder:      stmt.AccountedFor,
		}
		if b, _, err := r.GetReconBalances(&balOpts, balances); err != nil {
			return false, err
		} else {
			stmt.BegBal = *b
		}
		if found && running.amt.Cmp(&stmt.BegBal) != 0 {
			logger.TestLog(true, "Discrepancy detected for", stmt.Asset, "at block", trans.BlockNumber,
				"running:", running.amt.Text(10), "chain:", stmt.BegBal.Text(10))
			running.amt = stmt.BegBal
		}
	}

	stmt.PrevBal = stmt.BegBal
	newEndBal := stmt.EndBalCalc()
	stmt.EndBal = *newEndBal
	r.Running[key] = Running{
		amt:  *newEndBal,
		stmt: stmt,
	}

	stmt.SpotPrice, stmt.PriceSource, _ = pricing.PriceUsd(r.Connection, stmt)
	if r.ShowDebugging {
		stmt.DebugStatement(node)
	}

	return true, nil
}

// -----------------------------------------------------------------
func (r *Reconciler) case4MidLast(key AssetHolder, reason string, stmt *types.Statement, node *types.AppNode[types.Transaction], correct bool, balances map[BalanceOptions]*base.Wei) (bool, error) {
	_ = reason
	trans := node.Data()
	logger.TestLog(true, "XXX =============[ "+reason+" ]======================")
	logger.TestLog(true, "XXX Case 4: Last statement in block with prior")
	logger.TestLog(true, "XXX Block:", trans.BlockNumber, "Tx:", trans.TransactionIndex, "Asset:", stmt.Asset.Hex())
	logger.TestLog(true, "XXX FirstInBlock: false, LastInBlock: true")
	logger.TestLog(true, "XXX ================================================")

	running := r.Running[key]
	stmt.BegBal = running.amt
	stmt.PrevBal = stmt.BegBal
	newEndBal := stmt.EndBalCalc()
	stmt.EndBal = *newEndBal

	var blockchainEndBal base.Wei
	balOpts := BalanceOptions{
		BlockNumber: trans.BlockNumber,
		Asset:       stmt.Asset,
		Holder:      stmt.AccountedFor,
	}
	if _, e, err := r.GetReconBalances(&balOpts, balances); err != nil {
		return false, err
	} else {
		blockchainEndBal = *e
	}

	reconciled := newEndBal.Cmp(&blockchainEndBal) == 0
	if !reconciled && correct {
		if trans.Receipt != nil && stmt.IsNullTransfer(len(trans.Receipt.Logs), trans.To) {
			stmt.CorrectForNullTransfer()
		}
		if !stmt.Reconciled() {
			stmt.CorrectBeginBalance()
			stmt.CorrectEndBalance()
		}
		reconciled = stmt.EndBalCalc().Cmp(&blockchainEndBal) == 0
	}

	r.Running[key] = Running{
		amt:  *stmt.EndBalCalc(),
		stmt: stmt,
	}

	stmt.SpotPrice, stmt.PriceSource, _ = pricing.PriceUsd(r.Connection, stmt)
	if r.ShowDebugging {
		stmt.DebugStatement(node)
	}

	return reconciled, nil
}
