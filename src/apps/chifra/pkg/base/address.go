package base

import (
	"encoding/hex"
	"errors"
	"fmt"
	"log"
	"path/filepath"
	"strings"

	"github.com/ethereum/go-ethereum/common"
)

// Address is a wrapper for go-ethereum's Address type that always
// return lower case hex.
type Address struct {
	common.Address
}

// A few well-known address. ZeroAddr, of course, is 0x0. NotAMonitor is a marker to signify the end
// of the monitor list produced by ListMonitors. SentinalAddr is for misconfigured miner appearances
var (
	NotAMonitor  = HexToAddress("0x1234deaddeaddead98766789deaddeaddead4321")
	SentinalAddr = HexToAddress("0xdeaddeaddeaddeaddeaddeaddeaddeaddeaddead")
	ZeroAddr     = HexToAddress("0x0")
)

// Hex returns string representation of an address
func (a *Address) Hex() string {
	if a.IsZero() {
		return "0x0"
	}
	return bytesToAddressString(a.Address.Bytes())
}

func (a *Address) Prefix(n int) string {
	return a.Hex()[:Min(len(a.Hex()), 6)]
}

func (a *Address) Encoded32() string {
	return "000000000000000000000000" + a.Hex()[2:]
}

func (a Address) String() string {
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

// IsZero returns true if an address is a zero value or 0x0.
func (a *Address) IsZero() bool {
	v := bytesToAddressString(a.Address.Bytes())
	return v == "0x0000000000000000000000000000000000000000"
}

func (e *Address) UnmarshalJSON(data []byte) error {
	if string(data) == "\"0x0\"" || string(data) == "\"\"" || string(data) == "null" {
		return nil
	}
	return e.Address.UnmarshalJSON(data)
}

func (a *Address) Common() common.Address {
	return common.BytesToAddress(a.Bytes())
}

func (a *Address) SetCommon(c *common.Address) Address {
	return BytesToAddress(c.Bytes())
}

// HexToAddress returns new address with the given string
// as value.
func HexToAddress(hex string) (addr Address) {
	addr.SetHex(hex)
	return
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
func AddressFromPath(path, fileType string) (Address, error) {
	_, fileName := filepath.Split(path)

	if !strings.HasSuffix(fileName, fileType) {
		log.Fatal("should not happen ==> path should contain fileType")
	}

	if !strings.HasPrefix(fileType, ".") {
		log.Fatal("should not happen ==> fileType should have a leading dot")
	}

	if len(fileName) < (42+len(fileType)) || !strings.HasPrefix(fileName, "0x") || !strings.Contains(fileName, ".") {
		return ZeroAddr, errors.New("path does not appear to contain an address")
	}

	parts := strings.Split(fileName, ".")
	return HexToAddress(parts[0]), nil
}

// As per EIP 1352, all addresses less or equal to the following value are reserved for pre-compiles.
// We don't index precompiles. https://eips.ethereum.org/EIPS/eip-1352
var maxPrecompile = "0x000000000000000000000000000000000000ffff"

// IsPrecompile Returns true if the address is not a precompile and not the zero address
func IsPrecompile(addr string) bool {
	test := HexToAddress(addr) // normalizes the input as an address
	return test.Hex() <= maxPrecompile
}

func IsHex(str string) bool {
	return len(strings.Trim(str[2:], "0123456789abcdefABCDEF")) == 0
}

var ErrNoLeading0x = errors.New("hex string must start with 0x")
var ErrInvalidLength = errors.New("hex string must be an even length")
var ErrInvalidHex = errors.New("hex string must contain only hex characters")

func ValidHex(val string, nBytes int) (bool, error) {
	return isValidHex(val, nBytes)
}

func isValidHex(val string, nBytes int) (bool, error) {
	if !strings.HasPrefix(val, "0x") {
		return false, ErrNoLeading0x
	} else if len(val) != (2 + nBytes*2) {
		return false, ErrInvalidLength
	} else if !IsHex(val) {
		return false, ErrInvalidHex
	}
	return true, nil
}

func IsValidAddress(val string) bool {
	ok, _ := IsValidAddressE(val)
	return ok
}

func IsValidAddressE(val string) (bool, error) {
	if strings.HasSuffix(val, ".eth") {
		return strings.Replace(val, "\t\n\r", "", -1) == val, nil
	}
	return isValidHex(val, 20)
}

// FAKE_ETH_ADDRESS is the address we use to represent ETH in the ledgers
var FAKE_ETH_ADDRESS = HexToAddress("0xeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee")

// GetTestPublisher does not get customized per chain. We can only test against mainnet currently
func GetTestPublisher() Address {
	return HexToAddress("0xf503017d7baf7fbc0fff7492b751025c6a78179b")
}
