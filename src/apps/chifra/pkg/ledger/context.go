package ledger

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// ledgerContext represents the reconciliation state for a block.
type ledgerContext struct {
	PrevBlock base.Blknum
	CurBlock  base.Blknum
	NextBlock base.Blknum
	ReconType types.ReconType
	Reversed  bool
}

// newLedgerContext creates a ledgerContext instance while preserving the actual block relationships.
func newLedgerContext(prev, cur, next base.Blknum, isFirst, isLast, reversed bool) *ledgerContext {
	if prev > cur || cur > next {
		return &ledgerContext{ReconType: types.Invalid, Reversed: reversed}
	}

	reconType := types.Invalid
	if cur == 0 {
		reconType = types.Genesis
	} else {
		prevDiff := prev != cur
		nextDiff := cur != next

		if prevDiff && nextDiff {
			reconType = types.DiffDiff
		} else if !prevDiff && !nextDiff {
			reconType = types.SameSame
		} else if prevDiff {
			reconType = types.DiffSame
		} else if nextDiff {
			reconType = types.SameDiff
		} else {
			reconType = types.Invalid
			logger.Panic("should not happen")
		}
	}

	if isFirst {
		reconType |= types.First
	}

	if isLast {
		reconType |= types.Last
	}

	return &ledgerContext{
		PrevBlock: prev,
		CurBlock:  cur,
		NextBlock: next,
		ReconType: reconType,
		Reversed:  reversed,
	}
}

// Prev returns the block number of the previous block in the reconciliation sequence.
func (c *ledgerContext) Prev() base.Blknum {
	return c.PrevBlock
}

// Cur returns the block number of the current block being processed.
func (c *ledgerContext) Cur() base.Blknum {
	return c.CurBlock
}

// Next returns the block number of the next block in the reconciliation sequence.
func (c *ledgerContext) Next() base.Blknum {
	return c.NextBlock
}
