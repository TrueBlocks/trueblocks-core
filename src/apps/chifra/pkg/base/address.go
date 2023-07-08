package base

import (
	"encoding/hex"
	"errors"
	"fmt"
	"log"
	"math/big"
	"path/filepath"
	"strings"

	"github.com/ethereum/go-ethereum/common"
)

// Address is a wrapper for go-ethereum's Address type that always
// return lower case hex.
type Address struct {
	common.Address
}

// Hex returns string representation of an address
func (a *Address) Hex() string {
	if a.IsZero() {
		return "0x0"
	}
	return bytesToAddressString(a.Address.Bytes())
}

func (a *Address) String() string {
	return a.Hex()
}

// Format is used by Stringer don't remove
func (a Address) Format(s fmt.State, c rune) {
	s.Write([]byte(a.Hex()))
}

// MarshalText is used by Stringer don't remove
func (a Address) MarshalText() ([]byte, error) {
	return []byte(a.Hex()), nil
}

// SetHex sets the address based on the provided string
func (a *Address) SetHex(hexStr string) {
	a.Address = common.HexToAddress(hexStr)
}

// IsZero returns true if an addres is a zero value or 0x0.
func (a *Address) IsZero() bool {
	v := bytesToAddressString(a.Address.Bytes())
	return v == "0x0000000000000000000000000000000000000000"
}

func (h *Address) ToCommon() common.Address {
	return common.BytesToAddress(h.Bytes())
}

func (h *Address) FromCommon(c *common.Address) Address {
	return BytesToAddress(c.Bytes())
}

// HexToAddress returns new address with the given string
// as value.
func HexToAddress(hex string) (addr Address) {
	addr.SetHex(hex)
	return
}

func BigToAddress(b *big.Int) Address {
	return BytesToAddress(b.Bytes())
}

func BytesToAddress(b []byte) (addr Address) {
	addr.SetBytes(b)
	return
}

func bytesToAddressString(addressBytes []byte) string {
	return "0x" + hex.EncodeToString(addressBytes)
}

func (a *Address) Pad32() string {
	return "000000000000000000000000" + a.Hex()[2:]
}

// AddressFromPath returns an address from a path -- is assumes the filename is
// a valid address starting with 0x and ends with the fileType. if the path does
// not contain an address, an error is returned. If the path does not end with the
// given fileType, it panics.
func AddressFromPath(path, fileType string) (string, error) {
	_, fileName := filepath.Split(path)

	if !strings.HasSuffix(fileName, fileType) {
		log.Panic("should not happen - path should contain fileType")
	}

	if !strings.HasPrefix(fileType, ".") {
		log.Panic("should not happen - fileType should have a leading dot")
	}

	if len(fileName) < (42+len(fileType)) || !strings.HasPrefix(fileName, "0x") || !strings.Contains(fileName, ".") {
		return "", errors.New("path does not appear to contain an address")
	}

	parts := strings.Split(fileName, ".")
	return strings.ToLower(parts[0]), nil
}

// As per EIP 1352, all addresses less or equal to the following value are reserved for pre-compiles.
// We don't index precompiles. https://eips.ethereum.org/EIPS/eip-1352
var maxPrecompile = "0x000000000000000000000000000000000000ffff"

// IsPrecompile Returns true if the address is not a precompile and not the zero address
func IsPrecompile(addr string) bool {
	test := HexToAddress(addr) // normalizes the input as an address
	return test.Hex() <= maxPrecompile
}

// FAKE_ETH_ADDRESS is the address we use to represent ETH in the ledgers
var FAKE_ETH_ADDRESS = HexToAddress("0xeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee")
