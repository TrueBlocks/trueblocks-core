package tslib

import (
	"errors"
	"fmt"
)

// FromBnToName returns the block's chain-specific name (if found) given its block number
func FromBnToName(chain string, bn uint64) (string, error) {
	specials, _ := GetSpecials(chain)
	for _, value := range specials {
		if value.BlockNumber == bn {
			return value.Name, nil
		}
	}
	msg := fmt.Sprintf("Block number %d is not special", bn)
	return "", errors.New(msg)
}
