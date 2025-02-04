package ledger

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// appContext represents the reconciliation state for a block.
type appContext struct {
	Asset     base.Address
	PrevBlock base.Blknum
	CurBlock  base.Blknum
	NextBlock base.Blknum
	ReconType types.ReconType
	Reversed  bool
}

// newAppContext creates a appContext instance while preserving the actual block relationships.
func newAppContext(prev, cur, next base.Blknum, isFirst, isLast, reversed bool) *appContext {
	if prev > cur || cur > next {
		return &appContext{ReconType: types.Invalid, Reversed: reversed}
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

	return &appContext{
		PrevBlock: prev,
		CurBlock:  cur,
		NextBlock: next,
		ReconType: reconType,
		Reversed:  reversed,
	}
}

// Prev returns the block number of the previous block in the reconciliation sequence.
func (c *appContext) Prev() base.Blknum {
	return c.PrevBlock
}

// Cur returns the block number of the current block being processed.
func (c *appContext) Cur() base.Blknum {
	return c.CurBlock
}

// Next returns the block number of the next block in the reconciliation sequence.
func (c *appContext) Next() base.Blknum {
	return c.NextBlock
}

// Recon returns the block number of the next block in the reconciliation sequence.
func (c *appContext) Recon() types.ReconType {
	return c.ReconType
}

// Asset returns the block number of the next block in the reconciliation sequence.
func (c *appContext) Address() base.Address {
	return c.Asset
}

type assetContext appContext

// newAssetContext creates a assetContext instance while preserving the actual block relationships.
func newAssetContext(prev, cur, next base.Blknum, isFirst, isLast, reversed bool, asset base.Address) *assetContext {
	appCtx := newAppContext(prev, cur, next, isFirst, isLast, reversed)
	return &assetContext{
		Asset:     asset,
		PrevBlock: appCtx.PrevBlock,
		CurBlock:  appCtx.CurBlock,
		NextBlock: appCtx.NextBlock,
		ReconType: appCtx.ReconType,
		Reversed:  appCtx.Reversed,
	}
}

// Prev returns the block number of the previous block in the reconciliation sequence.
func (c *assetContext) Prev() base.Blknum {
	return c.PrevBlock
}

// Cur returns the block number of the current block being processed.
func (c *assetContext) Cur() base.Blknum {
	return c.CurBlock
}

// Next returns the block number of the next block in the reconciliation sequence.
func (c *assetContext) Next() base.Blknum {
	return c.NextBlock
}

// Recon returns the block number of the next block in the reconciliation sequence.
func (c *assetContext) Recon() types.ReconType {
	return c.ReconType
}

// Asset returns the block number of the next block in the reconciliation sequence.
func (c *assetContext) Address() base.Address {
	return c.Asset
}
