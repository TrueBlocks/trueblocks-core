package ledger1

import (
	"encoding/json"
	"fmt"
	"sort"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
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
	AddCorrections bool                        `json:"addCorrections"`
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
		AddCorrections: true,
		ShowDebugging:  true,
		RemoveAirdrops: true,
		Running:        make(map[base.Address]Running),
	}
	return r
}

func (r *Reconciler1) trialBalance(trans *types.Transaction, stmt *types.Statement, pos *types.AppPosition, correct bool) (bool, error) {
	var err error
	if stmt.PrevBal, stmt.BegBal, stmt.EndBal, err = r.Connection.GetReconBalances(&rpc.BalanceOptions{
		PrevAppBlk: pos.Prev,
		CurrBlk:    trans.BlockNumber,
		Asset:      stmt.Asset,
		Holder:     stmt.AccountedFor,
	}); err != nil {
		return false, err
	}
	if running, ok := r.Running[stmt.Asset]; ok {
		stmt.PrevBal = running.Amount
		pos.Prev = running.Prev
	}

	if correct {
		if !stmt.Reconciled() {
			if trans.Receipt != nil && stmt.IsNullTransfer(len(trans.Receipt.Logs), trans.To) {
				r.correctForNullTransfer(stmt)
			}
			if !stmt.Reconciled() {
				r.correctBegEndBal(stmt)
			}
		}
	}

	r.Running[stmt.Asset] = Running{
		Amount: *stmt.EndBalCalc(),
		Prev:   trans.BlockNumber,
	}

	if !stmt.IsMaterial() {
		return stmt.Reconciled(), nil
	}

	stmt.SpotPrice, stmt.PriceSource, _ = pricing.PriceUsd(r.Connection, stmt)
	if r.ShowDebugging {
		stmt.DebugStatement(pos)
	}

	return stmt.Reconciled(), nil
}

func (r *Reconciler1) correctForNullTransfer(s *types.Statement) bool {
	logger.TestLog(true, "Correcting token transfer for a null transfer")
	amt := s.TotalIn() // use totalIn since this is the amount that was faked
	s.AmountOut = *base.ZeroWei
	s.AmountIn = *base.ZeroWei
	s.CorrectingIn = *amt
	s.CorrectingOut = *amt
	s.CorrectingReason = "null-transfer"
	return s.Reconciled()
}

func (r *Reconciler1) correctBegEndBal(stmt *types.Statement) bool {
	reasons := []string{}
	if !stmt.BegBalDiff().Equal(base.ZeroWei) {
		logger.TestLog(true, "Correcting token transfer for unknown income or outflow")
		if stmt.BegBalDiff().LessThan(base.ZeroWei) {
			reasons = append(reasons, "begbal-out")
			val := new(base.Wei).Add(&stmt.CorrectingOut, stmt.BegBalDiff())
			stmt.CorrectingOut = *val
		} else {
			reasons = append(reasons, "begbal-in")
			val := new(base.Wei).Add(&stmt.CorrectingIn, stmt.BegBalDiff())
			stmt.CorrectingIn = *val
		}
	}

	if !stmt.EndBalDiff().Equal(base.ZeroWei) {
		logger.TestLog(true, "Correcting token transfer for unknown income or outflow")
		if stmt.EndBalDiff().LessThan(base.ZeroWei) {
			reasons = append(reasons, "endbal-out")
			val := new(base.Wei).Add(&stmt.CorrectingOut, stmt.EndBalDiff())
			stmt.CorrectingOut = *val
		} else {
			reasons = append(reasons, "endbal-in")
			val := new(base.Wei).Add(&stmt.CorrectingIn, stmt.EndBalDiff())
			stmt.CorrectingIn = *val
		}
	}

	stmt.CorrectingReason = strings.Join(reasons, "-")
	return stmt.Reconciled()
}

func (r *Reconciler1) SkipAirdrop(addr base.Address) bool {
	if name, found := r.Names[addr]; !found {
		return false
	} else {
		return name.IsAirdrop()
	}
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
				if reconciled, err = r.trialBalance(trans, stmt, pos, false); err != nil {
					return nil, err
				} else {
					if reconciled && stmt.IsMaterial() {
						r.addStatement(&results, stmt)
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
					if _, err = r.trialBalance(trans, stmt, pos, r.AddCorrections); err != nil {
						return nil, err
					} else {
						if stmt.IsMaterial() { // append even if not reconciled
							r.addStatement(&results, stmt)
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

				if reconciled, err := r.trialBalance(trans, stmt, pos, r.AddCorrections); err != nil {
					// TODO: Silent fail?
					continue
				} else {
					if reconciled {
						id := fmt.Sprintf(" %d.%d.%d", stmt.BlockNumber, stmt.TransactionIndex, stmt.LogIndex)
						logger.Progress(true, colors.Green+"Transaction", id, "reconciled       "+colors.Off)
					}

					// order matters - don't move
					if stmt.IsMaterial() { // add even if not reconciled
						r.addStatement(&results, stmt)
					}
				}
			}
		}
	}
	return results, nil
}

func (r *Reconciler1) addStatement(results *[]types.Statement, stmt *types.Statement) {
	if !base.IsTestMode() && !stmt.Reconciled() {
		t := "Eth"
		if !stmt.IsEth() {
			t = "Token"
		}
		logger.Warn(t, "statement at ", fmt.Sprintf("%d.%d.%d", stmt.BlockNumber, stmt.TransactionIndex, stmt.LogIndex), " does not reconcile.")
	}
	*results = append(*results, *stmt)
}

// GetTransfers returns a statement from a given transaction
func (r *Reconciler1) GetTransfers(pos *types.AppPosition, trans *types.Transaction) ([]types.Transfer, error) {
	var statements []types.Statement
	if types.AssetOfInterest(r.Opts.AssetFilters, base.FAKE_ETH_ADDRESS) {
		var stmt *types.Statement
		if r.Opts.UseTraces {
			if traces, err := r.Connection.GetTracesByTransactionHash(trans.Hash.Hex(), trans); err != nil {
				return nil, err
			} else {
				if s, err := trans.FetchStatementFromTraces(traces, r.Opts.AccountFor, r.Opts.AsEther); err != nil {
					return nil, err
				} else {
					stmt = s
				}
			}
		} else {
			var err error
			if stmt, err = trans.FetchStatement(r.Opts.AsEther, base.FAKE_ETH_ADDRESS, r.Opts.AccountFor); err != nil {
				return nil, err
			}
		}
		// Append only if the statement is material
		if stmt.IsMaterial() {
			statements = append(statements, *stmt)
		}
	}

	if trans.Receipt != nil {
		if receiptStatements, err := trans.Receipt.FetchStatements(r.Opts.AccountFor, r.Opts.AssetFilters, r.Opts.AppFilters); err != nil {
			return nil, err
		} else {
			for _, stmt := range receiptStatements {
				if r.RemoveAirdrops && r.SkipAirdrop(stmt.Asset) {
					continue
				} else if stmt.IsMaterial() {
					statements = append(statements, stmt)
				}
			}
		}
	}

	return convertToTransfers(statements)
}

func convertToTransfers(statements []types.Statement) ([]types.Transfer, error) {
	transfers := make([]types.Transfer, 0, len(statements)*2)
	for _, stmnt := range statements {
		t := types.Transfer{
			Asset:            stmnt.Asset,
			Holder:           stmnt.AccountedFor,
			Amount:           *stmnt.AmountNet(),
			BlockNumber:      stmnt.BlockNumber,
			TransactionIndex: stmnt.TransactionIndex,
			LogIndex:         stmnt.LogIndex,
			Decimals:         stmnt.Decimals,
			CorrectingReason: stmnt.CorrectingReason,
		}
		if !t.Amount.Equal(base.ZeroWei) {
			transfers = append(transfers, t)
		}
	}

	if base.IsTestMode() {
		sort.Slice(transfers, func(i, j int) bool {
			if transfers[i].BlockNumber == transfers[j].BlockNumber {
				if transfers[i].TransactionIndex == transfers[j].TransactionIndex {
					return transfers[i].LogIndex < transfers[j].LogIndex
				}
				return transfers[i].TransactionIndex < transfers[j].TransactionIndex
			}
			return transfers[i].BlockNumber < transfers[j].BlockNumber
		})
		for _, t := range transfers {
			if !t.Amount.Equal(base.ZeroWei) {
				logger.TestLog(true, "transfer:", t.BlockNumber, t.TransactionIndex, t.LogIndex, t.Asset, t.Holder, t.Amount.Text(10), t.CorrectingReason)
			}
		}
	}

	return transfers, nil
}
