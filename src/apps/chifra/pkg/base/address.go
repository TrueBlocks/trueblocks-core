package base

import (
	"bytes"
	"encoding/hex"
	"errors"
	"fmt"
	"path/filepath"
	"strings"

	"github.com/ethereum/go-ethereum/common"
	"golang.org/x/crypto/sha3"
)

// Address is a wrapper for go-ethereum's Address type that always
// return lower case hex.
type Address struct {
	common.Address `json:"address"`
}

// A few well-known address. ZeroAddr, of course, is 0x0. NotAMonitor is a marker to signify the end
// of the monitor list produced by ListMonitors. SentinelAddr is for misconfigured miner appearances
var (
	NotAMonitor  = HexToAddress("0x1234deaddeaddead98766789deaddeaddead4321")
	SentinelAddr = HexToAddress("0xdeaddeaddeaddeaddeaddeaddeaddeaddeaddead")
	ZeroAddr     = HexToAddress("0x0")
)

// Hex returns string representation of an address
func (a *Address) Hex() string {
	if a.IsZero() {
		return "0x0"
	}
	return bytesToAddressString(a.Address.Bytes())
}

func (a *Address) DefaultSymbol() string {
	return a.Hex()[:Min(len(a.Hex()), 6)]
}

func (a Address) String() string {
	return a.Hex()
}

// Format is used by Stringer don't remove
func (a Address) Format(s fmt.State, c rune) {
	_, _ = s.Write([]byte(a.Hex()))
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

// UnmarshalJSON implements the json.Unmarshaler interface for Address.
// It supports multiple representations of a "zero" or uninitialized address. If the JSON input is
// "0x0", an empty string, or the literal null, the Address is set to ZeroAddr. Otherwise, it delegates
// to the embedded Address type's UnmarshalJSON method to handle a full-length hexadecimal address.
func (e *Address) UnmarshalJSON(data []byte) error {
	s := string(data)
	if s == "\"0x0\"" || s == "\"\"" || s == "null" {
		*e = ZeroAddr
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

// HexToAddress returns new address with the given string as value.
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

func (a Address) Equal(b Address) bool {
	return bytes.Equal(a.Bytes(), b.Bytes())
}

func (a Address) LessThan(b Address) bool {
	return bytes.Compare(a.Bytes(), b.Bytes()) < 0
}

func (a *Address) LessThanOrEqual(b Address) bool {
	return a.LessThan(b) || a.Equal(b)
}

func (a Address) GreaterThan(b Address) bool {
	return !a.LessThanOrEqual(b)
}

func (a Address) GreaterThanOrEqual(b Address) bool {
	return !a.LessThan(b)
}

// As per EIP 1352, all addresses less or equal to the following value are reserved for pre-compiles.
// We don't index precompiles. https://eips.ethereum.org/EIPS/eip-1352
var maxPrecompileAddr = HexToAddress("0x000000000000000000000000000000000000ffff")

// IsPrecompile Returns true if the address is not a precompile (and not the zero address by extension)
func (a *Address) IsPrecompile() bool {
	return a.LessThanOrEqual(maxPrecompileAddr)
}

// AddressFromPath returns an address from a path -- is assumes the filename is
// a valid address starting with 0x and ends with the fileType. if the path does
// not contain an address, an error is returned. If the path does not end with the
// given fileType, it panics.
func AddressFromPath(path, fileType string) (Address, error) {
	_, fileName := filepath.Split(path)

	if !strings.HasSuffix(fileName, fileType) {
		return ZeroAddr, fmt.Errorf("file name %q does not have the expected file type suffix %q", fileName, fileType)
	}

	if !strings.HasPrefix(fileType, ".") {
		return ZeroAddr, fmt.Errorf("file type %q should have a leading dot", fileType)
	}

	// Check that the fileName is long enough to contain a valid address and the fileType.
	// A valid address is 42 characters (including "0x"). We require a '.' to separate the address from the file type.
	if len(fileName) < (42+len(fileType)) || !strings.HasPrefix(fileName, "0x") || !strings.Contains(fileName, ".") {
		return ZeroAddr, fmt.Errorf("path %q does not appear to contain a valid address", path)
	}

	parts := strings.Split(fileName, ".")
	return HexToAddress(parts[0]), nil
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
		if strings.ContainsAny(val, " \t\n\r") {
			return false, nil
		}
		return true, nil
	}
	return isValidHex(val, 20)
}

// FAKE_ETH_ADDRESS is the address we use to represent ETH in the ledgers
var FAKE_ETH_ADDRESS = HexToAddress("0xeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee")

// GetTestPublisher does not get customized per chain. We can only test against mainnet currently
func GetTestPublisher() Address {
	return HexToAddress("0xf503017d7baf7fbc0fff7492b751025c6a78179b")
}

// CheckSum returns the checksum address of the given address. We might want this, for example,
// to pick up ABIs and source code from Sourcify which requires checksum addresses.
func (a Address) CheckSum() string {
	str := a.Hex()[2:]
	str = strings.ToLower(str)
	hash := sha3.NewLegacyKeccak256()
	hash.Write([]byte(str))
	hashSum := hash.Sum(nil)
	hashStr := hex.EncodeToString(hashSum)
	result := "0x"
	for i := 0; i < len(str); i++ {
		if hashStr[i] >= '8' {
			result += strings.ToUpper(string(str[i]))
		} else {
			result += string(str[i])
		}
	}
	return result
}
