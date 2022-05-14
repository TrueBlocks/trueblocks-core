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
	"github.com/araddon/dateparse"
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
		dt, _ := dateparse.ParseLocal(date)
		return dt
	}

	if needle == "latest" {
		meta, _ := rpcClient.GetMetaData(chain, false)
		ts := rpcClient.GetBlockTimestamp(chain, meta.Latest)
		date, _ := DateFromTs(ts)
		dt, _ := dateparse.ParseLocal(date)
		return dt
	}

	for _, value := range specials {
		if value.Name == needle {
			date, _ := DateFromBn(chain, value.BlockNumber)
			dt, _ := dateparse.ParseLocal(date)
			return dt
		}
	}

	// default to first
	dt, _ := dateparse.ParseLocal(specials[0].Date)
	return dt
}

// DateFromTs returns a date string given a Linux timestamp (not chain-specific)
func DateFromTs(ts uint64) (string, error) {
	// TODO: can be initialized at the start of the program as all dates are UTC
	time.Local, _ = time.LoadLocation("UTC")
	tm := time.Unix(int64(ts), 0)
	return tm.Format("2006-01-02 15:04:05 UTC"), nil
}
