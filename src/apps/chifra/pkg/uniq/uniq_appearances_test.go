package uniq

import (
	"fmt"
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
)

// TestAddAddressToMaps_Precompile verifies that a precompile address is not inserted.
func TestAddAddressToMaps_Precompile(t *testing.T) {
	addrMap := make(AddressBooleanMap, 0)
	precompileAddr := "0x1"
	addAddressToMaps(precompileAddr, base.Blknum(10), base.Txnum(20), addrMap)
	if addrMap[precompileAddr] {
		t.Errorf("Expected precompile address %q to not be inserted", precompileAddr)
	}
}

// TestAddAddressToMaps_NormalizeImplicit verifies that an address without the "0x" prefix is normalized.
func TestAddAddressToMaps_NormalizeImplicit(t *testing.T) {
	addrMap := make(AddressBooleanMap, 0)
	inputAddr := "abcdef123456" // no "0x" prefix provided
	bn := base.Blknum(42)
	txid := base.Txnum(100)
	addAddressToMaps(inputAddr, bn, txid, addrMap)
	exp := base.HexToAddress("0x" + inputAddr)
	key := fmt.Sprintf(AppearanceFmt, exp.Hex(), bn, txid)
	if !addrMap[key] {
		t.Errorf("Expected normalized address %q to be inserted", key)
	}
}

// TestAddAddressToMaps_WithPrefix verifies that an address already containing "0x" is used as-is.
func TestAddAddressToMaps_WithPrefix(t *testing.T) {
	addrMap := make(AddressBooleanMap, 0)
	explicitAddr := "0xabcdef123456abcdef123456abcdef123456abcd" // already has the "0x" prefix
	bn := base.Blknum(55)
	txid := base.Txnum(200)
	addAddressToMaps(explicitAddr, bn, txid, addrMap)
	exp := base.HexToAddress(explicitAddr)
	key := fmt.Sprintf(AppearanceFmt, exp.Hex(), bn, txid)
	if !addrMap[key] {
		t.Errorf("Expected normalized address %q to be inserted", key)
	}
}
