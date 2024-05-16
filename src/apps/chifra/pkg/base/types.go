package base

import (
	"strconv"

	"github.com/bykof/gostradamus"
)

// make -j 12 && time make test-all shows:
// 306.71 and 353.82 when Value is its own type
// 375.40 and 332.51 when Value is an alias
// Conclusion: Minimal difference, but concrete type is slightly faster
// and more useful because it supports Json parsing and future needs
// Some more testing of speed
// When				Seconds		nTests	Seconds/Test
// Two weeks ago	505			579		0.8721
// Currently		1036.2		1200	0.8635 (a bit faster)
// As an alias		1056		1200	0.8800 (a bit slower)

// type Value = uint64 // as an alias
type Value uint64 // allows for json parsing
type Blknum = Value
type Txnum = Value
type Tracenum = Value
type Lognum = Value
type Gas = Value
type Topic = string
type Timestamp int64

const NOPOS = uint64(^uint64(0))
const NOPOSI = Timestamp(0xdeadbeef)
const NOPOSN = Value(^uint64(0))

func (g *Value) UnmarshalJSON(data []byte) error {
	result, _ := strconv.ParseUint(string(data), 0, 64)
	*g = Value(result)
	return nil
}

func (t *Timestamp) UnmarshalJSON(data []byte) error {
	result, _ := strconv.ParseUint(string(data), 0, 64)
	*t = Timestamp(result)
	return nil
}

func (t *Timestamp) Int64() int64 {
	return int64(*t)
}

func MustParseBlknum(input string) Blknum {
	ret, _ := strconv.ParseUint(input, 0, 64)
	return Blknum(ret)
}

func MustParseTxnum(input string) Txnum {
	ret, _ := strconv.ParseUint(input, 0, 64)
	return Txnum(ret)
}

func MustParseValue(input string) Value {
	ret, _ := strconv.ParseUint(input, 0, 64)
	return Value(ret)
}

func MustParseGas(input string) Gas {
	ret, _ := strconv.ParseUint(input, 0, 64)
	return Gas(ret)
}

func MustParseTimestamp(input string) Timestamp {
	ret, _ := strconv.ParseInt(input, 0, 64)
	return Timestamp(ret)
}

func MustParseWei(input string) Wei {
	i := MustParseUint(input)
	return *NewWei(0).SetUint64(i)
}

func MustParseInt(input string) int64 {
	ret, _ := strconv.ParseInt(input, 0, 64)
	return ret
}

func MustParseUint(input string) uint64 {
	ret, _ := strconv.ParseUint(input, 0, 64)
	return ret
}

func MustParseFloat(val string) float64 {
	f, _ := strconv.ParseFloat(val, 64)
	return f
}

func FormattedDate(ts Timestamp) string {
	return gostradamus.FromUnixTimestamp(int64(ts)).Format("2006-01-02 15:04:05 UTC")
}

func IsFinal(latestTs, blockTs Timestamp) bool {
	// TODO: This is not consistent with they way we determine unripe in the scraper, for example.
	var min = Timestamp(5)
	var s = Timestamp(60)
	var pendingPeriod = min * s
	var diff = latestTs - blockTs
	return diff >= pendingPeriod
}
