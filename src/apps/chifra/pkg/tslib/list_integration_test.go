//go:build integration
// +build integration

// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package tslib

import (
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

func TestGetValueByName2(t *testing.T) {
	value, err := FromNameToBn(utils.GetTestChain(), "tangerine")
	if err != nil {
		t.Error("Block tangerine not found by name")
	}
	if value != 2463000 {
		t.Errorf("Wrong value: %d", value)
	}
	_, err = FromNameToBn(utils.GetTestChain(), "latest")
	if err != nil {
		t.Error("Latest block not found")
	}
	if value == 0 {
		t.Error("Latest block not set")
	}
}

func TestGetSpecials2(t *testing.T) {
	specials, err := GetSpecials(utils.GetTestChain())
	if err != nil {
		t.Error(err)
	}

	if len(specials) != 33 {
		t.Error("Wrong number of special blocks ", len(specials), ". Should have 33.")
	}

	for _, item := range specials {
		if item.TimeStamp == 0 {
			t.Error("Special block ", item.Name, " with zero timestamp")
		}
		if item.Date == "" {
			t.Error("Special block ", item.Name, " with zero timestamp")
		}
	}
}
