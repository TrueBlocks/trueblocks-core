package base

import (
	"encoding/hex"
	"fmt"

	"github.com/ethereum/go-ethereum/common"
)

// Address is a wrapper for go-ethereum's Address type that always
// return lower case hex.
type Address struct {
	common.Address
	addrStr string
}

// Hex returns string representation of an address
func (a *Address) Hex() string {
	if a.IsZero() {
		return "0x0"
	}
	return a.addrStr
}

func (a *Address) String() string {
	return a.Hex()
}

func (a Address) Format(s fmt.State, c rune) {
	s.Write([]byte(a.Hex()))
}

func (a Address) MarshalText() ([]byte, error) {
	return []byte(a.Hex()), nil
}

// SetHex sets the address based on the provided string
func (a *Address) SetHex(hexStr string) {
	a.Address = common.HexToAddress(hexStr)
	if hexStr == "0x0000000000000000000000000000000000000000" || hexStr == "0x0" {
		a.addrStr = "0x0"
		return
	}
	a.addrStr = "0x" + hex.EncodeToString(a.Bytes())
}

// IsZero returns true if an addres is a zero value or 0x0.
func (a *Address) IsZero() bool {
	return len(a.addrStr) == 0 || a.addrStr == "0x0"
}

// HexToAddress returns new address with the given string
// as value.
func HexToAddress(hex string) (addr Address) {
	addr.SetHex(hex)
	return
}

func BytesToAddress(b []byte) (addr Address) {
	addr.SetBytes(b)
	addr.addrStr = "0x" + hex.EncodeToString(b)
	return
}
