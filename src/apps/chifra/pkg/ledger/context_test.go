// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package ledger

import (
	"testing"
)

func TestLedgerContext(t *testing.T) {
	expected := ledgerContext{PrevBlock: 11, CurBlock: 13, NextBlock: 14, IsPrevDiff: true, IsNextDiff: true, ReconType: "diff-diff"}
	got := newLedgerContext(12, 13, 14)
	if *got != expected {
		t.Error("expected:", expected, "got:", got)
	}
}
