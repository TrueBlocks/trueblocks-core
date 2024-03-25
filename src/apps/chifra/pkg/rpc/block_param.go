package rpc

import (
	"math/big"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

// BlockParam encapsulates the big.Int needed to call in to the Go Ethereum packages
type BlockParam = big.Int

// NewBlockParam creates a new BlockParam
func NewBlockParam(x int64) *BlockParam {
	return (*BlockParam)(big.NewInt(x))
}

func bnFromUint64(bn uint64) *big.Int {
	if bn == utils.NOPOS {
		return nil
	}
	return new(big.Int).SetUint64(bn)
}
