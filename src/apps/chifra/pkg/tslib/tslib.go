// Package `tslibPkg` provides conversions between four entities: block numbers, dates, Linux
// timestamps, and special named blocks. The function names are self-explanitory.
//
// Note: because the relationship between block numbers and dates or timestamps is a per-chain
// value, some of these functions require a `chain` name. Any chain name is permissable, but
// they all require proper configuration in the TrueBlocks config files.
package tslibPkg

import (
	"strings"
	"time"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/araddon/dateparse"
)

// BnFromDate returns a chain-specific block number given a date string (date strings are valid JSON dates).
func BnFromDate(chain, date string) (uint64, error) {
	ts, err := TsFromDate(date)
	if err != nil {
		return 0, err
	}
	ret, err := fromTs(chain, ts)
	return uint64(ret.Bn), err
}

// BnFromName returns the chain-specific block number (if found) given the name of a special block. The list of special blocks is per-chain.
func BnFromName(chain, needle string) (uint64, bool) {
	if needle == "latest" {
		return rpcClient.GetMetaData(chain, false).Latest, true
	}

	specials, _ := GetSpecials(chain)
	for _, value := range specials {
		if value.Name == needle {
			return value.BlockNumber, true
		}
	}

	return uint64(utils.NOPOS), false
}

// BnFromTs returns a chain-specific block number given a Linux timestamp.
func BnFromTs(chain string, ts uint64) (uint64, error) {
	ret, err := fromTs(chain, ts)
	return uint64(ret.Bn), err
}

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
	if needle == "latest" {
		ts := rpcClient.GetBlockTimestamp(chain, rpcClient.GetMetaData(chain, false).Latest)
		date, _ := DateFromTs(ts)
		dt, _ := dateparse.ParseLocal(date)
		return dt
	}

	specials, _ := GetSpecials(chain)
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

// NameFromBn returns the block's chain-specific name (if found) given its block number
func NameFromBn(chain string, needle uint64) (string, bool) {
	specials, _ := GetSpecials(chain)
	for _, value := range specials {
		if value.BlockNumber == needle {
			return value.Name, true
		}
	}
	return "", false
}

// TsFromBn returns a chain-specific Linux timestamp given a block number
func TsFromBn(chain string, bn uint64) (uint64, error) {
	ret, err := fromBn(chain, bn)
	return uint64(ret.Ts), err
}

// TsFromDate returns a Linux timestamp given a date string (not chain-specific)
func TsFromDate(date string) (uint64, error) {
	t, err := time.Parse("2006-01-02 15:04:05", strings.Replace(date, "T", " ", -1))
	if err != nil {
		return 0, err
	}
	return uint64(t.Unix()), nil
}
