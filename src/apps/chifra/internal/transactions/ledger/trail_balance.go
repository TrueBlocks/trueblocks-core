package ledger

import (
	"fmt"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/transactions/pricing"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
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

	if r.Reconciled() {
		var err error
		if r.SpotPrice, r.PriceSource, err = pricing.PriceUsd(l.Chain, l.TestMode, r); err != nil {
			logger.Error("Failed to get price for", r.AssetSymbol, "at", r.BlockNumber, r.TransactionIndex, ":", err)
		}
	}

	if l.TestMode {
		Report(r, ctx, msg)
	}

	return r.Reconciled()
}
