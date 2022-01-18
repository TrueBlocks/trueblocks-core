// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package tslibPkg

import (
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
)

func TestIsStringSpecialBlock(t *testing.T) {
	if noSpecialFile() {
		return
	}

	result := IsSpecialBlock("mainnet", "devcon1")
	if !result {
		t.Error("Fails for valid block name")
	}

	shouldBeFalse := IsSpecialBlock("mainnet", "nosuchblock")
	if shouldBeFalse {
		t.Error("Passes for invalid block name")
	}
}

func TestGetNameByValue(t *testing.T) {
	if noSpecialFile() {
		return
	}

	name, found := NameFromBn("mainnet", 2463000)
	if !found {
		t.Error("Block name not found")
	}
	if name != "tangerine" {
		t.Errorf("Wrong name: %s", name)
	}
}

func TestGetValueByName(t *testing.T) {
	if noSpecialFile() {
		return
	}

	value, found := BnFromName("mainnet", "tangerine")
	if !found {
		t.Error("Block not found by name")
	}
	if value != 2463000 {
		t.Errorf("Wrong value: %d", value)
	}

	_, found = BnFromName("mainnet", "latest")
	if !found {
		t.Error("Latest block not found")
	}
	// TODO: Turn off go testing that requires connection to a node
	// if value == 0 {
	// 	t.Error("Latest block not set")
	// }
}

func TestGetSpecials(t *testing.T) {
	if noSpecialFile() {
		return
	}

	specials, err := GetSpecials("mainnet")
	if err != nil {
		t.Error(err)
	}

	if len(specials) != 31 {
		t.Error("Wrong number of special blocks ", len(specials), ". Should have 31.")
	}

	// TODO: Turn off go testing that requires connection to a node
	// for _, item := range specials {
	// 	if item.TimeStamp == 0 {
	// 		t.Error("Special block ", item.Name, " with zero timestamp")
	// 	}
	// 	if item.Date == "" {
	// 		t.Error("Special block ", item.Name, " with zero timestamp")
	// 	}
	// }
}

func noSpecialFile() bool {
	// The initial run of the go testing does not find this file. Let's not fail because of that
	return !file.FileExists(config.GetPathToChainConfig("mainnet") + "specials.csv")
}
