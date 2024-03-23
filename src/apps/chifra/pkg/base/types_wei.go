package base

import "math/big"

// Wei is a type alias for big.Int. This means we can't extend it by
// adding any of our own methods.
type Wei = big.Int

// MyWei is a type in its own right. This means we can extend it by
// adding our own methods, such as UnMarshalJSON and MarshalJSON which
// are required because our Json encodes big.Ints as strings. Note that
// MyWei is only temporary and will be replaced by Wei once the
// encoding issue is resolved.
type MyWei big.Int

func NewMyWei(x int64) *MyWei {
	return (*MyWei)(big.NewInt(x))
}

func (w *MyWei) SetString(s string, base int) (*MyWei, bool) {
	if i, ok := (*big.Int)(w).SetString(s, base); ok {
		return (*MyWei)(i), true
	}
	return nil, false
}

func (w *MyWei) Add(x, y *MyWei) *MyWei {
	return (*MyWei)((*big.Int)(w).Add((*big.Int)(x), (*big.Int)(y)))
}

func (w *MyWei) Mul(x, y *MyWei) *MyWei {
	return (*MyWei)((*big.Int)(w).Mul((*big.Int)(x), (*big.Int)(y)))
}

func (w *MyWei) Div(x, y *MyWei) *MyWei {
	return (*MyWei)((*big.Int)(w).Div((*big.Int)(x), (*big.Int)(y)))
}
