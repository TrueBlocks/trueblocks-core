// Package `tslibPkg` provides conversions between four entities: block numbers, dates, Linux
// timestamps, and special named blocks. The function names are self-explanitory.
//
// Note: because the relationship between block numbers and dates or timestamps is a per-chain
// value, some of these functions require a `chain` name. Any chain name is permissable, but
// they all require proper configuration in the TrueBlocks config files.
package tslibPkg

import (
	"errors"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

// FromDateToBn returns a chain-specific block number given a date string (date strings are valid JSON dates).
func FromDateToBn(chain, dateStr string) (uint64, error) {
	ts, err := FromDateToTs(dateStr)
	if err != nil {
		return 0, err
	}
	ret, err := FromTs(chain, ts)
	if err != nil {
		return 0, err
	}
	return uint64(ret.Bn), err
}

// FromNameToBn returns the chain-specific block number (if found) given the name of a special block. The list of special blocks is per-chain.
func FromNameToBn(chain, name string) (uint64, error) {
	if name == "latest" {
		meta, err := rpcClient.GetMetaData(chain, false)
		if err != nil {
			return 0, err
		}
		return meta.Latest, nil
	}

	specials, _ := GetSpecials(chain) // it's okay if it's empty
	for _, value := range specials {
		if value.Name == name {
			return value.BlockNumber, nil
		}
	}

	return uint64(utils.NOPOS), errors.New("Block " + name + " not found")
}

// FromTsToBn returns a chain-specific block number given a Linux timestamp.
func FromTsToBn(chain string, ts uint64) (uint64, error) {
	ret, err := FromTs(chain, ts)
	if err != nil {
		return 0, err
	}
	return uint64(ret.Bn), err
}
