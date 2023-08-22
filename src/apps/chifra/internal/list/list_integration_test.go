//go:build integration
// +build integration

// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package listPkg

import (
	"os"
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/monitor"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/tslib"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

func Test_HandleFreshenMonitors(t *testing.T) {
	tslib.EstablishTsFile(utils.GetTestChain())
	opts := globals.GlobalOptions{}
	opts.Chain = "mainnet"
	listOpts := ListOptions{
		Addrs:   []string{"0x846a9cb5593483b59bb386f5a878fbb2a0d1d8dc"},
		Silent:  true,
		Globals: opts,
	}

	// This is an address that we use for testing...early transactor but not for long so unlikely to be used for real
	mon := monitor.NewMonitor("mainnet", base.HexToAddress(listOpts.Addrs[0]), true)
	file.Remove(mon.Path())

	got := mon.String()
	expected := "0x846a9cb5593483b59bb386f5a878fbb2a0d1d8dc\t0\t0\t0"
	if expected != got {
		t.Error("Expected:", expected, "\ngot:", got)
	}

	os.Setenv("FAKE_FINAL_BLOCK", "2500000")
	monitorArray := make([]monitor.Monitor, 0, len(listOpts.Addrs))
	_, err := listOpts.HandleFreshenMonitors(&monitorArray)
	if err != nil {
		t.Error(err)
	}

	// TODO: Does FAKE_FINAL_BLOCK work? Does it have to?
	// got = mon.String()
	// expected = "0x846a9cb5593483b59bb386f5a878fbb2a0d1d8dc\t351\t2816\t0"
	// if expected != got {
	// 	t.Error("Expected:", expected, "\ngot:", got)
	// }

	os.Setenv("FAKE_FINAL_BLOCK", "")
	_, err = listOpts.HandleFreshenMonitors(&monitorArray)
	if err != nil {
		t.Error(err)
	}

	got = mon.String()
	expected = "0x846a9cb5593483b59bb386f5a878fbb2a0d1d8dc\t353\t2832\t0"
	if expected != got {
		t.Error("Expected:", expected, "\ngot:", got)
	}
}
