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

type Gas = Numeral

func MustParseGas(input any) Gas {
	return MustParseNumeral(input)
}

type Blknum = uint64
type Txnum = uint64
type Topic = string
type Timestamp = int64

func IsFinal(latestTs, blockTs Timestamp) bool {
	// TODO: This is not consistent with they way we determine unripe in the scraper, for example.
	var min = Timestamp(5)
	var s = Timestamp(60)
	var pendingPeriod = min * s
	var diff = latestTs - blockTs
	return diff >= pendingPeriod
}
