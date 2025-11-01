//go:build integration
// +build integration

// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package tslib

import (
	"fmt"
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/utils"
	"github.com/ethereum/go-ethereum"
)

func TestGetValueByName2(t *testing.T) {
	value, err := FromNameToBn(utils.GetTestChain(), "tangerine_whistle")
	if err != nil {
		t.Error(fmt.Errorf("block at %s returned an error: %w", "tangerine_whistle", ethereum.NotFound))
	}
	if value != 2463000 {
		t.Errorf("Wrong value: %d", value)
	}
	_, err = FromNameToBn(utils.GetTestChain(), "latest")
	if err != nil {
		t.Error(fmt.Errorf("block at %s returned an error: %w", "latest", ethereum.NotFound))
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

	if len(specials) != 41 {
		t.Error("Wrong number of special blocks ", len(specials), ". Should have 41.")
	}

	for _, item := range specials {
		if item.Timestamp == 0 {
			t.Error("Special block ", item.Name, " with zero timestamp")
		}
	}
}
