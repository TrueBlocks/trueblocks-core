package ledger

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// assetBalancer represents the context for a specific asset within a transaction. It is defined
// as an alias to appBalancer and carries the same block boundary and reconciliation information,
// but it is used to track balance changes and other details at the asset level.
type assetBalancer appBalancer

func newAssetContext(prev, cur, next base.Blknum, isFirst, isLast, reversed bool, addr base.Address) *assetBalancer {
	appCtx := newAppContext(prev, cur, next, isFirst, isLast, reversed)
	return &assetBalancer{
		address:   addr,
		prvBlk:    appCtx.Prev(),
		curBlk:    appCtx.Cur(),
		nxtBlk:    appCtx.Next(),
		reconType: appCtx.Recon(),
		reversed:  appCtx.reversed,
	}
}

func (c *assetBalancer) Prev() base.Blknum {
	if c.reconType&types.First != 0 {
		if c.prvBlk == 0 {
			return 0
		}
		return c.prvBlk - 1
	}
	return c.prvBlk
}

func (c *assetBalancer) Cur() base.Blknum {
	return c.curBlk
}

func (c *assetBalancer) Next() base.Blknum {
	if c.reconType&types.Last != 0 {
		return c.nxtBlk + 1
	}
	return c.nxtBlk
}

func (c *assetBalancer) Recon() types.ReconType {
	return c.reconType
}

func (c *assetBalancer) Address() base.Address {
	return c.address
}

func (c *assetBalancer) RunningBal() *base.Wei {
	return nil
}
