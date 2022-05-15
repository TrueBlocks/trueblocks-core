// Package `tslibPkg` provides conversions between four entities: block numbers, dates, Linux
// timestamps, and special named blocks. The function names are self-explanitory.
//
// Note: because the relationship between block numbers and dates or timestamps is a per-chain
// value, some of these functions require a `chain` name. Any chain name is permissable, but
// they all require proper configuration in the TrueBlocks config files.
package tslibPkg

import (
	"time"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/bykof/gostradamus"
)

// DateFromBn returns a chain-specific date string given a block number.
func DateFromBn(chain string, bn uint64) (string, error) {
	ts, err := TsFromBn(chain, bn)
	if err != nil {
		return "", err
	}
	return DateFromTs(ts)
}

// DateFromName returns a chain-specific date (if found) given its chain-specific name
func DateFromName(chain, needle string) time.Time {
	specials, _ := GetSpecials(chain)
	if len(specials) == 0 || needle == "0" {
		date, _ := DateFromBn(chain, 0)
		dt, _ := gostradamus.Parse(date, gostradamus.Iso8601)
		return dt.Time()
	}

	if needle == "latest" {
		meta, _ := rpcClient.GetMetaData(chain, false)
		ts := rpcClient.GetBlockTimestamp(chain, meta.Latest)
		date, _ := DateFromTs(ts)
		dt, _ := gostradamus.Parse(date, gostradamus.Iso8601)
		return dt.Time()
	}

	for _, value := range specials {
		if value.Name == needle {
			date, _ := DateFromBn(chain, value.BlockNumber)
			dt, _ := gostradamus.Parse(date, gostradamus.Iso8601)
			return dt.Time()
		}
	}

	// default to first
	dt, _ := gostradamus.Parse(specials[0].Date, gostradamus.Iso8601)
	return dt.Time()
}

// DateFromTs returns a date string given a Linux timestamp (not chain-specific)
func DateFromTs(ts uint64) (string, error) {
	dt := gostradamus.FromUnixTimestamp(int64(ts))
	ret := dt.Format(gostradamus.Iso8601)
	return ret, nil
}
