package base

import (
	"io"
	"math/big"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

// Wei is a type alias for big.Int. This means we can't extend it by
// adding any of our own methods.
type Wei2 = MyWei

// MyWei is a type in its own right. This means we can extend it by
// adding our own methods, such as UnMarshalJSON and MarshalJSON which
// are required because our Json encodes big.Ints as strings. Note that
// MyWei is only temporary and will be replaced by Wei once the
// encoding issue is resolved.
type MyWei big.Int

func NewMyWei(x int64) *MyWei {
	return (*MyWei)(big.NewInt(x))
}

func (b *MyWei) ToInt() *big.Int {
	return (*big.Int)(b)
}

func (w *MyWei) Bytes() []byte {
	return (*big.Int)(w).Bytes()
}

func (w *MyWei) String() string {
	return (*big.Int)(w).String()
}

func (w *MyWei) SetUint64(x uint64) *MyWei {
	return (*MyWei)((*big.Int)(w).SetUint64(x))
}

func (w *MyWei) SetInt64(x int64) *MyWei {
	return (*MyWei)((*big.Int)(w).SetInt64(x))
}

func (w *MyWei) SetString(s string, base int) (*MyWei, bool) {
	if strings.HasPrefix(s, "0x") {
		s = s[2:]
		base = 16
	}
	if i, ok := (*big.Int)(w).SetString(s, base); ok {
		return (*MyWei)(i), true
	}
	return nil, false
}

func (w *MyWei) Float64() float64 {
	f, _ := (*big.Int)(w).Float64()
	return f
}

func (w *MyWei) Uint64() uint64 {
	return (*big.Int)(w).Uint64()
}

func (x *MyWei) Text(base int) string {
	return (*big.Int)(x).Text(base)
}

func (w *MyWei) Add(x, y *MyWei) *MyWei {
	return (*MyWei)((*big.Int)(w).Add((*big.Int)(x), (*big.Int)(y)))
}

func (w *MyWei) Sub(x, y *MyWei) *MyWei {
	return (*MyWei)((*big.Int)(w).Sub((*big.Int)(x), (*big.Int)(y)))
}

func (w *MyWei) Mul(x, y *MyWei) *MyWei {
	return (*MyWei)((*big.Int)(w).Mul((*big.Int)(x), (*big.Int)(y)))
}

func (w *MyWei) Div(x, y *MyWei) *MyWei {
	return (*MyWei)((*big.Int)(w).Div((*big.Int)(x), (*big.Int)(y)))
}

func (w *MyWei) Quo(x, y *MyWei) *MyWei {
	return (*MyWei)((*big.Int)(w).Quo((*big.Int)(x), (*big.Int)(y)))
}

func (w *MyWei) Cmp(y *MyWei) int {
	return (*big.Int)(w).Cmp((*big.Int)(y))
}

func (w *MyWei) MarshalText() (text []byte, err error) {
	return (*big.Int)(w).MarshalText()
}

func (w *MyWei) UnmarshalCache(version uint64, reader io.Reader) error {
	var v big.Int
	if err := cache.ReadValue(reader, &v, version); err != nil {
		return err
	}
	*w = (MyWei)(v)
	return nil
}

func (w *MyWei) MarshalCache(writer io.Writer) error {
	v := *w.ToInt()
	return cache.WriteValue(writer, &v)
}

func (w *MyWei) UnmarshalText(text []byte) error {
	return (*big.Int)(w).UnmarshalText(text)
}

func FormattedValue(in *MyWei, asEther bool, decimals int) string {
	if asEther {
		return ToEther(in).Text('f', -1*decimals)
	}
	return in.Text(10)
}

func ToEther(wei *MyWei) *Ether {
	f := NewEther(0)
	e := NewEther(1e18)
	return f.Quo(new(Ether).SetMyWei(wei), e)
}

func BiFromUint64(bn uint64) *big.Int {
	if bn == utils.NOPOS {
		return nil
	}
	return new(big.Int).SetUint64(bn)
}
