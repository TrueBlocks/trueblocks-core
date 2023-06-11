package base

import "math/big"

type Wei = big.Int
type BigInt struct {
	big.Int
}

type Gas = uint64
type Blknum = uint64
type Topic = string
type Timestamp = int64

func (w *BigInt) String() string {
	return w.Text(10)
}

func NewBigInt(i int64) BigInt {
	return BigInt{*big.NewInt(i)}
}

func NewFromBig(i *big.Int) BigInt {
	return BigInt{*i}
}
