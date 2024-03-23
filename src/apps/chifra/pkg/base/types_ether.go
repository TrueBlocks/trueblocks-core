package base

import (
	"math/big"

	"github.com/ethereum/go-ethereum/params"
)

// Ether encapsulates big.Float so we can Unmarshal strings into a big.Float.
// This is necessary because we sometimes import data from Json where we must
// use strings or lose precision. The type does nothing other than cast to the
// big.Float and call in to the same function.
type Ether big.Float

func (e *Ether) String() string {
	return (*big.Float)(e).String()
}

func (e *Ether) Text(fmt byte, prec int) string {
	return (*big.Float)(e).Text(fmt, prec)
}

func (e *Ether) Float64() (float64, big.Accuracy) {
	f, a := (*big.Float)(e).Float64()
	return f, a
}

func (e *Ether) Cmp(other *Ether) int {
	return (*big.Float)(e).Cmp((*big.Float)(other))
}

func (e *Ether) SetInt(i *big.Int) *Ether {
	return (*Ether)((*big.Float)(e).SetInt(i))
}

func (e *Ether) SetInt64(i int64) *Ether {
	return (*Ether)((*big.Float)(e).SetInt64(i))
}

func (e *Ether) SetUint64(i uint64) *Ether {
	return (*Ether)((*big.Float)(e).SetUint64(i))
}

func (e *Ether) SetFloat64(f float64) *Ether {
	return (*Ether)((*big.Float)(e).SetFloat64(f))
}

func (e *Ether) SetString(s string) (*Ether, bool) {
	if f, _, err := (*big.Float)(e).Parse(s, 0); err == nil {
		return (*Ether)(f), true
	}
	return nil, false
}

func (e *Ether) Quo(a, b *Ether) *Ether {
	return (*Ether)((*big.Float)(e).Quo((*big.Float)(a), (*big.Float)(b)))
}

func (e *Ether) SetPrec(prec uint) *Ether {
	return (*Ether)((*big.Float)(e).SetPrec(prec))
}

func (e *Ether) SetMode(mode big.RoundingMode) *Ether {
	return (*Ether)((*big.Float)(e).SetMode(mode))
}

func NewEther(f float64) *Ether {
	return new(Ether).SetFloat64(f)
}

func ToEther(wei *big.Int) *Ether {
	// Copied from https://github.com/ethereum/go-ethereum/issues/21221#issuecomment-805852059
	f := new(Ether)
	f.SetPrec(236) //  IEEE 754 octuple-precision binary floating-point format: binary256
	f.SetMode(big.ToNearestEven)
	fWei := new(Ether)
	fWei.SetPrec(236) //  IEEE 754 octuple-precision binary floating-point format: binary256
	fWei.SetMode(big.ToNearestEven)
	return f.Quo(fWei.SetInt(wei), NewEther(params.Ether))
}

func FormattedValue(in big.Int, asEther bool, decimals int) string {
	if asEther {
		return ToEther(&in).Text('f', -1*decimals)
	}
	return in.Text(10)
}
