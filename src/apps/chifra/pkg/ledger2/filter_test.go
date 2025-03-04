package ledger2

import (
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
)

func TestNewFilter(t *testing.T) {
	f := NewFilter("0xAsset", 100, 200)
	if f.AssetAddress != "0xAsset" {
		t.Fatalf("Expected AssetAddress=0xAsset, got=%s", f.AssetAddress)
	}
	if f.MinBlock != 100 {
		t.Fatalf("Expected MinBlock=100, got=%d", f.MinBlock)
	}
	if f.MaxBlock != 200 {
		t.Fatalf("Expected MaxBlock=200, got=%d", f.MaxBlock)
	}
}

func TestFilterString(t *testing.T) {
	f := NewFilter("0xTest", 1, 10)
	got := f.String()
	want := "Filter(AssetAddress=0xTest MinBlock=1 MaxBlock=10)"
	if got != want {
		t.Fatalf("String mismatch.\ngot:  %s\nwant: %s", got, want)
	}
}

func TestIsBlockInRange(t *testing.T) {
	f := NewFilter("", 50, 100)
	tests := []struct {
		block base.Blknum
		want  bool
	}{
		{49, false},
		{50, true},
		{75, true},
		{100, true},
		{101, false},
	}
	for _, tc := range tests {
		got := f.IsBlockInRange(tc.block)
		if got != tc.want {
			t.Fatalf("IsBlockInRange(%d) mismatch. got=%v want=%v", tc.block, got, tc.want)
		}
	}

	// If MaxBlock is zero, it means "no upper bound."
	f2 := NewFilter("", 10, 0)
	if !f2.IsBlockInRange(999999) {
		t.Fatalf("Expected block 999999 to be in range since MaxBlock=0 means infinite.")
	}
}

func TestMatchesAsset(t *testing.T) {
	f1 := NewFilter("0xABC", 0, 0)
	if !f1.MatchesAsset("0xABC") {
		t.Fatalf("Expected filter to match 0xABC.")
	}
	if f1.MatchesAsset("0x123") {
		t.Fatalf("Expected filter not to match 0x123.")
	}

	// If filter's asset is empty, we match anything.
	f2 := NewFilter("", 0, 0)
	if !f2.MatchesAsset("0xABC") {
		t.Fatalf("Expected filter with empty asset to match any asset.")
	}
	if !f2.MatchesAsset("") {
		t.Fatalf("Expected filter with empty asset to match empty asset as well.")
	}
}
