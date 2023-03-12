package tslib

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/bykof/gostradamus"
)

// FromBnToDate returns a chain-specific date given a block number.
func FromBnToDate(chain string, bn uint64) (gostradamus.DateTime, error) {
	ts, err := FromBnToTs(chain, bn)
	if err != nil {
		dt, _ := FromTsToDate(utils.EarliestEvmTs)
		return dt, err
	}
	return FromTsToDate(ts)
}

// FromNameToDate returns a chain-specific date (if found) given its chain-specific name
func FromNameToDate(chain, name string) (gostradamus.DateTime, error) {
	specials, _ := GetSpecials(chain)
	if len(specials) == 0 || name == "0" {
		return FromBnToDate(chain, 0)
	}

	if name == "latest" {
		meta, _ := rpcClient.GetMetaData(chain, false)
		ts := rpcClient.GetBlockTimestamp(chain, meta.Latest)
		return FromTsToDate(ts)
	}

	for _, value := range specials {
		if value.Name == name {
			return FromBnToDate(chain, value.BlockNumber)
		}
	}

	// default to first
	return gostradamus.Parse(specials[0].Date, gostradamus.Iso8601)
}

// FromTsToDate returns a date given a Linux timestamp (not chain-specific)
func FromTsToDate(ts types.Timestamp) (gostradamus.DateTime, error) {
	return gostradamus.FromUnixTimestamp(ts), nil
}
