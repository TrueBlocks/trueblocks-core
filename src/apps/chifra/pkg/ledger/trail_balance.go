package ledger

import (
	"fmt"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/pricing"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// trialBalance returns true of the reconciliation balances, false otherwise. If the statement
// does not reconcile, it tries to repair it in two ways (a) for null transfers and (b) for
// any other reason. If that works and the statement is material (money moved in some way), the
// function tries to price the asset. it then prints optional debugging information. Note that
// the statement may be modified in this function.
func (l *Ledger) trialBalance(reason types.TrialBalType, s *types.Statement) bool {
	key := l.getAppBalancerKey(s.BlockNumber, s.TransactionIndex)
	if ctx, exists := l.appBalancers[key]; !exists {
		logger.Fatal(fmt.Sprintf("should never happen - no context for %s", key))

	} else {
		logger.TestLog(l.testMode, "Start of trial balance report")

		s.PostFirst = ctx.first
		s.PostLast = ctx.last

		s.BlockNumberPrev = ctx.Prev()
		s.BlockNumberNext = ctx.Next()
		s.AssetType = reason

		var okay bool
		if okay = s.Reconciled(); !okay {
			if okay = l.correctForNullTransfer(s, l.theTx); !okay {
				if s.IsEth() {
					_ = l.correctForSomethingElseEth(s)
				} else {
					_ = l.correctForSomethingElseToken(s)
				}
			}
		}

		if s.IsMaterial() {
			s.SpotPrice, s.PriceSource, _ = pricing.PriceUsd(l.connection, s)
		}

		if l.testMode {
			s.DebugStatement(ctx.Prev(), ctx.Next())
		}
	}

	return s.Reconciled()
}

func isNullTransfer(s *types.Statement, tx *types.Transaction) bool {
	lotsOfLogs := len(tx.Receipt.Logs) > 10
	mayBeAirdrop := s.Sender.IsZero() || s.Sender == tx.To
	noBalanceChange := s.EndBal.Cmp(&s.BegBal) == 0 && s.IsMaterial()
	ret := (lotsOfLogs || mayBeAirdrop) && noBalanceChange

	logger.TestLog(true, "A possible nullTransfer")
	logger.TestLog(true, "  nLogs:            ", len(tx.Receipt.Logs))
	logger.TestLog(true, "    lotsOfLogs:      -->", lotsOfLogs)

	logger.TestLog(true, "  Sender.IsZero:    ", s.Sender, s.Sender.IsZero())
	logger.TestLog(true, "  or Sender == To:  ", s.Sender == tx.To)
	logger.TestLog(true, "    mayBeAirdrop:    -->", mayBeAirdrop)

	logger.TestLog(true, "  EndBal-BegBal:    ", s.EndBal.Cmp(&s.BegBal))
	logger.TestLog(true, "  Material:         ", s.IsMaterial())
	logger.TestLog(true, "    noBalanceChange: -->", noBalanceChange)

	if !ret {
		logger.TestLog(true, "  ---> Not a nullTransfer")
	}
	return ret
}

func (l *Ledger) correctForNullTransfer(s *types.Statement, tx *types.Transaction) bool {
	if s.IsEth() {
		return s.Reconciled()
	}

	if isNullTransfer(s, tx) {
		logger.TestLog(true, "Correcting token transfer for a null transfer")
		amt := s.TotalIn() // use totalIn since this is the amount that was faked
		s.AmountOut = *new(base.Wei)
		s.AmountIn = *new(base.Wei)
		s.CorrectingIn = *amt
		s.CorrectingOut = *amt
		s.CorrectingReason = "null-transfer"
	} else {
		logger.TestLog(true, "Needs correction for token transfer")
	}

	return s.Reconciled()
}

func (l *Ledger) correctForSomethingElseEth(s *types.Statement) bool {
	if s.AssetType == types.TrialBalTraceEth && s.PostFirst && s.PostLast {
		if s.EndBalCalc().Cmp(&s.EndBal) != 0 {
			s.EndBal = *s.EndBalCalc()
			s.CorrectingReason = "per-block-balance"
		}
	}
	return s.Reconciled()
}

func (l *Ledger) correctForSomethingElseToken(s *types.Statement) bool {
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
