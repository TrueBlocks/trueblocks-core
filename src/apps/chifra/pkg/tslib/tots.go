package tslib

import (
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/base"
	"github.com/bykof/gostradamus"
)

// FromBnToTs returns a chain-specific Linux timestamp given a block number
func FromBnToTs(chain string, bn base.Blknum) (base.Timestamp, error) {
	ret, err := FromBn(chain, bn)
	return base.Timestamp(ret.Ts), err
}

// FromDateToTs returns a Linux timestamp given a date string (not chain-specific)
func FromDateToTs(dateStr string) (base.Timestamp, error) {
	str := toIsoDateStr(dateStr)
	t, err := gostradamus.Parse(str, gostradamus.Iso8601)
	if err != nil {
		return 0, err
	}
	return base.Timestamp(t.UnixTimestamp()), nil
}

func toIsoDateStr(dateStr string) string {
	// assumes an already validated date string
	str := strings.ReplaceAll(dateStr, "T", " ")
	if strings.Count(str, ":") == 0 {
		if strings.Count(str, " ") == 1 {
			str += ":00:00"
		} else {
			str += " 00:00:00"
		}
	} else if strings.Count(str, ":") == 1 {
		str += ":00"
	}
	str = strings.ReplaceAll(str, " ", "T")
	str += ".000000"
	return str
}
