package ledger

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

type assetContext appContext

func newAssetContext(prev, cur, next base.Blknum, isFirst, isLast, reversed bool, addr base.Address) *assetContext {
	appCtx := newAppContext(prev, cur, next, isFirst, isLast, reversed)
	return &assetContext{
		address:   addr,
		prvBlk:    appCtx.Prev(),
		curBlk:    appCtx.Cur(),
		nxtBlk:    appCtx.Next(),
		reconType: appCtx.Recon(),
		reversed:  appCtx.reversed,
	}
}

func (c *assetContext) Prev() base.Blknum {
	return c.prvBlk
}

func (c *assetContext) Cur() base.Blknum {
	return c.curBlk
}

func (c *assetContext) Next() base.Blknum {
	return c.nxtBlk
}

func (c *assetContext) Recon() types.ReconType {
	return c.reconType
}

func (c *assetContext) Address() base.Address {
	return c.address
}
