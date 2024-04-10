package provider

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

type SlurpedTransaction struct {
	Address     *base.Address
	Transaction *types.SimpleSlurp
}

type Provider interface {
	TransactionsByAddress(address base.Address) ([]types.SimpleSlurp, error)
	Appearances(address base.Address) ([]types.SimpleSlurp, error)
	Count(address base.Address) (*types.SimpleSlurpCount, error)
}
