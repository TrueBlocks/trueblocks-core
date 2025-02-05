package ledger

import (
	"fmt"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/pricing"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

type trialBalType string

const (
	trialBalEth      trialBalType = "eth"
	trialBalTraceEth trialBalType = "trace-eth"
	trialBalToken    trialBalType = "token"
)

// trialBalance returns true of the reconciliation balances, false otherwise. If the statement
// does not reconcile, it tries to repair it in two ways (a) for null transfers and (b) for
// any other reason. If that works and the statement is material (money moved in some way), the
// function tries to price the asset. it then prints optional debugging information. Note that
// the statement may be modified in this function.
func (l *Ledger) trialBalance(reason trialBalType, s *types.Statement) bool {
	key := l.getAssetContextKey(s.BlockNumber, s.TransactionIndex, s.AssetAddr)
	if ctx, exists := l.assetContexts[key]; !exists {
		logger.Fatal(fmt.Sprintf("should never happen - no context for %s", key))

	} else {
		logger.TestLog(l.testMode, "Start of trial balance report")

		s.ReconType = ctx.Recon()
		s.AssetType = string(reason)

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
			s.DebugStatement(ctx)
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
	logger.TestLog(true, "  material:         ", s.IsMaterial())
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
	if s.AssetType == "trace-eth" && s.ReconType&types.First != 0 && s.ReconType&types.Last != 0 {
		if s.EndBalCalc().Cmp(&s.EndBal) != 0 {
			s.EndBal = *s.EndBalCalc()
			s.CorrectingReason = "per-block-balance"
		}
	} else {
		logger.TestLog(true, "Needs correction for eth")
	}
	return s.Reconciled()
}

func (l *Ledger) correctForSomethingElseToken(s *types.Statement) bool {
	logger.TestLog(true, "Correcting token transfer for unknown income or outflow")

	// We assume that the discrepancy is due to an error in the beginning balance.
	// Compute the difference at the beginning:
	deltaBeg := new(base.Wei).Sub(&s.BegBal, &s.PrevBal)
	// Compute the computed ending balance:
	endCalc := s.EndBalCalc()
	// Compute the difference at the end:
	deltaEnd := new(base.Wei).Sub(endCalc, &s.EndBal)

	// For reconciliation, we want both deltaBeg and deltaEnd to be zero.
	// One simple approach is to assume that the correcting entry should cancel the beginning discrepancy.
	// That is, if BegBal is too high relative to PrevBal (deltaBeg positive),
	// we record a correcting entry (as a debit) equal to deltaBeg.

	// Reset correcting fields.
	s.CorrectingIn.SetUint64(0)
	s.CorrectingOut.SetUint64(0)
	s.CorrectingReason = ""

	// If the beginning balance is too high, record a debit correction.
	if deltaBeg.Cmp(new(base.Wei).SetInt64(0)) > 0 {
		s.CorrectingIn = *deltaBeg
		s.CorrectingReason = "begbal"
	} else if deltaBeg.Cmp(new(base.Wei).SetInt64(0)) < 0 {
		s.CorrectingOut = *deltaBeg
		s.CorrectingReason = "begbal"
	}

	// Now adjust the ending balance so that the computed ending balance matches.
	// We want the effective ending balance (EndBal plus correction) to equal EndBalCalc.
	// Let’s define the net correction as the sum of the beginning and ending discrepancies.
	// One option is to add the ending discrepancy to the correcting entry.
	if deltaEnd.Cmp(new(base.Wei).SetInt64(0)) > 0 {
		n := new(base.Wei).Add(&s.CorrectingIn, deltaEnd)
		s.CorrectingIn = *n
		s.CorrectingReason += "endbal"
	} else if deltaEnd.Cmp(new(base.Wei).SetInt64(0)) < 0 {
		n := new(base.Wei).Add(&s.CorrectingOut, deltaEnd)
		s.CorrectingOut = *n
		s.CorrectingReason += "endbal"
	}

	// Finally, adjust EndBal so that the effective ending balance is correct.
	// One way is to subtract the net correcting entry from EndBalCalc.
	// (This assumes that the correction is recorded on the beginning side.)
	// In other words, set EndBal = EndBalCalc - (net correcting entry).
	var netCorrection *base.Wei
	if s.CorrectingIn.Cmp(new(base.Wei).SetInt64(0)) != 0 {
		netCorrection = &s.CorrectingIn
	} else {
		netCorrection = &s.CorrectingOut
	}

	adjustedEndBal := new(base.Wei).Sub(endCalc, netCorrection)
	s.EndBal = *adjustedEndBal

	// For logging clarity, also replace the reason string formatting.
	s.CorrectingReason = strings.Replace(s.CorrectingReason, "begbalendbal", "begbal-endbal", -1)

	return s.Reconciled() // Now s.Reconciled() should be true.
}
