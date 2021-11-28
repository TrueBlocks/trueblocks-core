// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package cmd

//----------------------------------------------------------------------------
import (
	"strings"
)

// goodAddr Returns true if the address is not a precompile and not zero
func goodAddr(addr string) bool {
	// As per EIP 1352, all addresses less or equal to the following
	// value are reserved for pre-compiles. We don't index precompiles.
	// https://eips.ethereum.org/EIPS/eip-1352
	if addr <= "0x000000000000000000000000000000000000ffff" {
		return false
	}
	return true
}

// potentialAddress Processing 'input' value, 'output' value or event 'data' value
// we do our best, but we don't include everything we could. We do the best we can
func potentialAddress(addr string) bool {
	// Any address smaller than this we call a 'baddress' and do not index
	small := "00000000000000000000000000000000000000ffffffffffffffffffffffffff"
	//        -------+-------+-------+-------+-------+-------+-------+-------+
	if addr <= small {
		return false
	}

	// Any address with less than this many leading zeros is not an left-padded 20-byte address
	largePrefix := "000000000000000000000000"
	//              -------+-------+-------+
	if !strings.HasPrefix(addr, largePrefix) {
		return false
	}

	if strings.HasSuffix(addr, "00000000") {
		return false
	}
	return true
}
