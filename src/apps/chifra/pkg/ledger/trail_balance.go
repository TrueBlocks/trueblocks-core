package ledger

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/pricing"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// trialBalance returns true of the reconciliation balances, false otherwise. It also prints the trial balance to the console.
func (l *Ledger) trialBalance(reason string, s *types.Statement) bool {
	key := l.getAssetContextKey(s.BlockNumber, s.TransactionIndex, s.AssetAddr)
	var ctx *assetContext
	var exists bool
	if ctx, exists = l.assetContexts[key]; !exists {
		return s.Reconciled()
	}

	s.ReconType = ctx.Recon()
	s.AssetType = reason

	logger.TestLog(l.testMode, "Start of trial balance report")

	// TODO: BOGUS PERF
	var okay bool
	if okay = s.Reconciled(); !okay {
		if okay = s.CorrectForNullTransfer(l.theTx); !okay {
			if s.IsEth() {
				_ = s.CorrectForSomethingElseEth(l.theTx)
			} else {
				_ = s.CorrectForSomethingElseToken(l.theTx)
			}
		}
	}

	// TODO: BOGUS PERF
	if s.IsMaterial() {
		s.SpotPrice, s.PriceSource, _ = pricing.PriceUsd(l.connection, s)
	}

	if l.testMode {
		s.DebugStatement(ctx)
	}

	return s.Reconciled()
}
