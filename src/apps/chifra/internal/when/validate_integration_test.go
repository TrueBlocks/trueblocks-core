//go:build integration
// +build integration

// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package whenPkg

import (
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/utils"
)

func Test_Validate(t *testing.T) {
	var opts = WhenOptions{}
	chain := utils.GetTestChain()
	opts.Conn = rpc.TempConnection(chain)
	opts.Blocks = append(opts.Blocks, "2014-01-01")
	err := opts.validateWhen()
	if err == nil {
		t.Error("Parsed a date before the first block", err)
	}
}
