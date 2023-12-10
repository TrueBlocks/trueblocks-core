package ledger

import (
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/pricing"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// trialBalance returns true of the reconciliation balances, false otherwise. It also prints the trial balance to the console.
func (l *Ledger) trialBalance(msg string, r *types.SimpleStatement) bool {
	key := l.ctxKey(r.BlockNumber, r.TransactionIndex)
	ctx := l.Contexts[key]

	r.ReconciliationType = ctx.ReconType.String()
	if r.AssetAddr == base.FAKE_ETH_ADDRESS {
		if strings.Contains(msg, "TRACE") {
			r.ReconciliationType += "-trace-eth"
		} else {
			r.ReconciliationType += "-eth"
		}
	} else {
		r.ReconciliationType += "-token"
	}

	logger.TestLog(l.TestMode, "Start of trial balance report")

	// TODO: BOGUS PERF
	var okay bool
	if okay = r.Reconciled(); !okay {
		if okay = r.CorrectForNullTransfer(l.theTx); !okay {
			okay = r.CorrectForSomethingElse(l.theTx)
		}
	}

	// TODO: BOGUS PERF
	if okay && r.MoneyMoved() {
		// var err error
		r.SpotPrice, r.PriceSource, _ = pricing.PriceUsd(l.Conn, l.TestMode, r)
		// 	if r.SpotPrice, r.PriceSource, err = pricing.PriceUsd(l.Conn, l.TestMode, r); err != nil {
		// 		logger.Error("Failed to get price for", r.AssetSymbol, "at", r.BlockNumber, r.TransactionIndex)
		// 		logger.Error("Error returned from PriceUsd:", err)
		// 	}
	}

	r.Report(l.TestMode, ctx, msg)

	return r.Reconciled()
}
