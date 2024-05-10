package base

import (
	"math/big"
	"strings"
)

// Ether encapsulates big.Float so we can Unmarshal strings into various of
// our types. This is necessary because we export big.Floats to Json strings
// so we don't lose precision. The types mostly just cast to the big.Float and
// call in to the identical functions.
type Ether big.Float

func NewEther(f float64) *Ether {
	e := new(Ether)
	(*big.Float)(e).SetPrec(236) //  IEEE 754 octuple-precision binary floating-point format: binary256
	(*big.Float)(e).SetMode(big.ToNearestEven)
	return e.SetFloat64(f)
}

func (e *Ether) String() string {
	// the negative number removes trailing zeros
	return e.Text('f', -18)
}

func (x *Ether) Text(format byte, prec int) string {
	return (*big.Float)(x).Text(format, prec)
}

func (e *Ether) SetWei(i *Wei) *Ether {
	return (*Ether)((*big.Float)(e).SetInt((*big.Int)(i)))
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
	str := strings.Trim(strings.TrimSpace(s), "\"") // strip quotes and whitespace if any
	if f, _, err := (*big.Float)(e).Parse(str, 0); err == nil {
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
	str := strings.Trim(strings.TrimSpace(string(data)), "\"") // strip quotes and whitespace if any
	if len(str) == 0 {
		return nil
	}
	return (*big.Float)(e).UnmarshalText([]byte(str))
}

func (e *Ether) MarshalJSON() ([]byte, error) {
	return []byte(`"` + e.String() + `"`), nil
}
