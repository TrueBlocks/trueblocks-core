// Package `tslibPkg` provides conversions between four entities: block numbers, dates, Linux
// timestamps, and special named blocks. The function names are self-explanitory.
//
// Note: because the relationship between block numbers and dates or timestamps is a per-chain
// value, some of these functions require a `chain` name. Any chain name is permissable, but
// they all require proper configuration in the TrueBlocks config files.
package tslibPkg

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
