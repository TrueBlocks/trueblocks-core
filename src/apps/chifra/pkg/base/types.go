package base

import "math/big"

type Wei = big.Int
type Gas = uint64
type Blknum = uint64
type Txnum = uint64
type Topic = string
type Timestamp = int64

func HexToWei(hex string) (result *Wei) {
	result = new(Wei)
	if hex == "" {
		return
	}

	if len(hex) > 66 {
		// Cut garbage off if hex is too long
		result.SetString(hex[2:66], 16)
	} else {
		result.SetString(hex[2:], 16)
	}
	return
}
