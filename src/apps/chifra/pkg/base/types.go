package base

import (
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
type Float float64

const NOPOS = uint64(^uint64(0))
const NOPOSI = Timestamp(0xdeadbeef)
const NOPOSN = Value(^uint64(0))

func (t *Timestamp) Int64() int64 {
	return int64(*t)
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
