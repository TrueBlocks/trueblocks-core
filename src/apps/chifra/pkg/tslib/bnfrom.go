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

// BnFromDate returns a chain-specific block number given a date string (date strings are valid JSON dates). Snap too, which may be
// empty, will push the date back to the next nearest period if provided. Period can be hourly, daily, weekly, monthly, or annually
func BnFromDate(chain, date, period string) (uint64, error) {
	ts, err := TsFromDate(date)
	if err != nil {
		return 0, err
	}
	ret, err := fromTs(chain, ts)
	return uint64(ret.Bn), err
}

// BnFromName returns the chain-specific block number (if found) given the name of a special block. The list of special blocks is per-chain.
func BnFromName(chain, name string) (uint64, error) {
	if name == "latest" {
		meta, _ := rpcClient.GetMetaData(chain, false)
		return meta.Latest, nil
	}

	specials, _ := GetSpecials(chain)
	for _, value := range specials {
		if value.Name == name {
			return value.BlockNumber, nil
		}
	}

	return uint64(utils.NOPOS), errors.New("Block " + name + " not found")
}

// BnFromTs returns a chain-specific block number given a Linux timestamp.
func BnFromTs(chain string, ts uint64) (uint64, error) {
	ret, err := fromTs(chain, ts)
	return uint64(ret.Bn), err
}
