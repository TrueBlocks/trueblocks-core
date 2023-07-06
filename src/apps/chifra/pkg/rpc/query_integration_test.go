//go:build integration
// +build integration

// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package rpc

import (
	"testing"
)

func Test_Client(t *testing.T) {
	if _, err := GetTxHashFromNumberAndId("mainnet", 1001001, 0); err != nil {
		t.Error("couldn't get known transaction hash from block number and tx id")
	}
}
