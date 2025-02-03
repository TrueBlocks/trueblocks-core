package ledger

import (
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func TestNewLedgerContext(t *testing.T) {
	cases := []struct {
		name        string
		prev        base.Blknum
		cur         base.Blknum
		next        base.Blknum
		isFirst     bool
		isLast      bool
		reversed    bool
		expectRecon types.ReconType
	}{
		{
			name: "Genesis Block",
			prev: 0, cur: 0, next: 1,
			isFirst: true, isLast: false, reversed: false,
			expectRecon: types.Genesis | types.First,
		},
		{
			name: "SameSame",
			prev: 1, cur: 1, next: 1,
			isFirst: false, isLast: false, reversed: false,
			expectRecon: types.SameSame,
		},
		{
			name: "DiffDiff",
			prev: 1, cur: 2, next: 3,
			isFirst: false, isLast: false, reversed: false,
			expectRecon: types.DiffDiff,
		},
		{
			name: "DiffSame",
			prev: 1, cur: 2, next: 2,
			isFirst: false, isLast: false, reversed: false,
			expectRecon: types.DiffSame,
		},
		{
			name: "SameDiff",
			prev: 2, cur: 2, next: 3,
			isFirst: false, isLast: false, reversed: false,
			expectRecon: types.SameDiff,
		},
		{
			name: "Invalid Block Order",
			prev: 3, cur: 2, next: 1,
			isFirst: false, isLast: false, reversed: false,
			expectRecon: types.Invalid,
		},
		// {
		// 	name: "Reversed DiffDiff",
		// 	prev: 3, cur: 2, next: 1,
		// 	isFirst: false, isLast: false, reversed: true,
		// 	expectRecon: types.DiffDiff,
		// },
		// {
		// 	name: "Reversed DiffSame",
		// 	prev: 2, cur: 2, next: 1,
		// 	isFirst: false, isLast: false, reversed: true,
		// 	expectRecon: types.DiffSame,
		// },
		// {
		// 	name: "Reversed SameDiff",
		// 	prev: 3, cur: 3, next: 2,
		// 	isFirst: false, isLast: false, reversed: true,
		// 	expectRecon: types.DiffSame,
		// },
	}

	for _, tc := range cases {
		t.Run(tc.name, func(t *testing.T) {
			ctx := newLedgerContext(tc.prev, tc.cur, tc.next, tc.isFirst, tc.isLast, tc.reversed)
			if ctx.ReconType != tc.expectRecon {
				t.Errorf("expected %v, got %v", tc.expectRecon, ctx.ReconType)
			}
			if ctx.Reversed != tc.reversed {
				t.Errorf("expected reversed %v, got %v", tc.reversed, ctx.Reversed)
			}
		})
	}
}
