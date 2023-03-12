package tslib

import (
	"strings"

	"github.com/bykof/gostradamus"
)

// FromBnToTs returns a chain-specific Linux timestamp given a block number
func FromBnToTs(chain string, bn uint64) (uint64, error) {
	ret, err := FromBn(chain, bn)
	return uint64(ret.Ts), err
}

// FromDateToTs returns a Linux timestamp given a date string (not chain-specific)
func FromDateToTs(dateStr string) (uint64, error) {
	str := toIsoDateStr(dateStr)
	t, err := gostradamus.Parse(str, gostradamus.Iso8601)
	if err != nil {
		return 0, err
	}
	return uint64(t.UnixTimestamp()), nil
}

func toIsoDateStr(dateStr string) string {
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
