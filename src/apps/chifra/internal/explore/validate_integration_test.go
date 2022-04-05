//go:build integration

// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package explorePkg

import (
	"testing"
)

func Test_Explore_Validate(t *testing.T) {
	var opts ExploreOptions
	opts.Terms = append(opts.Terms, "1001001.0")
	opts.Globals.Chain = "mainnet"
	err := opts.ValidateExplore()
	if err != nil {
		t.Error("1001001.1 did not validate on mainnet")
	}

	// TODO: transaction ids should validate on all transactions in the block
	// opts.Terms = append([]string{}, "1001001.*")
	// opts.Globals.Chain = "mainnet"
	// err = opts.ValidateExplore()
	// if err != nil {
	// 	t.Error("1001001.* did not validate on mainnet")
	// }

	opts.Terms = append([]string{}, "1001001")
	opts.Globals.Chain = "mainnet"
	err = opts.ValidateExplore()
	if err != nil {
		t.Error("1001001 did not validate on mainnet")
	}

	// TODO: block ids should validate on block ranges
	// opts.Terms = append([]string{}, "1001001-1001003")
	// opts.Globals.Chain = "mainnet"
	// err = opts.ValidateExplore()
	// if err != nil {
	// 	t.Error("1001001-1001003 did not validate on mainnet")
	// }
}
