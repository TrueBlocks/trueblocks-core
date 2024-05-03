package base

import (
	"io"
	"math/big"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

// Wei is a type in its own right. This means we can extend it by
// adding our own methods, such as UnMarshalJSON and MarshalJSON which
// are required because our Json encodes big.Ints as strings. Note that
type Wei big.Int

func NewWei(x int64) *Wei {
	return (*Wei)(big.NewInt(x))
}

func (b *Wei) ToInt() *big.Int {
	return (*big.Int)(b)
}

func (w *Wei) Bytes() []byte {
	return (*big.Int)(w).Bytes()
}

func (w *Wei) String() string {
	return (*big.Int)(w).String()
}

func (w *Wei) BigInt() *big.Int {
	return (*big.Int)(w)
}

func (w *Wei) IsZero() bool {
	return w.String() == "0"
}

func (w *Wei) SetUint64(x uint64) *Wei {
	return (*Wei)((*big.Int)(w).SetUint64(x))
}

func (w *Wei) SetInt64(x int64) *Wei {
	return (*Wei)((*big.Int)(w).SetInt64(x))
}

func (w *Wei) SetString(s string, base int) (*Wei, bool) {
	if strings.HasPrefix(s, "0x") {
		s = s[2:]
		base = 16
	}
	if i, ok := (*big.Int)(w).SetString(s, base); ok {
		return (*Wei)(i), true
	}
	return nil, false
}

func (w *Wei) Float64() float64 {
	f, _ := (*big.Int)(w).Float64()
	return f
}

func (w *Wei) Uint64() uint64 {
	return (*big.Int)(w).Uint64()
}

func (x *Wei) Text(base int) string {
	return (*big.Int)(x).Text(base)
}

func (w *Wei) Add(x, y *Wei) *Wei {
	return (*Wei)((*big.Int)(w).Add((*big.Int)(x), (*big.Int)(y)))
}

func (w *Wei) Sub(x, y *Wei) *Wei {
	return (*Wei)((*big.Int)(w).Sub((*big.Int)(x), (*big.Int)(y)))
}

func (w *Wei) Mul(x, y *Wei) *Wei {
	return (*Wei)((*big.Int)(w).Mul((*big.Int)(x), (*big.Int)(y)))
}

func (w *Wei) Div(x, y *Wei) *Wei {
	return (*Wei)((*big.Int)(w).Div((*big.Int)(x), (*big.Int)(y)))
}

func (w *Wei) Quo(x, y *Wei) *Wei {
	return (*Wei)((*big.Int)(w).Quo((*big.Int)(x), (*big.Int)(y)))
}

func (w *Wei) Cmp(y *Wei) int {
	return (*big.Int)(w).Cmp((*big.Int)(y))
}

func (w *Wei) MarshalText() (text []byte, err error) {
	return (*big.Int)(w).MarshalText()
}

func (w *Wei) UnmarshalCache(version uint64, reader io.Reader) error {
	var v big.Int
	if err := cache.ReadValue(reader, &v, version); err != nil {
		return err
	}
	*w = (Wei)(v)
	return nil
}

func (w *Wei) MarshalCache(writer io.Writer) error {
	v := *w.ToInt()
	return cache.WriteValue(writer, &v)
}

func (w *Wei) UnmarshalText(text []byte) error {
	return (*big.Int)(w).UnmarshalText(text)
}

// TODO: BOGUS - THIS NAME SUCKS

func FormattedValue(in *Wei, asEther bool, decimals int) string {
	if asEther {
		return ToEther(in).Text('f', -1*decimals)
	}
	return in.Text(10)
}

func ToEther(wei *Wei) *Ether {
	f := NewEther(0)
	e := NewEther(1e18)
	return f.Quo(new(Ether).SetWei(wei), e)
}

func BiFromUint64(bn uint64) *big.Int {
	if bn == utils.NOPOS {
		return nil
	}
	return new(big.Int).SetUint64(bn)
}

// TODO: This is probably the same as String (could be removed)

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
