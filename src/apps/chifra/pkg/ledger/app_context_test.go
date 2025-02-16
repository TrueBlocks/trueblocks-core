package ledger

import (
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func TestLedgerContext1(t *testing.T) {
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
		got := newAppBalancer(test.prev, test.cur, test.next, false, false, test.reversed)
		if got.reconType != test.expected {
			t.Error("expected:", test.expected, "got:", got.reconType)
		}
	}
}

func TestNewAppBalancer(t *testing.T) {
	cases := []struct {
		name        string
		prev        base.Blknum
		cur         base.Blknum
		next        base.Blknum
		isFirst     bool
		isLast      bool
		reversed    bool
		expectRecon string
		expectFirst bool
		expertLast  bool
	}{
		{
			name: "Genesis Block",
			prev: 0, cur: 0, next: 1,
			isFirst: true, isLast: false, reversed: false,
			expectRecon: "genesis-diff",
			expectFirst: true,
		},
		{
			name: "SameSame",
			prev: 1, cur: 1, next: 1,
			isFirst: false, isLast: false, reversed: false,
			expectRecon: "same-same",
		},
		{
			name: "DiffDiff",
			prev: 1, cur: 2, next: 3,
			isFirst: false, isLast: false, reversed: false,
			expectRecon: "diff-diff",
		},
		{
			name: "DiffSame",
			prev: 1, cur: 2, next: 2,
			isFirst: false, isLast: false, reversed: false,
			expectRecon: "diff-same",
		},
		{
			name: "SameDiff",
			prev: 2, cur: 2, next: 3,
			isFirst: false, isLast: false, reversed: false,
			expectRecon: "same-diff",
		},
		{
			name: "Invalid Block Order",
			prev: 3, cur: 2, next: 1,
			isFirst: false, isLast: false, reversed: false,
			expectRecon: "invalid",
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
			appBal := newAppBalancer(tc.prev, tc.cur, tc.next, tc.isFirst, tc.isLast, tc.reversed)
			if appBal.Recon().String() != tc.expectRecon {
				t.Errorf("expected %v, got %v", tc.expectRecon, appBal.ReconStr())
			}
			if appBal.reversed != tc.reversed {
				t.Errorf("expected reversed %v, got %v", tc.reversed, appBal.reversed)
			}
		})
	}
}
