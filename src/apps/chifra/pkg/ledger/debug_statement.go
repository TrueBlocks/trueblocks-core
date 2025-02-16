package ledger

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

type Balancer interface {
	Prev() base.Blknum
	Cur() base.Blknum
	Next() base.Blknum
	Recon() types.ReconType
	Address() base.Address
	RunningBal() *base.Wei
}
