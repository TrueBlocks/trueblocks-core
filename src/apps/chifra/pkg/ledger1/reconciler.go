package ledger1

import (
	"encoding/json"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/names"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/pricing"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
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
	opts               *ReconcilerOptions
	names              map[base.Address]types.Name
	enabledCorrections bool
}

func (r *Reconciler1) String() string {
	bytes, _ := json.MarshalIndent(r, "", "  ")
	return string(bytes)
}

func NewReconciler(opts *ReconcilerOptions) *Reconciler1 {
	parts := types.Custom | types.Prefund | types.Regular
	names, _ := names.LoadNamesMap(opts.Connection.Chain, parts, []string{})
	r := &Reconciler1{
		opts:               opts,
		names:              names,
		enabledCorrections: true,
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
