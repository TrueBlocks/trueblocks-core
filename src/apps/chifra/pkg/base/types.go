package base

import (
	"fmt"
	"strconv"
)

type Numeral uint64

func (g Numeral) Uint64() uint64 {
	return uint64(g)
}

func (g *Numeral) UnmarshalJSON(data []byte) error {
	result, _ := strconv.ParseUint(string(data), 0, 64)
	*g = Numeral(result)
	return nil
}

func MustParseNumeral(input any) Numeral {
	result, _ := strconv.ParseUint(fmt.Sprint(input), 0, 64)
	return Numeral(result)
}

type Blknum = uint64
type Gas = Numeral
type Txnum = Numeral
type Lognum = uint64
type Topic = string
type Timestamp = int64

// TODO: Might be nice if the below two values were the same so we could cast between them.
// TODO: Trouble is that these values may be stored on disc.

const NOPOS = uint64(^uint64(0))
const NOPOSI = int64(0xdeadbeef)
const NOPOSN = Numeral(^uint64(0))

func IsFinal(latestTs, blockTs Timestamp) bool {
	// TODO: This is not consistent with they way we determine unripe in the scraper, for example.
	var min = Timestamp(5)
	var s = Timestamp(60)
	var pendingPeriod = min * s
	var diff = latestTs - blockTs
	return diff >= pendingPeriod
}
