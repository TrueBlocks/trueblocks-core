//go:build integration
// +build integration

// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package monitor

import (
	"path/filepath"
	"strings"
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/base"
)

func Test_Monitor(t *testing.T) {
	testAddr := "0xF503017d7bAf7fbc0fff7492b751025c6a78179b"

	mon, _ := NewMonitor("mainnet", base.HexToAddress(testAddr), true /* create */)
	path := mon.Path()
	dir, fileName := filepath.Split(path)

	if !strings.Contains(dir, "/cache/mainnet/monitors/") {
		t.Error("Incorrect suffix in 'dir'. Expected: \"/cache/mainnet/monitors/\" Dir:", dir)
	}

	if testAddr+Ext == fileName {
		t.Error("Filename should be lower case: ", fileName)
	}

	if strings.ToLower(testAddr)+Ext != fileName {
		t.Error("Unexpected filename: ", fileName)
	}
}
