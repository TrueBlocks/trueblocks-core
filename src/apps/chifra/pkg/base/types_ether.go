package base

import (
	"fmt"
	"math/big"
	"strings"
)

// Ether encapsulates big.Float so we can Unmarshal strings into various of
// our types. This is necessary because we export big.Floats to Json strings
// so we don't lose precision. The types mostly just cast to the big.Float and
// call in to the identical functions.
type Ether big.Float
type Float big.Float

var (
	ZeroEther = NewEther(0)
	ZeroFloat = NewFloat(0)
	OneFloat  = NewFloat(1)
)

func NewEther(f float64) *Ether {
	e := new(Ether)
	(*big.Float)(e).SetPrec(236) // IEEE 754 octuple-precision binary floating-point format: binary256
	(*big.Float)(e).SetMode(big.ToNearestEven)
	return e.SetFloat64(f)
}

func (e *Ether) String() string {
	if e == nil {
		return "0"
	}
	// the negative number removes trailing zeros
	return e.Text('f', -18)
}

func (e *Ether) Text(format byte, prec int) string {
	if e == nil {
		return "0"
	}
	return (*big.Float)(e).Text(format, prec)
}

// SetRawWei sets the Ether’s value directly from the Wei’s big.Int.
// No scaling is done – it simply copies the Wei value into a big.Float.
func (e *Ether) SetRawWei(i *Wei) *Ether {
	if e == nil {
		e = NewEther(0)
	}
	if i == nil {
		return (*Ether)((*big.Float)(e).SetInt64(0))
	}
	return (*Ether)((*big.Float)(e).SetInt((*big.Int)(i)))
}

func (e *Ether) SetWei(i *Wei) *Ether {
	if e == nil {
		e = NewEther(0)
	}
	if i == nil {
		return (*Ether)((*big.Float)(e).SetInt64(0))
	}
	weiFloat := new(big.Float).SetInt((*big.Int)(i))
	divisor := new(big.Float).SetFloat64(1e18)
	converted := new(big.Float).Quo(weiFloat, divisor)
	return (*Ether)(converted)
}

func (e *Ether) NotEqual(other *Ether) bool {
	return !e.Equal(other)
}

func (e *Ether) Equal(other *Ether) bool {
	if e == nil && other == nil {
		return true
	}
	if e == nil || other == nil {
		return false
	}
	return (*big.Float)(e).Cmp((*big.Float)(other)) == 0
}

func (e *Ether) LessThan(other *Ether) bool {
	if e == nil && other == nil {
		return false
	}
	if e == nil {
		return other.Float64() > 0 // Treat nil as 0 for comparison
	}
	if other == nil {
		return e.Float64() < 0
	}
	return (*big.Float)(e).Cmp((*big.Float)(other)) < 0
}

func (e *Ether) LessThanOrEqual(other *Ether) bool {
	return e.LessThan(other) || e.Equal(other)
}

func (e *Ether) GreaterThan(other *Ether) bool {
	return !e.LessThanOrEqual(other)
}

func (e *Ether) GreaterThanOrEqual(other *Ether) bool {
	return !e.LessThan(other)
}

func (e *Ether) SetInt64(i int64) *Ether {
	if e == nil {
		e = NewEther(0)
	}
	return (*Ether)((*big.Float)(e).SetInt64(i))
}

func (e *Ether) SetUint64(i uint64) *Ether {
	if e == nil {
		e = NewEther(0)
	}
	return (*Ether)((*big.Float)(e).SetUint64(i))
}

func (e *Ether) SetFloat64(f float64) *Ether {
	if e == nil {
		e = NewEther(0)
	}
	return (*Ether)((*big.Float)(e).SetFloat64(f))
}

func (e *Ether) SetString(s string) (*Ether, bool) {
	str := strings.Trim(strings.TrimSpace(s), "\"")
	if e == nil {
		e = NewEther(0)
	}
	if f, _, err := (*big.Float)(e).Parse(str, 0); err == nil {
		return (*Ether)(f), true
	}
	return nil, false
}

func (e *Ether) Float64() float64 {
	if e == nil {
		return 0.0
	}
	f, _ := (*big.Float)(e).Float64()
	return f
}

func (e *Ether) Cmp(other *Ether) int {
	if e == nil && other == nil {
		return 0
	}
	if e == nil {
		return -1
	}
	if other == nil {
		return 1
	}
	return (*big.Float)(e).Cmp((*big.Float)(other))
}

func (e *Ether) Quo(a, b *Ether) *Ether {
	if a == nil || b == nil || b.Float64() == 0 {
		return nil
	}
	return (*Ether)((*big.Float)(e).Quo((*big.Float)(a), (*big.Float)(b)))
}

func (e *Ether) UnmarshalJSON(data []byte) error {
	if e == nil {
		return fmt.Errorf("cannot unmarshal into nil Ether")
	}
	str := strings.Trim(strings.TrimSpace(string(data)), "\"")
	if len(str) == 0 {
		return nil
	}
	return (*big.Float)(e).UnmarshalText([]byte(str))
}

func (e *Ether) MarshalJSON() ([]byte, error) {
	if e == nil {
		return []byte(`"0"`), nil
	}
	return []byte(`"` + e.String() + `"`), nil
}

func NewFloat(f float64) *Float {
	e := new(Float)
	(*big.Float)(e).SetPrec(236) // IEEE 754 octuple-precision binary floating-point format: binary256
	(*big.Float)(e).SetMode(big.ToNearestEven)
	return e.setFloat64(f)
}

func (e *Float) String() string {
	if e == nil {
		return "0"
	}
	// the negative number removes trailing zeros
	return (*big.Float)(e).String()
}

func (e *Float) setFloat64(f float64) *Float {
	if e == nil {
		e = new(Float)
	}
	return (*Float)((*big.Float)(e).SetFloat64(f))
}

func (e *Float) Quo(a, b *Float) *Float {
	if a == nil || b == nil || b.Float64() == 0 {
		return nil
	}
	return (*Float)((*big.Float)(e).Quo((*big.Float)(a), (*big.Float)(b)))
}

func (e *Float) Mul(a, b *Float) *Float {
	if a == nil || b == nil {
		return nil
	}
	return (*Float)((*big.Float)(e).Mul((*big.Float)(a), (*big.Float)(b)))
}

func (e *Float) Float64() float64 {
	if e == nil {
		return 0.0
	}
	f, _ := (*big.Float)(e).Float64()
	return f
}

func (e *Float) SetString(s string) (*Float, bool) {
	str := strings.Trim(strings.TrimSpace(s), "\"")
	if e == nil {
		e = ZeroFloat
	}
	if f, _, err := (*big.Float)(e).Parse(str, 0); err == nil {
		return (*Float)(f), true
	}
	return nil, false
}

// SetRawWei sets the Ether’s value directly from the Wei’s big.Int.
// No scaling is done – it simply copies the Wei value into a big.Float.
func (e *Float) SetRawWei(i *Wei) *Float {
	if e == nil {
		e = ZeroFloat
	}
	if i == nil {
		return (*Float)((*big.Float)(e).SetInt64(0))
	}
	return (*Float)((*big.Float)(e).SetInt((*big.Int)(i)))
}

func (e *Float) SetInt64(i int64) *Float {
	if e == nil {
		e = ZeroFloat
	}
	return (*Float)((*big.Float)(e).SetInt64(i))
}
