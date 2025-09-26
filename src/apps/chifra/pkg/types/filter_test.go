package types

import (
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/ranges"
)

func TestNewFilter(t *testing.T) {
	filter := NewFilter(
		true,  // reversed
		false, // reverted
		[]string{"0x1234"},
		ranges.BlockRange{First: 100, Last: 200},
		ranges.RecordRange{First: 10, Last: 50},
	)

	if !filter.Reversed {
		t.Error("Expected Reversed to be true")
	}
	if filter.reverted {
		t.Error("Expected reverted to be false")
	}
	if len(filter.fourBytes) != 1 || filter.fourBytes[0] != "0x1234" {
		t.Error("Expected fourBytes to contain '0x1234'")
	}
	if filter.nSeen != -1 {
		t.Error("Expected nSeen to be initialized to -1")
	}
}

func TestPassesCountFilter(t *testing.T) {
	filter := NewFilter(false, false, []string{}, ranges.BlockRange{}, ranges.RecordRange{First: 2, Last: 5})

	// First call: nSeen goes from -1 to 0, below first (2)
	passed, finished := filter.PassesCountFilter()
	if passed || finished {
		t.Error("Expected first call to fail (below first record)")
	}

	// Second call: nSeen = 1, still below first (2)
	passed, finished = filter.PassesCountFilter()
	if passed || finished {
		t.Error("Expected second call to fail (below first record)")
	}

	// Third call: nSeen = 2, equals first (2), should pass, nExported becomes 1
	passed, finished = filter.PassesCountFilter()
	if !passed || finished {
		t.Error("Expected third call to pass (at first record)")
	}

	// Fourth call: nSeen = 3, should pass, nExported becomes 2
	passed, finished = filter.PassesCountFilter()
	if !passed || finished {
		t.Error("Expected fourth call to pass")
	}

	// Fifth call: nSeen = 4, should pass, nExported becomes 3
	passed, finished = filter.PassesCountFilter()
	if !passed || finished {
		t.Error("Expected fifth call to pass")
	}

	// Sixth call: nSeen = 5, should pass, nExported becomes 4
	passed, finished = filter.PassesCountFilter()
	if !passed || finished {
		t.Error("Expected sixth call to pass")
	}

	// Seventh call: nSeen = 6, should pass, nExported becomes 5 (reaches limit)
	passed, finished = filter.PassesCountFilter()
	if !passed || finished {
		t.Error("Expected seventh call to pass")
	}

	// Eighth call: nSeen = 7, nExported is 5 which >= limit, should finish
	passed, finished = filter.PassesCountFilter()
	if passed || !finished {
		t.Error("Expected eighth call to finish (reached max records)")
	}
}

func TestPassesTxFilter(t *testing.T) {
	tests := []struct {
		name      string
		reverted  bool
		fourBytes []string
		tx        Transaction
		expected  bool
	}{
		{
			name:      "no filters - should pass",
			reverted:  false,
			fourBytes: []string{},
			tx:        Transaction{IsError: false, Input: "0xabcd"},
			expected:  true,
		},
		{
			name:      "reverted filter - pass error tx",
			reverted:  true,
			fourBytes: []string{},
			tx:        Transaction{IsError: true, Input: "0xabcd"},
			expected:  true,
		},
		{
			name:      "reverted filter - fail non-error tx",
			reverted:  true,
			fourBytes: []string{},
			tx:        Transaction{IsError: false, Input: "0xabcd"},
			expected:  false,
		},
		{
			name:      "fourbyte filter - match",
			reverted:  false,
			fourBytes: []string{"0x1234"},
			tx:        Transaction{IsError: false, Input: "0x1234abcd"},
			expected:  true,
		},
		{
			name:      "fourbyte filter - no match",
			reverted:  false,
			fourBytes: []string{"0x1234"},
			tx:        Transaction{IsError: false, Input: "0x5678abcd"},
			expected:  false,
		},
		{
			name:      "both filters - reverted fail overrides fourbyte match",
			reverted:  true,
			fourBytes: []string{"0x1234"},
			tx:        Transaction{IsError: false, Input: "0x1234abcd"},
			expected:  false,
		},
	}

	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			filter := NewFilter(false, tt.reverted, tt.fourBytes, ranges.BlockRange{}, ranges.RecordRange{})
			result := filter.PassesTxFilter(&tt.tx)
			if result != tt.expected {
				t.Errorf("Expected %v, got %v", tt.expected, result)
			}
		})
	}
}

func TestPassesLogFilter(t *testing.T) {
	addr1 := base.HexToAddress("0x1234567890123456789012345678901234567890")
	addr2 := base.HexToAddress("0xabcdefabcdefabcdefabcdefabcdefabcdefabcd")

	log := Log{
		Address: addr1,
		Topics:  []base.Hash{base.HexToHash("0x1111"), base.HexToHash("0x2222")},
		Data:    "0xdeadbeef",
	}

	filter := NewEmptyFilter()

	// Should match when address is in the array
	addrArray := []base.Address{addr1, addr2}
	if !filter.PassesLogFilter(&log, addrArray) {
		t.Error("Expected log to match when address is in array")
	}

	// Should not match when address is not in the array
	addrArray = []base.Address{addr2}
	if filter.PassesLogFilter(&log, addrArray) {
		t.Error("Expected log not to match when address is not in array")
	}
}

func TestSort(t *testing.T) {
	records := []AppRecord{
		{BlockNumber: 200, TransactionIndex: 1},
		{BlockNumber: 100, TransactionIndex: 2},
		{BlockNumber: 100, TransactionIndex: 1},
		{BlockNumber: 200, TransactionIndex: 0},
	}

	// Test ascending sort
	filter := NewFilter(false, false, []string{}, ranges.BlockRange{}, ranges.RecordRange{})
	filter.Sort(records)

	expected := []AppRecord{
		{BlockNumber: 100, TransactionIndex: 1},
		{BlockNumber: 100, TransactionIndex: 2},
		{BlockNumber: 200, TransactionIndex: 0},
		{BlockNumber: 200, TransactionIndex: 1},
	}

	for i, record := range records {
		if record.BlockNumber != expected[i].BlockNumber || record.TransactionIndex != expected[i].TransactionIndex {
			t.Errorf("Sort failed at index %d: got (%d,%d), expected (%d,%d)",
				i, record.BlockNumber, record.TransactionIndex, expected[i].BlockNumber, expected[i].TransactionIndex)
		}
	}
}
