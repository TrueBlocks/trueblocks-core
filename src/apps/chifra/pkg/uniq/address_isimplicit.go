package uniq

import (
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/base"
)

// IsImplicitAddress processes a transaction's 'input' data and 'output' data or an event's data field.
// Anything with 12 bytes of leading zeros but not more than 19 leading zeros (24 and 38 characters
// respectively).
func IsImplicitAddress(addr string) (base.Address, bool) {
	// Any 32-byte value smaller than this number is assumed to be a 'value'. We call them baddresses.
	// While this may seem like a lot of addresses being labeled as baddresses, it's not very many:
	// ---> 2 out of every 10000000000000000000000000000000000000000000000 are baddresses.
	tooSmall := "00000000000000000000000000000000000000ffffffffffffffffffffffffff"
	//           -------+-------+-------+-------+-------+-------+-------+-------+
	if addr <= tooSmall {
		return base.ZeroAddr, false
	}

	// Any 32-byte value with less than this many leading zeros is not an address (while addresses may
	// be packed into a 32-byte string differently, in our world, implicit addresses are 20-bytes and
	// zero padded to the left). This is the zero padding to the left.
	leftPadding := "000000000000000000000000"
	//              -------+-------+-------+
	if !strings.HasPrefix(addr, leftPadding) {
		return base.ZeroAddr, false
	}

	// Of the valid addresses, we assume any ending with this many trailing zeros is also a baddress.
	if strings.HasSuffix(addr, "00000000") {
		return base.ZeroAddr, false
	}

	// This should never happen, but avoid a panic...
	if len(addr) < 24 {
		return base.ZeroAddr, false
	}

	// Extract the implicit address and return it.
	address := base.HexToAddress("0x" + string(addr[24:]))
	return address, !address.IsPrecompile()
}
