package index

import (
	"fmt"
	"strings"
	"sync"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/ethereum/go-ethereum/common"
	"github.com/ethereum/go-ethereum/common/hexutil"
)

var mapSync sync.Mutex

// AddToMaps adds an address to two maps. The first is a map of addresses to appearance records.
// The second is a map of addresses to booleans. The boolean map is used to build the address table in the chunk.
func AddToMaps(address string, bn, txid int, appsMap AddressAppearanceMap, addressMap AddressBooleanMap) {
	isPrecompile := !base.NotPrecompile(address)
	if isPrecompile {
		return
	}

	// Make sure we have a 20 byte '0x' prefixed string (implicit strings come in as 32-byte, non-0x-prefixed strings)
	if !strings.HasPrefix(address, "0x") {
		address = hexutil.Encode(common.HexToAddress(address).Bytes())
	}

	mapSync.Lock()
	defer mapSync.Unlock()

	key := fmt.Sprintf("%s\t%09d\t%05d", address, bn, txid)
	if !addressMap[key] {
		appsMap[address] = append(appsMap[address], AppearanceRecord{
			BlockNumber:   uint32(bn),
			TransactionId: uint32(txid),
		})
	}
	addressMap[key] = true
}

// AddImplicitToMaps determines if an address is implicit and, if so, adds it to the maps.
func AddImplicitToMaps(address string, bn, txid int, appsMap AddressAppearanceMap, addressMap AddressBooleanMap) {
	isImplicit := IsImplicitAddress(address)
	if !isImplicit {
		return
	}

	AddToMaps(address, bn, txid, appsMap, addressMap)
}
