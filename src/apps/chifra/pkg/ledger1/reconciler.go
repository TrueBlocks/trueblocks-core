package ledger1

import (
	"encoding/json"
	"fmt"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/names"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/pricing"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

type ReconcilerOptions struct {
	AccountFor   base.Address            `json:"accountFor"`
	FirstBlock   base.Blknum             `json:"firstBlock"`
	LastBlock    base.Blknum             `json:"lastBlock"`
	AsEther      bool                    `json:"asEther"`
	UseTraces    bool                    `json:"useTraces"`
	Reversed     bool                    `json:"reversed"`
	AssetFilters []base.Address          `json:"assetFilters"`
	AppFilters   *types.AppearanceFilter `json:"appFilters"`
}

type Running struct {
	Amount base.Wei    `json:"amount"`
	Prev   base.Blknum `json:"blockNumber"`
}

type Reconciler1 struct {
	Connection     *rpc.Connection             `json:"-"`
	Opts           *ReconcilerOptions          `json:"opts"`
	Names          map[base.Address]types.Name `json:"-"`
	ShowDebugging  bool                        `json:"showDebugging"`
	RemoveAirdrops bool                        `json:"removeAirdrops"`
	Running        map[base.Address]Running    `json:"running"`
}

func (r *Reconciler1) String() string {
	bytes, _ := json.MarshalIndent(r, "", "  ")
	return string(bytes)
}

func NewReconciler(conn *rpc.Connection, opts *ReconcilerOptions) *Reconciler1 {
	parts := types.Custom | types.Prefund | types.Regular
	names, _ := names.LoadNamesMap("mainnet", parts, []string{})
	r := &Reconciler1{
		Opts:           opts,
		Connection:     conn,
		Names:          names,
		ShowDebugging:  true,
		RemoveAirdrops: true,
		Running:        make(map[base.Address]Running),
	}
	return r
}

func (r *Reconciler1) trialBalance(reason string, trans *types.Transaction, stmt *types.Statement, pos *types.AppPosition) (bool, error) {
	logger.TestLog(true, "------------------------------------")
	logger.TestLog(true, "# Reason:", reason)
	logger.TestLog(true, "Trial balance for ", stmt.Asset, stmt.Holder, "at block", trans.BlockNumber)
	logger.TestLog(true, "------------------------------------")
	var err error
	if stmt.BegBal, stmt.EndBal, err = r.Connection.GetReconBalances(&rpc.BalanceOptions{
		CurrBlk: trans.BlockNumber,
		Asset:   stmt.Asset,
		Holder:  stmt.AccountedFor,
	}); err != nil {
		logger.TestLog(true, "----------err GetReconBalances --------------------------")
		logger.TestLog(true)
		return false, err
	}

	if found, k := r.Running[stmt.Asset]; k {
		logger.TestLog(true, "XXXFound ", stmt.Asset, "at block", found.Prev, "of", found.Amount.Text(10))
	} else {
		logger.TestLog(true, "XXXNot found ", stmt.Asset)
	}

	if running, ok := r.Running[stmt.Asset]; ok {
		// We've seen this asset before. Beginning balance is either...
		if running.Prev == trans.BlockNumber {
			// ...(a) the last running balance (if we're in the same block), or...
			logger.TestLog(true, "Same block ", stmt.Asset, "at block", running.Prev, "and", trans.BlockNumber, "of", running.Amount.Text(10))
			stmt.BegBal = running.Amount
		} else {
			// ...(b) the balance from the chain at the last appearance.
			logger.TestLog(true, "Querying at block", running.Prev, "for", stmt.Asset, stmt.Holder)
			if val, err := r.Connection.GetBalanceAtToken(stmt.Asset, stmt.Holder, running.Prev); err != nil {
				logger.TestLog(true, "----------err GetBalanceAtToken --------------------------")
				logger.TestLog(true, "")
				return false, err
			} else {
				if val == nil {
					logger.TestLog(true, "Different block (nil)", stmt.Asset, "at block", running.Prev, "and", trans.BlockNumber, "of", running.Amount.Text(10))
					stmt.BegBal = running.Amount
				} else {
					logger.TestLog(true, "Different block ", stmt.Asset, "at block", running.Prev, "and", trans.BlockNumber, "of", val.Text(10))
					stmt.BegBal = *val
				}
			}
		}
		// The previous balance is the running balance
		stmt.PrevBal = running.Amount
		pos.Prev = running.Prev
	} else {
		// We've never seen this asset before. Beginning balance is already queried (at blockNumber-1) and
		// the previous balance is that beginning balance. Note that this will be zero if blockNumber is 0.
		logger.TestLog(true, "Using block-1 balance for ", stmt.Asset, "at block", running.Prev, "of", running.Amount.Text(10))
		stmt.PrevBal = stmt.BegBal
		pos.Prev = base.Blknum(base.Max(int(trans.BlockNumber), 1) - 1)
	}

	logger.TestLog(true, "pos:", pos.Prev, pos.Current, pos.Next, pos.First, pos.Last)
	if pos.Current == pos.Next {
		logger.TestLog(true, "resets endbal from", stmt.EndBal.Text(10), stmt.EndBalCalc().Text(10))
		stmt.EndBal = *stmt.EndBalCalc()
	}

	if !stmt.Reconciled() {
		if trans.Receipt != nil && stmt.IsNullTransfer(len(trans.Receipt.Logs), trans.To) {
			stmt.CorrectForNullTransfer()
		}
		if !stmt.Reconciled() {
			stmt.CorrectBeginBalance()
			stmt.CorrectEndBalance()
		}
	}

	logger.TestLog(true, "EndBalCalc:", stmt.BegBal.Text(10), stmt.AmountNet().Text(10), stmt.EndBalCalc().Text(10))
	newEndBal := stmt.EndBalCalc() //new(base.Wei).Add(&stmt.BegBal, stmt.AmountNet())
	logger.TestLog(true, "Inserting ", stmt.Asset, "at block", trans.BlockNumber, "of", newEndBal.Text(10))
	r.Running[stmt.Asset] = Running{
		Amount: *newEndBal,
		Prev:   trans.BlockNumber,
	}
	if found, k := r.Running[stmt.Asset]; k {
		logger.TestLog(true, "Found ", stmt.Asset, "at block", found.Prev, "of", found.Amount.Text(10))
	} else {
		logger.TestLog(true, "Not found ", stmt.Asset)
	}

	if !stmt.IsMaterial() {
		// logger.TestLog(true, "----------Not Material --------------------------")
		// logger.TestLog(true, "")
		return stmt.Reconciled(), nil
	}

	stmt.SpotPrice, stmt.PriceSource, _ = pricing.PriceUsd(r.Connection, stmt)
	if r.ShowDebugging {
		stmt.DebugStatement(pos)
	}

	logger.TestLog(true, "----------Done ", stmt.Reconciled(), " --------------------------")
	logger.TestLog(true, "")
	return stmt.Reconciled(), nil
}

func (r *Reconciler1) SkipAirdrop(addr base.Address) bool {
	_ = addr
	return false
	// if name, found := r.Names[addr]; !found {
	// 	return false
	// } else {
	// 	return name.IsAirdrop()
	// }
}

func (r *Reconciler1) GetStatements(pos *types.AppPosition, trans *types.Transaction) ([]types.Statement, error) {
	results := make([]types.Statement, 0, 20)
	if types.AssetOfInterest(r.Opts.AssetFilters, base.FAKE_ETH_ADDRESS) {
		if r.Connection.Store != nil {
			// walk.Cache_Statement
			s := &types.StatementGroup{
				Address:          base.FAKE_ETH_ADDRESS,
				BlockNumber:      trans.BlockNumber,
				TransactionIndex: trans.TransactionIndex,
			}
			if err := r.Connection.Store.Read(s); err == nil {
				// success
				return s.Statements, nil
			}
		}

		reconciled := false
		if !r.Opts.UseTraces {
			if stmt, err := trans.FetchStatement(r.Opts.AsEther, base.FAKE_ETH_ADDRESS, r.Opts.AccountFor); err != nil {
				return nil, err
			} else {
				var err error
				if reconciled, err = r.trialBalance("first-tx", trans, stmt, pos); err != nil {
					return nil, err
				} else {
					if reconciled && stmt.IsMaterial() {
						_ = r.WriteToCache(base.FAKE_ETH_ADDRESS, stmt, trans.Timestamp)
						ReportProgress(stmt)
						results = append(results, *stmt)
					}
				}
			}
		}

		if r.Opts.UseTraces || !reconciled {
			if traces, err := r.Connection.GetTracesByTransactionHash(trans.Hash.Hex(), trans); err != nil {
				return nil, err
			} else {
				if stmt, err := trans.FetchStatementFromTraces(traces, r.Opts.AccountFor, r.Opts.AsEther); err != nil {
					// logger.Warn(colors.Yellow+"Statement at ", fmt.Sprintf("%d.%d", trans.BlockNumber, trans.TransactionIndex), " does not reconcile."+colors.Off)
					// TODO: Silent fail?
					logger.Error(err.Error())
				} else {
					if _, err = r.trialBalance("trace-tx", trans, stmt, pos); err != nil {
						return nil, err
					} else {
						if stmt.IsMaterial() { // append even if not reconciled
							_ = r.WriteToCache(base.FAKE_ETH_ADDRESS, stmt, trans.Timestamp)
							ReportProgress(stmt)
							results = append(results, *stmt)
						}
					}
				}
			}
		}
	}

	if trans.Receipt != nil {
		if statements, err := trans.Receipt.FetchStatements(r.Opts.AccountFor, r.Opts.AssetFilters, r.Opts.AppFilters); err != nil {
			return nil, err
		} else {
			for _, s := range statements {
				if r.Connection.Store != nil {
					// walk.Cache_Statement
					sg := &types.StatementGroup{
						Address:          s.Asset,
						BlockNumber:      trans.BlockNumber,
						TransactionIndex: trans.TransactionIndex,
					}
					if err := r.Connection.Store.Read(sg); err == nil {
						// success
						results = append(results, sg.Statements...)
						continue
					}
				}

				stmt := &s
				stmt.Symbol = stmt.Asset.DefaultSymbol()
				stmt.Decimals = base.Value(18)
				if name, found := r.Names[stmt.Asset]; found {
					if r.RemoveAirdrops && r.SkipAirdrop(stmt.Asset) {
						continue
					}
					if name.Symbol != "" {
						stmt.Symbol = name.Symbol
					}
					if name.Decimals != 0 {
						stmt.Decimals = base.Value(name.Decimals)
					}
				}

				if _, err := r.trialBalance("token", trans, stmt, pos); err != nil {
					// TODO: Silent fail?
					logger.Error(err.Error())
					continue
				} else {
					// if reconciled {
					// 	id := fmt.Sprintf(" %d.%d.%d", stmt.BlockNumber, stmt.TransactionIndex, stmt.LogIndex)
					// 	logger.Progress(true, colors.Green+"Transaction", id, "reconciled       "+colors.Off)
					// }
					// order matters - don't move
					if stmt.IsMaterial() { // add even if not reconciled
						_ = r.WriteToCache(base.FAKE_ETH_ADDRESS, stmt, trans.Timestamp)
						ReportProgress(stmt)
						results = append(results, *stmt)
					}
				}
			}
		}
	}
	return results, nil
}

func ReportProgress(stmt *types.Statement) {
	msg := fmt.Sprintf("Ether statement at % 9d.%d.%d %s %s", stmt.BlockNumber, stmt.TransactionIndex, stmt.LogIndex, stmt.Asset.Hex(), stmt.Holder.Hex())
	if !stmt.IsEth() {
		msg = fmt.Sprintf("Token statement at % 9d.%d.%d %s %s", stmt.BlockNumber, stmt.TransactionIndex, stmt.LogIndex, stmt.Asset.Hex(), stmt.Holder.Hex())
	}
	spacer := strings.Repeat(" ", 100-base.Min(100, len(msg)))
	if !stmt.Reconciled() {
		// logger.Progress(true, colors.Green+msg+" reconciled.", colors.Off, spacer)
		// } else {
		logger.Warn(msg+" did not reconcile.", spacer)
	}
}

func (r *Reconciler1) WriteToCache(addr base.Address, stmt *types.Statement, ts base.Timestamp) error {
	// if !stmt.Reconciled() {
	_ = addr
	_ = stmt
	_ = ts
	return nil
	// }

	// sg := &types.StatementGroup{
	// 	Address:          base.FAKE_ETH_ADDRESS,
	// 	BlockNumber:      stmt.BlockNumber,
	// 	TransactionIndex: stmt.TransactionIndex,
	// 	Statements:       []types.Statement{*stmt},
	// }
	// return r.Connection.Store.WriteToCache(sg, walk.Cache_Statements, ts)
}
