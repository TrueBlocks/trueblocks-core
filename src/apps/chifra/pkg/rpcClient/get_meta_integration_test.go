//go:build integration
// +build integration

// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package rpcClient

import (
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

func Test_GetMetaData(t *testing.T) {
	rpcOptions := DefaultRpcOptions(&DefaultRpcOptionsSettings{
		Chain: utils.GetTestChain(),
	})
	rpcOptions.GetMetaData("mainnet", false)
}
