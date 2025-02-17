package uniq

import (
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
)

// TestIsImplicitAddress verifies that IsImplicitAddress returns the correct
// base.Address and boolean flag for various 32‑byte hex strings.
func TestIsImplicitAddress(t *testing.T) {
	tooSmall := "00000000000000000000000000000000000000ffffffffffffffffffffffffff"
	largePrefix := "000000000000000000000000"
	validSuffix := "f1aa581f353005ba3765b81bf52d6b1c488c2101"
	invalidSuffix := "1111111111111111111111111111111100000000"

	tests := []struct {
		name            string
		input           string
		expectedOk      bool
		expectedAddress base.Address
	}{
		{
			name:            "Too small value returns false",
			input:           tooSmall,
			expectedOk:      false,
			expectedAddress: base.ZeroAddr,
		},
		{
			name:            "Invalid prefix returns false",
			input:           "abcdefabcdefabcdefabcdef" + validSuffix,
			expectedOk:      false,
			expectedAddress: base.ZeroAddr,
		},
		{
			name:            "Trailing zeros returns false",
			input:           largePrefix + invalidSuffix,
			expectedOk:      false,
			expectedAddress: base.ZeroAddr,
		},
		{
			name:            "Input too short returns false",
			input:           "0000",
			expectedOk:      false,
			expectedAddress: base.ZeroAddr,
		},
		{
			name:            "Valid implicit address returns true",
			input:           largePrefix + validSuffix,
			expectedOk:      true,
			expectedAddress: base.HexToAddress("0x" + validSuffix),
		},
	}

	for _, tc := range tests {
		tc := tc
		t.Run(tc.name, func(t *testing.T) {
			gotAddr, gotOk := IsImplicitAddress(tc.input)
			if gotOk != tc.expectedOk {
				t.Errorf("IsImplicitAddress(%q) returned ok=%v; want %v", tc.input, gotOk, tc.expectedOk)
			}
			if gotAddr != tc.expectedAddress {
				t.Errorf("IsImplicitAddress(%q) returned address=%v; want %v", tc.input, gotAddr, tc.expectedAddress)
			}
		})
	}
}
