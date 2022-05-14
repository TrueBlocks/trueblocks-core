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
)

// TsFromBn returns a chain-specific Linux timestamp given a block number
func TsFromBn(chain string, bn uint64) (uint64, error) {
	ret, err := fromBn(chain, bn)
	return uint64(ret.Ts), err
}

// TsFromDate returns a Linux timestamp given a date string (not chain-specific)
func TsFromDate(date string) (uint64, error) {
	// assumes an already validated date string
	str := strings.Replace(date, "T", " ", -1)
	if strings.Count(str, ":") == 0 {
		if strings.Count(str, " ") == 1 {
			str += ":00:00"
		} else {
			str += " 00:00:00"
		}
	} else if strings.Count(str, ":") == 1 {
		str += ":00"
	}
	t, err := time.Parse("2006-01-02 15:04:05", str)
	if err != nil {
		return 0, err
	}
	return uint64(t.Unix()), nil
}
