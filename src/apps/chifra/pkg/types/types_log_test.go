package types

import (
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/base"
)

func TestIsRelevant(t *testing.T) {
	tests := []struct {
		name         string
		log          Log
		search       base.Address
		checkAddress bool
		expected     bool
	}{
		{
			name: "Address match with checkAddress true",
			log: Log{
				Address: base.HexToAddress("0xdeadbeef"),
				// Data and Topics do not contain the target.
				Data:   string(base.HexToHash("0xabcdef0123456789abcdef0123456789abcdef01").Bytes()),
				Topics: []base.Hash{base.HexToHash("0xabcdef0123456789abcdef0123456789abcdef01")},
			},
			search:       base.HexToAddress("0xdeadbeef"),
			checkAddress: true,
			expected:     true,
		},
		{
			name: "Address match ignored when checkAddress false",
			log: Log{
				Address: base.HexToAddress("0xdeadbeef"),
				// Data and Topics do not contain the target.
				Data:   string(base.HexToHash("0xabcdef0123456789abcdef0123456789abcdef01").Bytes()),
				Topics: []base.Hash{base.HexToHash("0xabcdef0123456789abcdef0123456789abcdef01")},
			},
			search:       base.HexToAddress("0xdeadbeef"),
			checkAddress: false,
			expected:     false,
		},
		{
			name: "Match in Data field (address embedded in 32-byte block)",
			log: Log{
				Address: base.HexToAddress("0x12345678"), // does not match search
				Data:    string(hashWithEmbeddedAddress(base.HexToAddress("0xdeadbeef")).Bytes()),
				Topics:  []base.Hash{base.HexToHash("0xabcdef0123456789abcdef0123456789abcdef01")},
			},
			search:       base.HexToAddress("0xdeadbeef"),
			checkAddress: false,
			expected:     true,
		},
		{
			name: "Match in Topic field (address embedded in a topic)",
			log: Log{
				Address: base.HexToAddress("0x12345678"),
				Data:    string(base.HexToHash("0xabcdef0123456789abcdef0123456789abcdef01").Bytes()),
				Topics: []base.Hash{
					hashWithEmbeddedAddress(base.HexToAddress("0xdeadbeef")),
				},
			},
			search:       base.HexToAddress("0xdeadbeef"),
			checkAddress: false,
			expected:     true,
		},
		{
			name: "No match anywhere",
			log: Log{
				Address: base.HexToAddress("0xabcdef"),
				Data:    string(base.HexToHash("0x1234567890abcdef1234567890abcdef12345678").Bytes()),
				Topics:  []base.Hash{base.HexToHash("0x1234567890123456789012345678901234567890")},
			},
			search:       base.HexToAddress("0xdeadbeef"),
			checkAddress: true,
			expected:     false,
		},
		{
			name: "Multiple fields match with checkAddress true",
			log: Log{
				Address: base.HexToAddress("0x11111111"),
				Data:    string(hashWithEmbeddedAddress(base.HexToAddress("0x11111111")).Bytes()),
				Topics: []base.Hash{
					hashWithEmbeddedAddress(base.HexToAddress("0x11111111")),
				},
			},
			search:       base.HexToAddress("0x11111111"),
			checkAddress: true,
			expected:     true,
		},
		{
			name: "Multiple fields match with checkAddress false (address ignored)",
			log: Log{
				Address: base.HexToAddress("0x11111111"),
				Data:    string(hashWithEmbeddedAddress(base.HexToAddress("0x11111111")).Bytes()),
				Topics: []base.Hash{
					hashWithEmbeddedAddress(base.HexToAddress("0x11111111")),
				},
			},
			search:       base.HexToAddress("0x11111111"),
			checkAddress: false,
			expected:     true,
		},
		{
			name: "Zero address embedded in Data",
			log: Log{
				Address: base.HexToAddress("0xabcdef"),
				Data:    string(hashWithEmbeddedAddress(base.ZeroAddr).Bytes()),
				Topics:  nil,
			},
			search:       base.ZeroAddr,
			checkAddress: true,
			expected:     true,
		},
		{
			name: "Empty log fields",
			log: Log{
				Address: base.HexToAddress("0x0"),
				Data:    string(base.HexToHash("0x0000000000000000000000000000000000000000").Bytes()),
				Topics:  nil,
			},
			search:       base.HexToAddress("0xdeadbeef"),
			checkAddress: true,
			expected:     false,
		},
	}

	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			result := tt.log.IsRelevant(tt.search, tt.checkAddress)
			if result != tt.expected {
				t.Errorf("IsRelevant(%x, checkAddress=%v) = %v; expected %v",
					tt.search.Bytes(), tt.checkAddress, result, tt.expected)
			}
		})
	}
}

func hashWithEmbeddedAddress(addr base.Address) base.Hash {
	var b [32]byte
	copy(b[12:], addr.Bytes())
	return base.BytesToHash(b[:])
}
