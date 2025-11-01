// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package articulate

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/abi"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/types"
)

// TestArticulateTx is a wrapper around articulateTx for testing purposes
func TestArticulateTx(tx *types.Transaction, abiMap *abi.SelectorSyncMap) (*types.Function, string, error) {
	return articulateTx(tx, abiMap)
}
