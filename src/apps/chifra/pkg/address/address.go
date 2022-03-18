package address

import (
	"bytes"
	"encoding/hex"
)

const (
	// AddressLength = number of bytes of an address
	AddressLength = 20
)

// EthAddress is 20 bytes
type EthAddress [AddressLength]byte

// Compares two addresses
func (a EthAddress) Compare(b EthAddress) int {
	return bytes.Compare(a[:], b[:])
}

// Hex_2_Address Converts a hex string to an address
func Hex_2_Address(hexString string) (newAddress EthAddress, err error) {
	if hexString[:2] == "0x" {
		hexString = hexString[2:]
	}

	hexBytes, err := hex.DecodeString(hexString)
	if err != nil {
		return EthAddress{}, err
	}

	if len(hexBytes) > len(newAddress) {
		hexBytes = hexBytes[len(hexBytes)-AddressLength:]
	}
	copy(newAddress[AddressLength-len(hexBytes):], hexBytes)

	return
}
