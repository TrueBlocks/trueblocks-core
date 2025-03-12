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

type Reconciler1 struct {
	Connection     *rpc.Connection             `json:"-"`
	Opts           *ReconcilerOptions          `json:"opts"`
	Names          map[base.Address]types.Name `json:"-"`
	AddCorrections bool                        `json:"addCorrections"`
	ShowDebugging  bool                        `json:"showDebugging"`
	RemoveAirdrops bool                        `json:"removeAirdrops"`
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
	}
	return r
}

// trialBalance returns true of the reconciliation balances, false otherwise. If the statement
// does not reconcile, it tries to repair it in two ways (a) for null transfers and (b) for
// any other reason. If that works and the statement is material (money moved in some way), the
// function tries to price the asset. it then prints optional debugging information. Note that
// the statement may be modified in this function.
func (r *Reconciler1) trialBalance(pos *types.AppPosition, trans *types.Transaction, s *types.Statement) (bool, error) {
	var err error
	if s.PrevBal, s.BegBal, s.EndBal, err = r.Connection.GetReconBalances(&rpc.BalanceOptions{
		PrevAppBlk: pos.Prev,
		CurrBlk:    trans.BlockNumber,
		Asset:      s.Asset,
		Holder:     s.AccountedFor,
	}); err != nil {
		return false, err
	}

	if r.AddCorrections {
		var okay bool
		if okay = s.Reconciled(); !okay {
			if !s.IsEth() {
				if okay = r.correctForNullTransfer(s, trans); !okay {
					_ = r.correctForSomethingElseToken(s)
				}
			}
		}
	}

	if !s.IsMaterial() {
		return s.Reconciled(), nil
	}

	s.SpotPrice, s.PriceSource, _ = pricing.PriceUsd(r.Connection, s)
	if r.ShowDebugging {
		s.DebugStatement(pos)
	}

	return s.Reconciled(), nil
}

func (r *Reconciler1) correctForSomethingElseToken(s *types.Statement) bool {
	logger.TestLog(true, "Correcting token transfer for unknown income or outflow")

	s.CorrectingIn.SetUint64(0)
	s.CorrectingOut.SetUint64(0)
	s.CorrectingReason = ""
	zero := new(base.Wei).SetInt64(0)
	cmpBegBal := s.BegBalDiff().Cmp(zero)
	cmpEndBal := s.EndBalDiff().Cmp(zero)

	if cmpBegBal > 0 {
		s.CorrectingIn = *s.BegBalDiff()
		s.CorrectingReason = "begbal"
	} else if cmpBegBal < 0 {
		s.CorrectingOut = *s.BegBalDiff()
		s.CorrectingReason = "begbal"
	}

	if cmpEndBal > 0 {
		n := new(base.Wei).Add(&s.CorrectingIn, s.EndBalDiff())
		s.CorrectingIn = *n
		s.CorrectingReason += "endbal"
	} else if cmpEndBal < 0 {
		n := new(base.Wei).Add(&s.CorrectingOut, s.EndBalDiff())
		s.CorrectingOut = *n
		s.CorrectingReason += "endbal"
	}
	s.CorrectingReason = strings.Replace(s.CorrectingReason, "begbalendbal", "begbal-endbal", -1)

	return s.Reconciled()
}

func (r *Reconciler1) correctForNullTransfer(s *types.Statement, tx *types.Transaction) bool {
	if s.IsNullTransfer(tx) {
		logger.TestLog(true, "Correcting token transfer for a null transfer")
		amt := s.TotalIn() // use totalIn since this is the amount that was faked
		s.AmountOut = *base.ZeroWei
		s.AmountIn = *base.ZeroWei
		s.CorrectingIn = *amt
		s.CorrectingOut = *amt
		s.CorrectingReason = "null-transfer"
	} else {
		logger.TestLog(true, "Needs correction for token transfer")
	}

	return s.Reconciled()
}

func (r *Reconciler1) SkipAirdrop(stmt *types.Statement) bool {
	if !r.RemoveAirdrops {
		return false
	}

	if name, found := r.Names[stmt.Asset]; !found {
		return false
	} else {
		return name.IsAirdrop()
	}
}

func (r *Reconciler1) GetStatements(pos *types.AppPosition, trans *types.Transaction) ([]types.Statement, error) {
	results := make([]types.Statement, 0, 20)
	if types.AssetOfInterest(r.Opts.AssetFilters, base.FAKE_ETH_ADDRESS) {
		var err error
		reconciled := false
		if !r.Opts.UseTraces {
			stmt := trans.FetchStatement(r.Opts.AsEther, base.FAKE_ETH_ADDRESS, r.Opts.AccountFor)
			if reconciled, err = r.trialBalance(pos, trans, stmt); err != nil {
				return nil, err
			} else {
				if reconciled && stmt.IsMaterial() {
					results = append(results, *stmt)
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
					if _, err = r.trialBalance(pos, trans, stmt); err != nil {
						return nil, err
					} else {
						if stmt.IsMaterial() { // append even if not reconciled
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
			for _, stmt := range statements {
				name := r.Names[stmt.Asset]
				stmt.Symbol = stmt.Asset.DefaultSymbol()
				stmt.Decimals = base.Value(18)
				if name.Address == stmt.Asset {
					if name.Symbol != "" {
						stmt.Symbol = name.Symbol
					}
					if name.Decimals != 0 {
						stmt.Decimals = base.Value(name.Decimals)
					}
				}
				if reconciled, err := r.trialBalance(pos, trans, &stmt); err != nil {
					// TODO: Silent fail?
					continue
				} else {
					if reconciled {
						id := fmt.Sprintf(" %d.%d.%d", stmt.BlockNumber, stmt.TransactionIndex, stmt.LogIndex)
						logger.Progress(true, colors.Green+"Transaction", id, "reconciled       "+colors.Off)
					} else {
						if !base.IsTestMode() {
							id := fmt.Sprintf(" %d.%d.%d", stmt.BlockNumber, stmt.TransactionIndex, stmt.LogIndex)
							logger.Warn("Log statement at ", id, " does not reconcile.")
						}
					}

					// order matters - don't move
					if stmt.IsMaterial() && !r.SkipAirdrop(&stmt) { // add even if not reconciled
						results = append(results, stmt)
					}
				}
			}
		}
	}
	return results, nil
}

// GetTransfers returns a statement from a given transaction
func (r *Reconciler1) GetTransfers(pos *types.AppPosition, trans *types.Transaction) ([]types.Transfer, error) {
	var statements []types.Statement

	// if base.IsTestMode() {
	// 	logger.Info(r.String())
	// }

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
			stmt = trans.FetchStatement(r.Opts.AsEther, base.FAKE_ETH_ADDRESS, r.Opts.AccountFor)
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
				if stmt.IsMaterial() && !r.SkipAirdrop(&stmt) {
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
