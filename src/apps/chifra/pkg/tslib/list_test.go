// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package tslib

import (
	"fmt"
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/utils"
	"github.com/ethereum/go-ethereum"
)

func TestLoadSpecials(t *testing.T) {
	path := config.MustGetPathToChainConfig(utils.GetTestChain())
	t.Log("path: ", path)

	specials, err := GetSpecials(utils.GetTestChain())
	if len(specials) == 0 {
		t.Error("Could not load special blocks")
	}
	if err != nil {
		t.Error(err)
	}
}

func TestIsStringSpecialBlock(t *testing.T) {
	result := IsSpecialBlock(utils.GetTestChain(), "devcon1")
	if !result {
		t.Error("Fails for valid block name")
	}

	shouldBeFalse := IsSpecialBlock(utils.GetTestChain(), "nosuchblock")
	if shouldBeFalse {
		t.Error("Passes for invalid block name")
	}
}

func TestGetNameByValue(t *testing.T) {
	name, err := FromBnToName(utils.GetTestChain(), 2463000)
	if err != nil {
		t.Error(fmt.Errorf("block at %s returned an error: %w", "2463000", ethereum.NotFound))
	}
	if name != "tangerine_whistle" {
		t.Errorf("Wrong name: %s", name)
	}
}

func TestGetValueByName(t *testing.T) {
	value, err := FromNameToBn(utils.GetTestChain(), "tangerine_whistle")
	if err != nil {
		t.Error(fmt.Errorf("block at %s returned an error: %w", "tangerine_whistle", ethereum.NotFound))
	}
	if value != 2463000 {
		t.Errorf("Wrong value: %d", value)
	}

	// _, found = FromNameToBn(utils.GetTestChain(), "latest")
	// if !found {
	// 	t.Error(fmt.Errorf("block at %s returned an error: %w", "latest", ethereum.NotFound))
	// }
	// TODO: Turn off go testing that requires connection to a node
	// if value == 0 {
	// 	t.Error("Latest block not set")
	// }
}

func TestGetSpecials(t *testing.T) {
	specials, err := GetSpecials(utils.GetTestChain())
	if err != nil {
		t.Error(err)
	}

	if len(specials) != 41 {
		t.Error("Wrong number of special blocks ", len(specials), ". Should have 41.")
	}

	// TODO: Turn off go testing that requires connection to a node
	// for _, item := range specials {
	// 	if item.Timestamp == 0 {
	// 		t.Error("Special block ", item.Name, " with zero timestamp")
	// 	}
	// 	if item.Date == "" {
	// 		t.Error("Special block ", item.Name, " with zero timestamp")
	// 	}
	// }
}
