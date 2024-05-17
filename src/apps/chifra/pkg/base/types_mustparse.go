package base

import (
	"strconv"
)

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
	i := MustParseUint64(input)
	return *NewWei(0).SetUint64(i)
}

func MustParseInt64(input string) int64 {
	ret, _ := strconv.ParseInt(input, 0, 64)
	return ret
}

func MustParseUint64(input string) uint64 {
	ret, _ := strconv.ParseUint(input, 0, 64)
	return ret
}

func MustParseFloat64(val string) float64 {
	f, _ := strconv.ParseFloat(val, 64)
	return f
}
