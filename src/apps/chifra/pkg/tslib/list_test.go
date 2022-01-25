// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package tslibPkg

import (
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
)

func TestLoadSpecials(t *testing.T) {
	path := config.GetPathToChainConfig(GetTestChain())
	t.Log("path: ", path)

	specials, err := GetSpecials(GetTestChain())
	if len(specials) == 0 {
		t.Error("Could not load special blocks")
	}
	if err != nil {
		t.Error(err)
	}
}

func TestIsStringSpecialBlock(t *testing.T) {
	result := IsSpecialBlock(GetTestChain(), "devcon1")
	if !result {
		t.Error("Fails for valid block name")
	}

	shouldBeFalse := IsSpecialBlock(GetTestChain(), "nosuchblock")
	if shouldBeFalse {
		t.Error("Passes for invalid block name")
	}
}

func TestGetNameByValue(t *testing.T) {
	name, found := NameFromBn(GetTestChain(), 2463000)
	if !found {
		t.Error("Block name not found")
	}
	if name != "tangerine" {
		t.Errorf("Wrong name: %s", name)
	}
}

func TestGetValueByName(t *testing.T) {
	value, found := BnFromName(GetTestChain(), "tangerine")
	if !found {
		t.Error("Block not found by name")
	}
	if value != 2463000 {
		t.Errorf("Wrong value: %d", value)
	}

	// _, found = BnFromName(GetTestChain(), "latest")
	// if !found {
	// 	t.Error("Latest block not found")
	// }
	// TODO: Turn off go testing that requires connection to a node
	// if value == 0 {
	// 	t.Error("Latest block not set")
	// }
}

func TestGetSpecials(t *testing.T) {
	specials, err := GetSpecials(GetTestChain())
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

// GetTestChain is duplicated in multiple packages to avoid dependancies. See
// https://stackoverflow.com/questions/49789055/
func GetTestChain() string {
	return "mainnet"
}
