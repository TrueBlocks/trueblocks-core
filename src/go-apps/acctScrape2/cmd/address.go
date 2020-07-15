package cmd

import (
	"bytes"
	"encoding/hex"
)

const (
	// AddressLength = number of bytes of an address
	AddressLength = 20
)

// Address is 20 bytes
type Address [AddressLength]byte


func (a Address) Compare(b Address) int {
	return bytes.Compare(a[:], b[:])
}

func (a *Address) setBytes(b []byte) {
	if len(b) > len(a) {
		b = b[len(b)-AddressLength:]
	}
	copy(a[AddressLength-len(b):], b)
}

func NewAddressFromHex(hexString string) (newAddress Address, err error) {
	if hexString[:2] == "0x" {
		hexString = hexString[2:]
	}
	hexBytes, err := hex.DecodeString(hexString)
	if err != nil {
		return Address{}, err
	}
	newAddress.setBytes(hexBytes)
	return
}
