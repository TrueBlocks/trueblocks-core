package base

import (
	"math/big"
	"strings"
)

// Ether encapsulates big.Float so we can Unmarshal strings into a big.Float.
// This is necessary because we sometimes import data from Json where we must
// use strings or lose precision. The type does nothing other than cast to the
// big.Float and call in to the same function.
type Ether big.Float

func NewEther(f float64) *Ether {
	e := new(Ether)
	(*big.Float)(e).SetPrec(236) //  IEEE 754 octuple-precision binary floating-point format: binary256
	(*big.Float)(e).SetMode(big.ToNearestEven)
	return e.SetFloat64(f)
}

func (e *Ether) String() string {
	// the negative number removes trailing zeros
	return (*big.Float)(e).Text('f', -18)
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

func (e *Ether) Float64() float64 {
	f, _ := (*big.Float)(e).Float64()
	return f
}

func (e *Ether) Cmp(other *Ether) int {
	return (*big.Float)(e).Cmp((*big.Float)(other))
}

func (e *Ether) Quo(a, b *Ether) *Ether {
	return (*Ether)((*big.Float)(e).Quo((*big.Float)(a), (*big.Float)(b)))
}

func (e *Ether) UnmarshalJSON(data []byte) error {
	str := strings.Replace(string(data), "\"", "", -1)
	(*big.Float)(e).SetString(str)
	return nil
}

func ToEther(wei *big.Int) *Ether {
	f := NewEther(0)
	e := NewEther(1e18)
	return f.Quo(new(Ether).SetInt(wei), e)
}

func FormattedValue(in big.Int, asEther bool, decimals int) string {
	if asEther {
		return (*big.Float)(ToEther(&in)).Text('f', -1*decimals)
	}
	return in.Text(10)
}
