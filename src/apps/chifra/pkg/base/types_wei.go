package base

import (
	"fmt"
	"io"
	"math/big"
	"strings"
)

// Wei is a type in its own right. This means we can extend it by
// adding our own methods, such as UnMarshalJSON and MarshalJSON which
// are required because our Json encodes big.Ints as strings. Note that
type Wei big.Int

var (
	ZeroWei = NewWei(0)
)

func NewWei(x int64) *Wei {
	return (*Wei)(big.NewInt(x))
}

func NewWeiStr(x string) *Wei {
	val := big.NewInt(0)
	if strings.HasPrefix(x, "0x") || strings.HasPrefix(x, "0X") {
		_, ok := val.SetString(x[2:], 16)
		if !ok {
			return (*Wei)(big.NewInt(0))
		}
	} else {
		_, ok := val.SetString(x, 10)
		if !ok {
			return (*Wei)(big.NewInt(0))
		}
	}
	return (*Wei)(val)
}

func (b *Wei) ToBigInt() *big.Int {
	return (*big.Int)(b)
}

func (w *Wei) Bytes() []byte {
	if w == nil {
		return nil
	}
	return (*big.Int)(w).Bytes()
}

func (w *Wei) String() string {
	if w == nil {
		return "0"
	}
	return (*big.Int)(w).String()
}

func (w *Wei) Equal(other *Wei) bool {
	if w == nil && other == nil {
		return true
	}
	if w == nil || other == nil {
		return false
	}
	return (*big.Int)(w).Cmp((*big.Int)(other)) == 0
}

func (w *Wei) NotEqual(other *Wei) bool {
	return !w.Equal(other)
}

func (w *Wei) LessThan(other *Wei) bool {
	if w == nil && other == nil {
		return false
	}
	if w == nil {
		return other.Sign() > 0
	}
	if other == nil {
		return w.Sign() < 0
	}
	return (*big.Int)(w).Cmp((*big.Int)(other)) < 0
}

func (w *Wei) LessThanOrEqual(other *Wei) bool {
	return w.LessThan(other) || w.Equal(other)
}

func (w *Wei) GreaterThan(other *Wei) bool {
	return !w.LessThanOrEqual(other)
}

func (w *Wei) GreaterThanOrEqual(other *Wei) bool {
	return !w.LessThan(other)
}

func (w *Wei) BigInt() *big.Int {
	return (*big.Int)(w)
}

func (w *Wei) IsZero() bool {
	if w == nil {
		return true
	}
	return (*big.Int)(w).Sign() == 0
}

func (w *Wei) SetUint64(x uint64) *Wei {
	if w == nil {
		return (*Wei)(new(big.Int).SetUint64(x))
	}
	return (*Wei)((*big.Int)(w).SetUint64(x))
}

func (w *Wei) SetInt64(x int64) *Wei {
	if w == nil {
		return (*Wei)(new(big.Int).SetInt64(x))
	}
	return (*Wei)((*big.Int)(w).SetInt64(x))
}

func (w *Wei) SetString(s string, base int) (*Wei, bool) {
	if strings.HasPrefix(s, "0x") {
		s = s[2:]
		base = 16
	}
	v := new(big.Int)
	if w != nil {
		v = (*big.Int)(w)
	}
	if i, ok := v.SetString(s, base); ok {
		return (*Wei)(i), true
	}
	return nil, false
}

func (w *Wei) Float64() float64 {
	if w == nil {
		return 0.0
	}
	f, _ := (*big.Int)(w).Float64()
	return f
}

func (w *Wei) Uint64() uint64 {
	if w == nil {
		return 0
	}
	return (*big.Int)(w).Uint64()
}

func (w *Wei) Text(base int) string {
	if w == nil {
		return "0"
	}
	return (*big.Int)(w).Text(base)
}

func (w *Wei) Add(x, y *Wei) *Wei {
	if x == nil || y == nil {
		return nil
	}
	result := new(big.Int).Add((*big.Int)(x), (*big.Int)(y))
	return (*Wei)(result)
}

func (w *Wei) Sub(x, y *Wei) *Wei {
	if x == nil || y == nil {
		return nil
	}
	result := new(big.Int).Sub((*big.Int)(x), (*big.Int)(y))
	return (*Wei)(result)
}

func (w *Wei) Mul(x, y *Wei) *Wei {
	if x == nil || y == nil {
		return nil
	}
	result := new(big.Int).Mul((*big.Int)(x), (*big.Int)(y))
	return (*Wei)(result)
}

func (w *Wei) Div(x, y *Wei) *Wei {
	if x == nil || y == nil || y.IsZero() {
		return nil
	}
	result := new(big.Int).Div((*big.Int)(x), (*big.Int)(y))
	return (*Wei)(result)
}

func (w *Wei) Quo(x, y *Wei) *Wei {
	if x == nil || y == nil || y.IsZero() {
		return nil
	}
	result := new(big.Int).Quo((*big.Int)(x), (*big.Int)(y))
	return (*Wei)(result)
}

func (w *Wei) Cmp(y *Wei) int {
	if w == nil && y == nil {
		return 0
	}
	if w == nil {
		return -1
	}
	if y == nil {
		return 1
	}
	return (*big.Int)(w).Cmp((*big.Int)(y))
}

func (w *Wei) MarshalText() (text []byte, err error) {
	if w == nil {
		return []byte("0"), nil
	}
	return (*big.Int)(w).MarshalText()
}

func (w *Wei) UnmarshalText(text []byte) error {
	if w == nil {
		return fmt.Errorf("cannot unmarshal into nil Wei")
	}
	return (*big.Int)(w).UnmarshalText(text)
}

func (e *Wei) UnmarshalJSON(data []byte) error {
	if e == nil {
		return fmt.Errorf("cannot unmarshal into nil Wei")
	}
	str := strings.Trim(strings.TrimSpace(string(data)), "\"")
	if len(str) == 0 {
		return nil
	}
	return (*big.Int)(e).UnmarshalText([]byte(str))
}

func (w *Wei) UnmarshalCache(fileVersion uint64, reader io.Reader) error {
	var v big.Int
	if err := ReadValue(reader, &v, fileVersion); err != nil {
		return err
	}
	*w = (Wei)(v)
	return nil
}

func (w *Wei) MarshalCache(writer io.Writer) error {
	if w == nil {
		return WriteValue(writer, big.NewInt(0))
	}
	v := *w.ToBigInt()
	return WriteValue(writer, &v)
}

func (w *Wei) ToFloatString(decimals int) string {
	return ToFloatWithDecimals(w, decimals).Text('f', -1*decimals)
}

func ToFloat(wei *Wei) *Float {
	return ToFloatWithDecimals(wei, 18)
}

func ToFloatWithDecimals(wei *Wei, decimals int) *Float {
	f := NewFloat(0)
	// Calculate 10^decimals as the divisor
	divisorInt := new(big.Int).Exp(big.NewInt(10), big.NewInt(int64(decimals)), nil)
	divisor := (*Float)(new(big.Float).SetInt(divisorInt))
	return f.Quo(new(Float).SetRawWei(wei), divisor)
}

func BiFromBn(bn Blknum) *big.Int {
	if bn == NOPOSN {
		return nil
	}
	return new(big.Int).SetUint64(uint64(bn))
}

func HexToWei(hex string) *Wei {
	result := new(Wei)
	if hex == "" {
		return result
	}

	if len(hex) > 66 {
		// Cut garbage off if hex is too long
		result.SetString(hex[2:66], 16)
	} else {
		result.SetString(hex[2:], 16)
	}
	return result
}

func WeiToHash(wei *Wei) (string, error) {
	b := wei.Bytes()
	if len(b) > 32 {
		return "", fmt.Errorf("wei.Bytes() returned %d bytes, which is more than the allowed 32", len(b))
	}
	padded := make([]byte, 32)
	copy(padded[32-len(b):], b)
	hash := BytesToHash(padded)
	return hash.Hex(), nil
}

// Sign returns -1 if the Wei is negative, 0 if it is zero, and 1 if it is positive.
func (w *Wei) Sign() int {
	if w == nil {
		return 0
	}
	return (*big.Int)(w).Sign()
}

// Neg returns a new Wei that is the negation (i.e., multiplication by -1) of w.
func (w *Wei) Neg() *Wei {
	if w == nil {
		return nil
	}
	result := new(big.Int).Neg((*big.Int)(w))
	return (*Wei)(result)
}

// Abs returns the absolute value of w. Returns nil if w is nil.
func (w *Wei) Abs() *Wei {
	if w == nil {
		return nil
	}
	result := new(big.Int).Abs((*big.Int)(w))
	return (*Wei)(result)
}
