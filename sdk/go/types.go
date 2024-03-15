package sdk

import (
	"fmt"
	"math/big"
	"strconv"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
)

type AddressStr struct {
	base.Address
}

func (b *AddressStr) UnmarshalJSON(data []byte) error {
	b.Address = base.HexToAddress(string(data))
	return nil
}

type BigIntStr struct {
	big.Int
}

func (b *BigIntStr) UnmarshalJSON(data []byte) error {
	if len(data) < 3 {
		return nil
	}
	strData := string(data[1 : len(data)-1])
	if _, ok := b.Int.SetString(strData, 10); !ok {
		return fmt.Errorf("big.Int.SetString: error setting value")
	}
	return nil
}

type IntStr base.Blknum

func (i *IntStr) UnmarshalJSON(data []byte) error {
	if len(data) < 3 {
		return nil
	}
	intValue, err := strconv.ParseUint(string(data[1:len(data)-1]), 10, 64)
	if err != nil {
		return err
	}
	*i = IntStr(intValue)
	return nil
}

type FloatStr float64

func (f *FloatStr) UnmarshalJSON(data []byte) error {
	if len(data) < 3 {
		return nil
	}
	floatValue, err := strconv.ParseFloat(string(data[1:len(data)-1]), 64)
	if err != nil {
		return err
	}
	*f = FloatStr(floatValue)
	return nil
}
