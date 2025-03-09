package ledger1

import (
	"encoding/json"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/ledger10"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/names"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/pricing"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

type Reconciler1 struct {
	opts               *ledger10.ReconcilerOptions
	names              map[base.Address]types.Name
	hasStartBlock      bool
	enabledCorrections bool
	transfers          map[blockTxKey][]ledger10.AssetTransfer
	accountLedger      map[assetHolderKey]base.Wei
	ledgerAssets       map[base.Address]bool
	correctionCounter  base.Value
	entryCounter       base.Value
	ledgers            map[base.Address]Ledger
}

func (r *Reconciler1) String() string {
	bytes, _ := json.MarshalIndent(r, "", "  ")
	return string(bytes)
}

func NewReconciler(opts *ledger10.ReconcilerOptions) *Reconciler1 {
	parts := types.Custom | types.Prefund | types.Regular
	names, _ := names.LoadNamesMap(opts.Connection.Chain, parts, []string{})
	r := &Reconciler1{
		opts:               opts,
		names:              names,
		hasStartBlock:      false,
		enabledCorrections: true,
		transfers:          make(map[blockTxKey][]ledger10.AssetTransfer),
		accountLedger:      make(map[assetHolderKey]base.Wei),
		ledgerAssets:       make(map[base.Address]bool),
		ledgers:            make(map[base.Address]Ledger),
	}
	_ = r.correctionCounter
	_ = r.entryCounter
	return r
}

type Ledger struct{}

type blockTxKey struct {
	BlockNumber      base.Blknum
	TransactionIndex base.Txnum
}

type assetHolderKey struct {
	Asset  base.Address
	Holder base.Address
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

	var okay bool
	if okay = s.Reconciled(); !okay {
		if !s.IsEth() {
			if okay = r.correctForNullTransfer(s, trans); !okay {
				_ = r.correctForSomethingElseToken(s)
			}
		}
	}

	if s.IsMaterial() {
		s.SpotPrice, s.PriceSource, _ = pricing.PriceUsd(r.opts.Connection, s)
		s.DebugStatement(pos)
	}

	return s.Reconciled(), nil
}

func (r *Reconciler1) correctForSomethingElseToken(s *types.Statement) bool {
	if !r.enabledCorrections {
		return true
	}

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
	if !r.enabledCorrections {
		return true
	}

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
