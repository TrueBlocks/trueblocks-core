package ledger

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
)

func newAssetBalancer(prev, cur, next base.Blknum, isFirst, isLast, reversed bool, addr base.Address) *appBalancer {
	appBal := newAppBalancer(prev, cur, next, isFirst, isLast, reversed)
	return &appBalancer{
		address:   addr,
		prvBlk:    appBal.Prev(),
		curBlk:    appBal.Cur(),
		nxtBlk:    appBal.Next(),
		reconType: appBal.Recon(),
		reversed:  appBal.reversed,
	}
}
