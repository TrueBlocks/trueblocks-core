package ledger

import (
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/pricing"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// trialBalance returns true of the reconciliation balances, false otherwise. It also prints the trial balance to the console.
func (l *Ledger) trialBalance(msg string, s *types.SimpleStatement) bool {
	key := l.ctxKey(s.BlockNumber, s.TransactionIndex)
	ctx := l.Contexts[key]

	rT := ctx.ReconType.String()
	if s.AssetAddr == base.FAKE_ETH_ADDRESS {
		if strings.Contains(msg, "TRACE") {
			rT += "-trace-eth"
		} else {
			rT += "-eth"
		}
	} else {
		rT += "-token"
	}

	logger.TestLog(l.TestMode, "Start of trial balance report")

	// TODO: BOGUS PERF
	var okay bool
	if okay = s.Reconciled(); !okay {
		if okay = s.CorrectForNullTransfer(l.theTx); !okay {
			okay = s.CorrectForSomethingElse(l.theTx)
		}
	}

	// TODO: BOGUS PERF
	if okay && s.MoneyMoved() {
		// var err error
		s.SpotPrice, s.PriceSource, _ = pricing.PriceUsd(l.Conn, l.TestMode, s)
		// 	if s.SpotPrice, s.PriceSource, err = pricing.PriceUsd(l.Conn, l.TestMode, s); err != nil {
		// 		logger.Error("Failed to get price for", s.AssetSymbol, "at", s.BlockNumber, s.TransactionIndex)
		// 		logger.Error("Error returned from PriceUsd:", err)
		// 	}
	}

	s.Report(l.TestMode, rT, ctx, msg)

	return s.Reconciled()
}
