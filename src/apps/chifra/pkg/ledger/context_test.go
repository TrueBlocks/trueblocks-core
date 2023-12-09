// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package ledger

import (
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
)

func TestLedgerContext(t *testing.T) {
	tests := []struct {
		prev     base.Blknum
		cur      base.Blknum
		next     base.Blknum
		reversed bool
		expected reconType
	}{
		{0, 0, 0, false, genesis},
		{0, 0, 1, false, genesis},
		{1, 0, 1, false, invalid},
		{12, 13, 14, false, diffDiff},
		{12, 12, 13, false, sameDiff},
		{12, 13, 13, false, diffSame},
		{12, 12, 12, false, sameSame},
		{10, 9, 9, false, invalid},
		{10, 10, 9, false, invalid},
		{10, 9, 8, false, invalid},
	}

	for _, test := range tests {
		got := newLedgerContext(test.prev, test.cur, test.next, test.reversed)
		if got.ReconType != test.expected {
			t.Error("expected:", test.expected, "got:", got.ReconType)
		}
	}
}
