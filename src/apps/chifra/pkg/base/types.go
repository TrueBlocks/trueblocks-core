package base

import (
	"strconv"
)

// make -j 12 && time make test-all shows:
// 306.71 and 353.82 when Numeral is its own type
// 375.40 and 332.51 when Numeral is an alias
// Conclusion: Minimal difference, but concrete type is slightly faster
// and more useful because it supports Json parsing and future needs

type Numeral uint64

func (g *Numeral) UnmarshalJSON(data []byte) error {
	result, _ := strconv.ParseUint(string(data), 0, 64)
	*g = Numeral(result)
	return nil
}

type Blknum = Numeral
type Txnum = Numeral
type TraceId = Numeral
type Lognum = Numeral
type Gas = Numeral
type Topic = string
type Timestamp = int64

const NOPOS = uint64(^uint64(0))
const NOPOSI = int64(0xdeadbeef)
const NOPOSN = Numeral(^uint64(0))

func MustParseNumeral(input string) Numeral {
	ret, _ := strconv.ParseUint(input, 0, 64)
	return Numeral(ret)
}

// TODO: This is here to avoid circular imports
func MustParseBlknum(input string) Blknum {
	ret, _ := strconv.ParseUint(input, 0, 64)
	return Blknum(ret)
}

func MustParseInt(input string) int64 {
	ret, _ := strconv.ParseInt(input, 0, 64)
	return ret
}

func MustParseUint(input string) uint64 {
	ret, _ := strconv.ParseUint(input, 0, 64)
	return ret
}

func MustParseWei(input string) Wei {
	i := MustParseUint(input)
	return *NewWei(0).SetUint64(i)
}

func MustParseFloat(val string) float64 {
	f, _ := strconv.ParseFloat(val, 64)
	return f
}

func IsFinal(latestTs, blockTs Timestamp) bool {
	// TODO: This is not consistent with they way we determine unripe in the scraper, for example.
	var min = Timestamp(5)
	var s = Timestamp(60)
	var pendingPeriod = min * s
	var diff = latestTs - blockTs
	return diff >= pendingPeriod
}
