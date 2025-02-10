package uniq

import (
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
)

// IsImplicitAddress processes a transaction's 'input' data and 'output' data or an event's data field.
// Anything with 12 bytes of leading zeros but not more than 19 leading zeros (24 and 38 characters
// respectively).
func IsImplicitAddress(addr string) (base.Address, bool) {
	// Any 32-byte value smaller than this number is assumed to be a 'value'. We call them baddresses.
	// While this may seem like a lot of addresses being labeled as baddresses, it's not very many:
	// ---> 2 out of every 10000000000000000000000000000000000000000000000 are baddresses.
	tooSmall := "00000000000000000000000000000000000000ffffffffffffffffffffffffff"
	//        -------+-------+-------+-------+-------+-------+-------+-------+
	if addr <= tooSmall {
		return base.ZeroAddr, false
	}

	// Any 32-byte value with less than this many leading zeros is not an address (address are 20-bytes and
	// zero padded to the left)
	largePrefix := "000000000000000000000000"
	//              -------+-------+-------+
	if !strings.HasPrefix(addr, largePrefix) {
		return base.ZeroAddr, false
	}

	// Of the valid addresses, we assume any ending with this many trailing zeros is also a baddress.
	if strings.HasSuffix(addr, "00000000") {
		return base.ZeroAddr, false
	}

	// should never happen, but avoid a panic...
	if len(addr) < 24 {
		return base.ZeroAddr, false
	}

	// extract the potential address
	address := base.HexToAddress("0x" + string(addr[24:]))
	return address, !base.IsPrecompile(address.Hex())
}
