package ledger1

import (
	"encoding/json"
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/names"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/pricing"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
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
	logger.Info("------------------------------------")
	logger.Info("# Reason:", reason)
	logger.Info("Trial balance for ", stmt.Asset, stmt.Holder, "at block", trans.BlockNumber)
	logger.Info("------------------------------------")
	var err error
	if stmt.BegBal, stmt.EndBal, err = r.Connection.GetReconBalances(&rpc.BalanceOptions{
		CurrBlk: trans.BlockNumber,
		Asset:   stmt.Asset,
		Holder:  stmt.AccountedFor,
	}); err != nil {
		logger.Info("----------err GetReconBalances --------------------------")
		logger.Info()
		return false, err
	}

	if found, k := r.Running[stmt.Asset]; k {
		logger.Info("XXXFound ", stmt.Asset, "at block", found.Prev, "of", found.Amount.Text(10))
	} else {
		logger.Info("XXXNot found ", stmt.Asset)
	}

	if running, ok := r.Running[stmt.Asset]; ok {
		// We've seen this asset before. Begining balance is either...
		if running.Prev == trans.BlockNumber {
			// ...(a) the last running balance (if we're in the same block), or...
			logger.Info("Same block ", stmt.Asset, "at block", running.Prev, "and", trans.BlockNumber, "of", running.Amount.Text(10))
			stmt.BegBal = running.Amount
		} else {
			// ...(b) the balance from the chain at the last appearance.
			logger.Info("Querying at block", running.Prev, "for", stmt.Asset, stmt.Holder)
			if val, err := r.Connection.GetBalanceAtToken(stmt.Asset, stmt.Holder, running.Prev); err != nil {
				logger.Info("----------err GetBalanceAtToken --------------------------")
				logger.Info()
				return false, err
			} else {
				if val == nil {
					logger.Info("Different block (nil)", stmt.Asset, "at block", running.Prev, "and", trans.BlockNumber, "of", running.Amount.Text(10))
					stmt.BegBal = running.Amount
				} else {
					logger.Info("Different block ", stmt.Asset, "at block", running.Prev, "and", trans.BlockNumber, "of", val.Text(10))
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
		logger.Info("Using block-1 balance for ", stmt.Asset, "at block", running.Prev, "of", running.Amount.Text(10))
		stmt.PrevBal = stmt.BegBal
		pos.Prev = base.Blknum(utils.Max(int(trans.BlockNumber), 1) - 1)
	}

	logger.Info("pos:", pos.Prev, pos.Current, pos.Next, pos.First, pos.Last)
	if pos.Current == pos.Next {
		logger.Info("resets endbal from", stmt.EndBal.Text(10), stmt.EndBalCalc().Text(10))
		stmt.EndBal = *stmt.EndBalCalc()
	}

	if !stmt.Reconciled() {
		if trans.Receipt != nil && stmt.IsNullTransfer(len(trans.Receipt.Logs), trans.To) {
			stmt.CorrectForNullTransfer()
		}
		if !stmt.Reconciled() {
			stmt.CorrectBegEndBal()
		}
	}

	logger.Info("EndBalCalc:", stmt.BegBal.Text(10), stmt.AmountNet().Text(10), stmt.EndBalCalc().Text(10))
	newEndBal := stmt.EndBalCalc() //new(base.Wei).Add(&stmt.BegBal, stmt.AmountNet())
	logger.Info("Inserting ", stmt.Asset, "at block", trans.BlockNumber, "of", newEndBal.Text(10))
	r.Running[stmt.Asset] = Running{
		Amount: *newEndBal,
		Prev:   trans.BlockNumber,
	}
	if found, k := r.Running[stmt.Asset]; k {
		logger.Info("Found ", stmt.Asset, "at block", found.Prev, "of", found.Amount.Text(10))
	} else {
		logger.Info("Not found ", stmt.Asset)
	}

	if !stmt.IsMaterial() {
		// logger.Info("----------Not Material --------------------------")
		// logger.Info()
		return stmt.Reconciled(), nil
	}

	stmt.SpotPrice, stmt.PriceSource, _ = pricing.PriceUsd(r.Connection, stmt)
	if r.ShowDebugging {
		stmt.DebugStatement(pos)
	}

	logger.Info("----------Done ", stmt.Reconciled(), " --------------------------")
	logger.Info()
	return stmt.Reconciled(), nil
}

func (r *Reconciler1) SkipAirdrop(addr base.Address) bool {
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
						stmt.WarnIfUnreconciled()
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
					logger.Warn(colors.Yellow+"Statement at ", fmt.Sprintf("%d.%d", trans.BlockNumber, trans.TransactionIndex), " does not reconcile."+colors.Off)
				} else {
					if _, err = r.trialBalance("trace-tx", trans, stmt, pos); err != nil {
						return nil, err
					} else {
						if stmt.IsMaterial() { // append even if not reconciled
							stmt.WarnIfUnreconciled()
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

				if reconciled, err := r.trialBalance("token", trans, stmt, pos); err != nil {
					// TODO: Silent fail?
					continue
				} else {
					if reconciled {
						id := fmt.Sprintf(" %d.%d.%d", stmt.BlockNumber, stmt.TransactionIndex, stmt.LogIndex)
						logger.Progress(true, colors.Green+"Transaction", id, "reconciled       "+colors.Off)
					}

					// order matters - don't move
					if stmt.IsMaterial() { // add even if not reconciled
						stmt.WarnIfUnreconciled()
						results = append(results, *stmt)
					}
				}
			}
		}
	}
	return results, nil
}
