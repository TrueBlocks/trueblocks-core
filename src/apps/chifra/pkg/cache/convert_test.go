// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package cache

import (
	"testing"
)

func TestConvert(t *testing.T) {
	blocks := []string{"1", "1000"}
	conv := Convert(blocks)
	expected := []uint64{1, 1000}
	for i := 0; i < len(blocks); i++ {
		if conv[i] != expected[i] {
			t.Error("Not converted correctly", conv[i], "!=", expected[i])
		}
	}
}
