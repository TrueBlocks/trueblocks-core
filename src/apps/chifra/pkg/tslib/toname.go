package tslib

import (
	"errors"
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/types"
)

// FromBnToName returns the block's chain-specific name (if found) given its block number
func FromBnToName(chain string, bn base.Blknum) (string, error) {
	nb, err := FromBnToNamedBlock(chain, bn)
	if err != nil || nb == nil {
		return "", err
	}
	return nb.Name, nil
}

// FromBnToNamedBlock returns the block's chain-specific name (if found) given its block number
func FromBnToNamedBlock(chain string, bn base.Blknum) (*types.NamedBlock, error) {
	specials, _ := GetSpecials(chain)
	for _, value := range specials {
		if value.BlockNumber == bn {
			return &value, nil
		}
	}
	msg := fmt.Sprintf("Block number %d is not special", bn)
	return nil, errors.New(msg)
}
