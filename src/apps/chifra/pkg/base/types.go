package base

import (
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

type Gas = Numeral
type Txnum = Numeral
type TraceId = Numeral
type Lognum = Numeral
type Topic = string
type Timestamp = int64

type Blknum = uint64

const NOPOSN2 = NOPOS

func MustParseNumeral(input string) Numeral {
	result, _ := strconv.ParseUint(input, 0, 64)
	return Numeral(result)
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

func MustParseUint(input string) (result uint64) {
	result, _ = strconv.ParseUint(input, 0, 64)
	return
}

func MustParseWei(input string) Wei {
	return *NewWei(0).SetUint64(MustParseUint(input))
}

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
