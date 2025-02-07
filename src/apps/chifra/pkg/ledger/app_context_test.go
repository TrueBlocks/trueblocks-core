package ledger

import (
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func TestLedgerContext(t *testing.T) {
	tests := []struct {
		prev     base.Blknum
		cur      base.Blknum
		next     base.Blknum
		reversed bool
		expected types.ReconType
	}{
		{0, 0, 0, false, types.Genesis},
		{0, 0, 1, false, types.Genesis},
		{1, 0, 1, false, types.Invalid},
		{12, 13, 14, false, types.DiffDiff},
		{12, 12, 13, false, types.SameDiff},
		{12, 13, 13, false, types.DiffSame},
		{12, 12, 12, false, types.SameSame},
		{10, 9, 9, false, types.Invalid},
		{10, 10, 9, false, types.Invalid},
		{10, 9, 8, false, types.Invalid},
	}

	for _, test := range tests {
		// got := newLedgerContext(test.prev, test.cur, test.next, i == 0, false, test.reversed)
		got := newLedgerContext(test.prev, test.cur, test.next, false, false, test.reversed)
		if got.ReconType != test.expected {
			t.Error("expected:", test.expected, "got:", got.ReconType)
		}
	}
}
