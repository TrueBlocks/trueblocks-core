package ledger

import (
	"fmt"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/pricing"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// TrialBalance returns true of the reconciliation balances, false otherwise. It also prints the trial balance to the console.
func (l *Ledger) TrialBalance(msg string, r *types.SimpleStatement) bool {
	key := fmt.Sprintf("%09d-%05d", r.BlockNumber, r.TransactionIndex)
	ctx := l.Contexts[key]
	r.ReconciliationType = ctx.ReconType
	if r.AssetAddr == base.FAKE_ETH_ADDRESS {
		if strings.Contains(msg, "TRACE") {
			r.ReconciliationType = "trace-eth"
		} else {
			r.ReconciliationType += "-eth"
		}
	} else {
		r.ReconciliationType += "-token"
	}

	if l.TestMode {
		logger.TestLog(l.TestMode, "Start of trial balance report")
	}

	okay := r.Reconciled()
	if !okay {
		if okay = r.CorrectForNullTransfer(l.Tx); !okay {
			okay = r.CorrectForSomethingElse(l.Tx)
		}
	}

	if okay && r.MoneyMoved() {
		var err error
		if r.SpotPrice, r.PriceSource, err = pricing.PriceUsd(l.Chain, l.TestMode, r); err != nil {
			logger.Error("Failed to get price for", r.AssetSymbol, "at", r.BlockNumber, r.TransactionIndex)
			logger.Error("Error returned from PriceUsd:", err)
		}
	}

	if l.TestMode {
		Report(r, ctx, msg)
		logger.TestLog(l.TestMode, "End of trial balance report")
	}

	return r.Reconciled()
}
