package base

import (
	"math/big"
	"strconv"
	"strings"
)

type Float big.Float

var (
	ZeroFloat = NewFloat(0)
	OneFloat  = NewFloat(1)
)

func NewFloat(f float64) *Float {
	e := new(Float)
	(*big.Float)(e).SetPrec(236) // IEEE 754 octuple-precision binary floating-point format: binary256
	(*big.Float)(e).SetMode(big.ToNearestEven)
	return e.SetFloat64(f)
}

func (f *Float) String() string {
	if f == nil {
		return "0"
	}
	// the negative number removes trailing zeros
	return (*big.Float)(f).String()
}

func (f *Float) Text(format byte, prec int) string {
	if f == nil {
		return "0"
	}
	return (*big.Float)(f).Text(format, prec)
}

// SetRawWei sets the Ether’s value directly from the Wei’s big.Int.
// No scaling is done – it simply copies the Wei value into a big.Float.
func (f *Float) SetRawWei(i *Wei) *Float {
	if f == nil {
		f = ZeroFloat
	}
	if i == nil {
		return (*Float)((*big.Float)(f).SetInt64(0))
	}
	return (*Float)((*big.Float)(f).SetInt((*big.Int)(i)))
}

func (f *Float) SetWei(i *Wei) *Float {
	if f == nil {
		f = ZeroFloat
	}
	if i == nil {
		return (*Float)((*big.Float)(f).SetInt64(0))
	}
	weiFloat := new(big.Float).SetInt((*big.Int)(i))
	divisor := new(big.Float).SetFloat64(1e18)
	converted := new(big.Float).Quo(weiFloat, divisor)
	return (*Float)(converted)
}

func (f *Float) IsZero() bool {
	return f.Equal(ZeroFloat)
}

func (f *Float) NotEqual(other *Float) bool {
	return !f.Equal(other)
}

func (f *Float) Equal(other *Float) bool {
	if f == nil && other == nil {
		return true
	}
	if f == nil || other == nil {
		return false
	}
	return (*big.Float)(f).Cmp((*big.Float)(other)) == 0
}

func (f *Float) LessThan(other *Float) bool {
	if f == nil && other == nil {
		return false
	}
	if f == nil {
		return other.Float64() > 0 // Treat nil as 0 for comparison
	}
	if other == nil {
		return f.Float64() < 0
	}
	return (*big.Float)(f).Cmp((*big.Float)(other)) < 0
}

func (f *Float) LessThanOrEqual(other *Float) bool {
	return f.LessThan(other) || f.Equal(other)
}

func (f *Float) GreaterThan(other *Float) bool {
	return !f.LessThanOrEqual(other)
}

func (f *Float) GreaterThanOrEqual(other *Float) bool {
	return !f.LessThan(other)
}

func (f *Float) SetInt64(i int64) *Float {
	if f == nil {
		f = ZeroFloat
	}
	return (*Float)((*big.Float)(f).SetInt64(i))
}

func (f *Float) SetUint64(i uint64) *Float {
	if f == nil {
		f = ZeroFloat
	}
	return (*Float)((*big.Float)(f).SetUint64(i))
}

func (f *Float) SetFloat64(fl float64) *Float {
	if f == nil {
		f = new(Float) // Create a new Float if nil
	}
	return (*Float)((*big.Float)(f).SetFloat64(fl))
}

func (f *Float) SetString(s string) (*Float, bool) {
	str := strings.Trim(strings.TrimSpace(s), "\"")
	if f == nil {
		f = ZeroFloat
	}
	if f, _, err := (*big.Float)(f).Parse(str, 0); err == nil {
		return (*Float)(f), true
	}
	return nil, false
}

func (f *Float) Float64() float64 {
	if f == nil {
		return 0.0
	}
	ret, _ := (*big.Float)(f).Float64()
	return ret
}

func (f *Float) Cmp(other *Float) int {
	if f == nil && other == nil {
		return 0
	}
	if f == nil {
		return -1
	}
	if other == nil {
		return 1
	}
	return (*big.Float)(f).Cmp((*big.Float)(other))
}

func (f *Float) Quo(a, b *Float) *Float {
	if a == nil || b == nil || b.Float64() == 0 {
		return ZeroFloat
	}
	return (*Float)((*big.Float)(f).Quo((*big.Float)(a), (*big.Float)(b)))
}

func (f *Float) Mul(a, b *Float) *Float {
	if a == nil || b == nil {
		return ZeroFloat
	}
	return (*Float)((*big.Float)(f).Mul((*big.Float)(a), (*big.Float)(b)))
}

func (f *Float) UnmarshalJSON(data []byte) error {
	str := safeUnquote(string(data))
	result, _ := strconv.ParseFloat(str, 64)
	*f = *NewFloat(result)
	return nil
}

func (f *Float) MarshalJSON() ([]byte, error) {
	if f == nil {
		return []byte(`"0"`), nil
	}
	return []byte(`"` + f.String() + `"`), nil
}
