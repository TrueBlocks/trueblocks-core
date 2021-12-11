// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package tslibPkg

import (
	"testing"
)

func TestIsStringSpecialBlock(t *testing.T) {
	result := IsSpecialBlock("devcon1")

	if !result {
		t.Error("Fails for valid block name")
	}

	shouldBeFalse := IsSpecialBlock("nosuchblock")

	if shouldBeFalse {
		t.Error("Passes for invalid block name")
	}
}

func TestGetNameByValue(t *testing.T) {
	name, found := NameFromBn(2463000)

	if !found {
		t.Error("Block name not found")
	}

	if name != "tangerine" {
		t.Errorf("Wrong name: %s", name)
	}
}

func TestGetValueByName(t *testing.T) {
	value, found := BnFromName("tangerine")

	if !found {
		t.Error("Block not found by name")
	}

	if value != 2463000 {
		t.Errorf("Wrong value: %d", value)
	}

	value, found = BnFromName("latest")
	if !found {
		t.Error("Latest block not found")
	}
	if value == 0 {
		t.Error("Latest block not set")
	}
}

func TestGetSpecials(t *testing.T) {
	specials := GetSpecials()
	if len(specials) != 31 {
		t.Error("Wrong number of special blocks ", len(specials), ". Should have 31.")
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
