package ledger

import (
	"sort"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/walk"
)

// -----------------------------------------------------------------
func (r *Reconciler) statementsFromCache(trans *types.Transaction) ([]types.Statement, bool) {
	if r.HasFilters() {
		nHits := 0
		results := make([]types.Statement, 0)
		for _, asset := range r.Opts.AssetFilters {
			sg := &types.StatementGroup{
				Holder:           r.Opts.AccountFor,
				Asset:            asset,
				BlockNumber:      trans.BlockNumber,
				TransactionIndex: trans.TransactionIndex,
			}
			if err := r.Connection.ReadFromCache(sg); err == nil {
				results = append(results, sg.Statements...)
				nHits++
			} else {
				break
			}
		}
		sort.Slice(results, func(i, j int) bool {
			if results[i].IsEth() != results[j].IsEth() {
				results[i].IsEth()
			}
			if results[i].BlockNumber != results[j].BlockNumber {
				return results[i].BlockNumber < results[j].BlockNumber
			}
			if results[i].TransactionIndex != results[j].TransactionIndex {
				return results[i].TransactionIndex < results[j].TransactionIndex
			}
			return results[i].LogIndex < results[j].LogIndex
		})
		if nHits == len(r.Opts.AssetFilters) {
			r.attachPointers(trans, results)
			return results, true
		}
	}

	sg := &types.StatementGroup{
		Holder:           r.Opts.AccountFor,
		BlockNumber:      trans.BlockNumber,
		TransactionIndex: trans.TransactionIndex,
	}
	if err := r.Connection.ReadFromCache(sg); err == nil {
		if r.HasFilters() {
			results := make([]types.Statement, 0)
			for _, asset := range r.Opts.AssetFilters {
				for _, stmt := range sg.Statements {
					if stmt.Asset == asset {
						results = append(results, stmt)
					}
				}
			}
			r.attachPointers(trans, results)
			return results, true
		}
		r.attachPointers(trans, sg.Statements)
		return sg.Statements, true
	}

	return nil, false
}

// -----------------------------------------------------------------
func (r *Reconciler) statementsToCache(trans *types.Transaction, stmts []types.Statement) error {
	if r.HasFilters() {
		for _, asset := range r.Opts.AssetFilters {
			out := make([]types.Statement, 0)
			for _, stmt := range stmts {
				if stmt.Asset == asset {
					out = append(out, stmt)
				}
			}
			if len(out) == 0 {
				continue
			}
			sg := &types.StatementGroup{
				Holder:           r.Opts.AccountFor,
				Asset:            asset,
				BlockNumber:      trans.BlockNumber,
				TransactionIndex: trans.TransactionIndex,
				Statements:       out,
			}
			err := r.Connection.WriteToCache(sg, walk.Cache_Statements, trans.Timestamp)
			if err != nil {
				return err
			}
		}
		return nil
	}

	statementGroup := &types.StatementGroup{
		Holder:           r.Opts.AccountFor,
		BlockNumber:      trans.BlockNumber,
		TransactionIndex: trans.TransactionIndex,
		Statements:       stmts,
	}
	return r.Connection.WriteToCache(statementGroup, walk.Cache_Statements, trans.Timestamp)
}

// -----------------------------------------------------------------
func (r *Reconciler) attachPointers(trans *types.Transaction, stmts []types.Statement) {
	for i := range stmts {
		stmts[i].Transaction = trans
		if !stmts[i].IsEth() && trans.Receipt != nil && len(trans.Receipt.Logs) > int(stmts[i].LogIndex) {
			stmts[i].Log = &trans.Receipt.Logs[stmts[i].LogIndex]
		}
	}
}
