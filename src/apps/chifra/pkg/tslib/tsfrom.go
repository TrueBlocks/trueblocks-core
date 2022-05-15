// Package `tslibPkg` provides conversions between four entities: block numbers, dates, Linux
// timestamps, and special named blocks. The function names are self-explanitory.
//
// Note: because the relationship between block numbers and dates or timestamps is a per-chain
// value, some of these functions require a `chain` name. Any chain name is permissable, but
// they all require proper configuration in the TrueBlocks config files.
package tslibPkg

import (
	"strings"

	"github.com/bykof/gostradamus"
)

// TsFromBn returns a chain-specific Linux timestamp given a block number
func TsFromBn(chain string, bn uint64) (uint64, error) {
	ret, err := fromBn(chain, bn)
	return uint64(ret.Ts), err
}

func ToIsoDateStr(dateStr string) string {
	// assumes an already validated date string
	str := strings.Replace(dateStr, "T", " ", -1)
	if strings.Count(str, ":") == 0 {
		if strings.Count(str, " ") == 1 {
			str += ":00:00"
		} else {
			str += " 00:00:00"
		}
	} else if strings.Count(str, ":") == 1 {
		str += ":00"
	}
	str = strings.Replace(str, " ", "T", -1)
	str += ".000000"
	return str
}

// TsFromDate returns a Linux timestamp given a date string (not chain-specific)
func TsFromDate(dateStr string) (uint64, error) {
	str := ToIsoDateStr(dateStr)
	t, err := gostradamus.Parse(str, gostradamus.Iso8601)
	if err != nil {
		return 0, err
	}
	return uint64(t.UnixTimestamp()), nil
}
