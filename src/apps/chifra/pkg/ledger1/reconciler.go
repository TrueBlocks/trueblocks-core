package ledger1

import (
	"encoding/json"
	"fmt"
	"slices"
	"sort"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/names"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/normalize"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/pricing"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/topics"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

type ReconcilerOptions struct {
	Connection   *rpc.Connection
	AccountFor   base.Address
	FirstBlock   base.Blknum
	LastBlock    base.Blknum
	AsEther      bool
	UseTraces    bool
	Reversed     bool
	AssetFilters []base.Address
	AppFilters   *types.AppearanceFilter
}

type Reconciler1 struct {
	opts           *ReconcilerOptions
	Names          map[base.Address]types.Name
	AddCorrections bool
	ShowDebugging  bool
	RemoveAirdrops bool
}

func (r *Reconciler1) String() string {
	bytes, _ := json.MarshalIndent(r, "", "  ")
	return string(bytes)
}

func NewReconciler(opts *ReconcilerOptions) *Reconciler1 {
	parts := types.Custom | types.Prefund | types.Regular
	names, _ := names.LoadNamesMap("mainnet", parts, []string{})
	r := &Reconciler1{
		opts:           opts,
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
	if s.PrevBal, s.BegBal, s.EndBal, err = r.opts.Connection.GetReconBalances(&rpc.BalanceOptions{
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

	s.SpotPrice, s.PriceSource, _ = pricing.PriceUsd(r.opts.Connection, s)
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
	if AssetOfInterest(r.opts.AssetFilters, base.FAKE_ETH_ADDRESS) {
		var err error
		reconciled := false
		if !r.opts.UseTraces {
			stmt := trans.StatementFromTransaction(r.opts.AsEther, base.FAKE_ETH_ADDRESS, r.opts.AccountFor)
			if reconciled, err = r.trialBalance(pos, trans, stmt); err != nil {
				return nil, err
			} else {
				if reconciled && stmt.IsMaterial() {
					results = append(results, *stmt)
				}
			}
		}

		if r.opts.UseTraces || !reconciled {
			if stmt, err := r.getStatementFromTraces(trans); err != nil {
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

	if trans.Receipt != nil {
		if statements, err := r.getStatementsFromLogs(trans.Receipt.Logs); err != nil {
			return nil, err
		} else {
			for _, stmt := range statements {
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

func (r *Reconciler1) getStatementFromTraces(trans *types.Transaction) (*types.Statement, error) {
	if traces, err := r.opts.Connection.GetTracesByTransactionHash(trans.Hash.Hex(), trans); err != nil {
		return nil, err

	} else {
		stmt := trans.StatementFromTransaction(r.opts.AsEther, base.FAKE_ETH_ADDRESS, r.opts.AccountFor)
		for i, trace := range traces {
			if i == 0 {
				// the first trace is identical to the transaction itself, so we can skip it
				continue
			}

			if trace.Action.CallType == "delegatecall" && trace.Action.To != stmt.AccountedFor {
				// delegate calls are not included in the transaction's gas cost, so we skip them
				continue
			}

			plusEq := func(a1, a2 *base.Wei) base.Wei {
				return *a1.Add(a1, a2)
			}

			// Do not collapse, more than one of these can be true at the same time
			if trace.Action.From == stmt.AccountedFor {
				stmt.InternalOut = plusEq(&stmt.InternalOut, &trace.Action.Value)
				stmt.Sender = trace.Action.From
				if trace.Action.To.IsZero() {
					if trace.Result != nil {
						stmt.Recipient = trace.Result.Address
					}
				} else {
					stmt.Recipient = trace.Action.To
				}
			}

			if trace.Action.To == stmt.AccountedFor {
				stmt.InternalIn = plusEq(&stmt.InternalIn, &trace.Action.Value)
				stmt.Sender = trace.Action.From
				stmt.Recipient = trace.Action.To
			}

			if trace.Action.SelfDestructed == stmt.AccountedFor {
				stmt.SelfDestructOut = plusEq(&stmt.SelfDestructOut, &trace.Action.Balance)
				stmt.Sender = trace.Action.SelfDestructed
				if stmt.Sender.IsZero() {
					stmt.Sender = trace.Action.Address
				}
				stmt.Recipient = trace.Action.RefundAddress
			}

			if trace.Action.RefundAddress == stmt.AccountedFor {
				stmt.SelfDestructIn = plusEq(&stmt.SelfDestructIn, &trace.Action.Balance)
				stmt.Sender = trace.Action.SelfDestructed
				if stmt.Sender.IsZero() {
					stmt.Sender = trace.Action.Address
				}
				stmt.Recipient = trace.Action.RefundAddress
			}

			if trace.Action.Address == stmt.AccountedFor && !trace.Action.RefundAddress.IsZero() {
				stmt.SelfDestructOut = plusEq(&stmt.SelfDestructOut, &trace.Action.Balance)
				// self destructed send
				stmt.Sender = trace.Action.Address
				stmt.Recipient = trace.Action.RefundAddress
			}

			if trace.Result != nil {
				if trace.Result.Address == stmt.AccountedFor {
					stmt.InternalIn = plusEq(&stmt.InternalIn, &trace.Action.Value)
					stmt.Sender = trace.Action.From
					stmt.Recipient = trace.Result.Address
				}
			}
		}
		return stmt, nil
	}
}

func (r *Reconciler1) getStatementsFromLogs(logs []types.Log) ([]types.Statement, error) {
	statements := make([]types.Statement, 0, 20)
	for _, log := range logs {
		isTransfer := log.Topics[0] == topics.TransferTopic
		isOfIterest := AssetOfInterest(r.opts.AssetFilters, log.Address)
		passesFilter := r.opts.AppFilters.ApplyLogFilter(&log, []base.Address{r.opts.AccountFor})
		if isTransfer && isOfIterest && passesFilter {
			if stmt, err := r.getStatementFromLog(&log); err != nil {
				// TODO: silent fail?
				continue
			} else if stmt == nil {
				continue
			} else {
				statements = append(statements, *stmt)
			}
		}
	}
	return statements, nil
}

func (r *Reconciler1) getStatementFromLog(log *types.Log) (*types.Statement, error) {
	if normalized, err := normalize.NormalizeKnownLogs(log); err != nil {
		return nil, err
	} else if normalized.IsNFT() {
		return nil, nil
	} else {
		sender := base.HexToAddress(normalized.Topics[1].Hex())
		recipient := base.HexToAddress(normalized.Topics[2].Hex())
		isSender, isRecipient := r.opts.AccountFor == sender, r.opts.AccountFor == recipient
		if !isSender && !isRecipient {
			return nil, nil
		}

		var amountIn, amountOut base.Wei
		amount, _ := new(base.Wei).SetString(strings.ReplaceAll(normalized.Data, "0x", ""), 16)
		if amount == nil {
			amount = base.ZeroWei
		}
		if r.opts.AccountFor == sender {
			amountOut = *amount
		}
		if r.opts.AccountFor == recipient {
			amountIn = *amount
		}
		stmt := &types.Statement{
			AccountedFor:     r.opts.AccountFor,
			Sender:           sender,
			Recipient:        recipient,
			BlockNumber:      normalized.BlockNumber,
			TransactionIndex: normalized.TransactionIndex,
			LogIndex:         normalized.LogIndex,
			TransactionHash:  normalized.TransactionHash,
			Timestamp:        normalized.Timestamp,
			Asset:            normalized.Address,
			PriceSource:      "not-priced",
			AmountIn:         amountIn,
			AmountOut:        amountOut,
		}

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
		return stmt, nil
	}
}

// ---------------------------------------------------------
func AssetOfInterest(filters []base.Address, needle base.Address) bool {
	if len(filters) == 0 {
		return true
	}
	return slices.ContainsFunc(filters, func(asset base.Address) bool {
		return asset.Hex() == needle.Hex()
	})
}

// GetTransfers returns a statement from a given transaction
func (r *Reconciler1) GetTransfers(pos *types.AppPosition, trans *types.Transaction) ([]types.Transfer, error) {
	r.AddCorrections = false
	r.ShowDebugging = false
	if statements, err := r.GetStatements(pos, trans); err != nil {
		return nil, err
	} else {
		return convertToTransfers(statements)
	}
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
		transfers = append(transfers, t)
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
			logger.TestLog(true, "transfer:", t.BlockNumber, t.TransactionIndex, t.LogIndex, t.Asset, t.Holder, t.Amount.Text(10), t.CorrectingReason)
		}
	}

	return transfers, nil
}
