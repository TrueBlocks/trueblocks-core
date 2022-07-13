package scrapePkg

import (
	"strings"
)

// isAddress Returns true if the address is not a precompile and not the zero address
func isAddress(addr string) bool {
	// As per EIP 1352, all addresses less or equal to the following value are reserved for pre-compiles.
	// We don't index precompiles. https://eips.ethereum.org/EIPS/eip-1352
	return addr > "0x000000000000000000000000000000000000ffff"
}

// isImplicitAddress processes a transaction's 'input' data and 'output' data or an event's data field.
// Anything with 12 bytes of leading zeros but not more than 19 leading zeros (24 and 38 characters
// respectively).
func isImplicitAddress(addr string) bool {
	// Any 32-byte value smaller than this number is assumed to be a 'value'. We call them baddresses.
	// While this may seem like a lot of addresses being labeled as baddresses, it's not very many:
	// ---> 2 out of every 10000000000000000000000000000000000000000000000 are baddresses.
	small := "00000000000000000000000000000000000000ffffffffffffffffffffffffff"
	//        -------+-------+-------+-------+-------+-------+-------+-------+
	if addr <= small {
		return false
	}

	// Any 32-byte value with less than this many leading zeros is not an address (address are 20-bytes and
	// zero padded to the left)
	largePrefix := "000000000000000000000000"
	//              -------+-------+-------+
	if !strings.HasPrefix(addr, largePrefix) {
		return false
	}

	// Of the valid addresses, we assume any ending with this many trailing zeros is also a baddress.
	if strings.HasSuffix(addr, "00000000") {
		return false
	}

	// extract the potential address
	return isAddress("0x" + string(addr[24:]))
}

// TODO:
// TODO: This "baddress"
// TODO:
// TODO: 0x00000000000004ee2d6d415371e298f092cc0000
// TODO:
// TODO: appears in the index but it is not an actual address. It appears only four times in the entire index.
// TODO: We know this is not an address because it only appears the event 'data' section for Transfers or Approvals
// TODO: which we know to be the value, not an address.
// TODO:
// TODO: The trouble is knowing this is a "non-chain knowledge leak." The chain itself knows nothing about
// TODO: ERC20 tokens. I'm not sure how many 'false records' (or baddresses) this would remove, but it may
// TODO: be significant given that Transfers and Approvals dominate the chain data.
// TODO:
// TODO: What we could do is this:
// TODO:
// TODO: If we're scraping a log, and
// TODO:
// TODO: 	If we see certain topics (topic[0] is a Transfer or Approval, we do not include the value
// TODO:	even if it looks like an address. This is a very slippery slope. What does 'well known' mean?
// TODO:
// TODO: Another downside; implementing this would require a full re-generation of the index and would
// TODO: change the hashes and the underlying files. In order to do this, we would require a migration that
// TODO: removes the 'old' index from the end user's machine and then downloads the new index. We can do this,
// TODO: but it feels quite precarious.
// TODO:
// TODO: My expectation is that we will eventually have to re-generate the index at some point (version 1.0?).
// TODO: We can this then.
// TODO:
