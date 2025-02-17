package uniq

import (
	"fmt"
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
)

func TestAddAddressToMaps(t *testing.T) {
	testCases := []struct {
		name       string
		input      base.Address
		isInserted bool
		normalized base.Address
	}{
		{
			name:       "Precompile address not inserted",
			input:      base.HexToAddress("0x1"),
			isInserted: false,
			normalized: base.HexToAddress("0x1"),
		},
		{
			name:       "Address with prefix used as-is",
			input:      base.HexToAddress("0xabcdef123456abcdef123456abcdef123456abcd"),
			isInserted: true,
			normalized: base.HexToAddress("0xabcdef123456abcdef123456abcdef123456abcd"),
		},
	}

	testBn := base.Blknum(42)
	testId := base.Txnum(100)
	for _, tc := range testCases {
		tc := tc
		t.Run(tc.name, func(t *testing.T) {
			addrMap := make(AddressBooleanMap)
			addAddressToMaps(tc.input, testBn, testId, addrMap)
			key := fmt.Sprintf(AppearanceFmt, tc.normalized.Hex(), testBn, testId)
			if addrMap[key] != tc.isInserted {
				if tc.isInserted {
					t.Errorf("Expected normalized address %q to be inserted (key %q present), but it was not", tc.normalized.Hex(), key)
				} else {
					t.Errorf("Expected precompile address %q to not be inserted (key %q absent), but it was", tc.normalized.Hex(), key)
				}
			}
		})
	}
}
