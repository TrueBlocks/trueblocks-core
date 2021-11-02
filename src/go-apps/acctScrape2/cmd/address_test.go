/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
package cmd

import (
	"testing"
)

func TestNewAddressFromHex(t *testing.T) {
	var addr1 Address
	var addr2 Address
	if addr1.Compare(addr2) != 0 {
		t.Error("two new addresses should compare")
	}
}

/*
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
*/
