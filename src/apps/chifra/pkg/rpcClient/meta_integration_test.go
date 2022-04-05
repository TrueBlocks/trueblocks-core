//go:build integration

// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package rpcClient

import (
	"testing"
)

func Test_GetMetaData(t *testing.T) {
	GetMetaData("mainnet", false)
}
