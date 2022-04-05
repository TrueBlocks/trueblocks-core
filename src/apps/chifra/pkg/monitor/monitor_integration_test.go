//go:build integration

// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package monitor

import (
	"fmt"
	"path/filepath"
	"strings"
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/ethereum/go-ethereum/common"
)

func Test_Monitor(t *testing.T) {
	testAddr := "0xF503017d7bAf7fbc0fff7492b751025c6a78179b"

	mon := NewMonitor("mainnet", testAddr, true /* create */, true /* testMode */)
	path := mon.Path()
	dir, fileName := filepath.Split(path)

	if !strings.HasSuffix(dir, "/config/mainnet/mocked/monitors/") {
		t.Error("Incorrect suffix in 'dir'. Expected: \"/config/mainnet/mocked/monitors/\" Dir:", dir)
	}

	if testAddr+Ext == fileName {
		t.Error("Filename should be lower case: ", fileName)
	}

	if strings.ToLower(testAddr)+Ext != fileName {
		t.Error("Unexpected filename: ", fileName)
	}
}

func Test_Monitor_Print(t *testing.T) {
	mon := GetTestMonitor(t)
	defer func() {
		RemoveTestMonitor(&mon, t)
	}()

	// Append again, expect twice as many
	count, err := mon.WriteAppearances(testApps, 2002002)
	if err != nil {
		t.Error(err)
	}
	if count != nTests*2 {
		t.Error("Expected count", nTests*2, "for monitor, got:", count)
	}

	// The monitor should report that it has two appearances
	got := fmt.Sprintln(mon.ToJSON())
	expected := "{\"address\":\"0xf503017d7baf7fbc0fff7492b751025c6a781791\",\"count\":6,\"fileSize\":56,\"lastScanned\":2002003}\n"
	if got != expected {
		t.Error("Expected:", expected, "Got:", got)
	}

	got = mon.String()
	expected = "0xf503017d7baf7fbc0fff7492b751025c6a781791\t6\t56"
	if got != expected {
		t.Error("Expected:", expected, "Got:", got)
	}
}

func Test_Monitor_ReadApp(t *testing.T) {
	mon := GetTestMonitor(t)
	defer func() {
		RemoveTestMonitor(&mon, t)
	}()

	var got index.AppearanceRecord
	err := mon.ReadAppearanceAt(0, &got)
	if err == nil {
		t.Error("Should have been 'index out of range in ReadAppearanceAt[0]' error")
	}

	expected := index.AppearanceRecord{BlockNumber: 1001001, TransactionId: 0}
	err = mon.ReadAppearanceAt(1, &got)
	if got != expected || err != nil {
		t.Error("Expected:", expected, "Got:", got, err)
	}

	expected = index.AppearanceRecord{BlockNumber: 1001002, TransactionId: 1}
	err = mon.ReadAppearanceAt(2, &got)
	if got != expected || err != nil {
		t.Error("Expected:", expected, "Got:", got, err)
	}

	expected = index.AppearanceRecord{BlockNumber: 1001003, TransactionId: 2}
	err = mon.ReadAppearanceAt(mon.Count, &got)
	if got != expected || err != nil {
		t.Error("Expected:", expected, "Got:", got, err)
	}

	err = mon.ReadAppearanceAt(4, &got)
	if err == nil {
		t.Error("Should have been 'index out of range in ReadAppearanceAt[3]' error")
	}
}

func Test_Monitor_ReadApps(t *testing.T) {
	mon := GetTestMonitor(t)
	defer func() {
		RemoveTestMonitor(&mon, t)
	}()

	if mon.Count != nTests {
		t.Error("Number of records in monitor", mon.Count, "is not as expected", nTests)
	}

	err := mon.ReadHeader()
	if err != nil {
		t.Error(err)
	}
	// TODO: read the header

	apps := make([]index.AppearanceRecord, mon.Count)
	err = mon.ReadAppearances(&apps)
	if err != nil {
		t.Error(err)
	}

	for i, app := range apps {
		if testApps[i] != app {
			t.Error("Record", i, "as read (", app, ") is not equal to testApp", testApps[i])
		}
	}
}

func Test_Monitor_Delete(t *testing.T) {
	mon := GetTestMonitor(t)
	defer func() {
		RemoveTestMonitor(&mon, t)
	}()

	// The monitor should report that it has two appearances
	got := fmt.Sprintln(mon.ToJSON())
	expected := "{\"address\":\"0xf503017d7baf7fbc0fff7492b751025c6a781791\",\"count\":3,\"fileSize\":32,\"lastScanned\":2002003}\n"
	if got != expected {
		t.Error("Expected:", expected, "Got:", got)
	}

	// Try to remove the monitor. It should not be removed because it is not deleted first
	removed, err := mon.Remove()
	if removed {
		t.Error("Should not be able to remove monitor without deleting it first")
	} else {
		t.Log("Correctly errors with: ", err)
	}
	if !file.FileExists(mon.Path()) {
		t.Error("Monitor file should exist")
	}

	wasDeleted := mon.Delete()
	t.Log(mon.ToJSON())
	if wasDeleted || !mon.Deleted {
		t.Error("Should not have been previously deleted, but it should be deleted now")
	}
	if !file.FileExists(mon.Path()) {
		t.Error("Monitor file should exist")
	}

	wasDeleted = mon.Delete()
	t.Log(mon.ToJSON())
	if !wasDeleted || !mon.Deleted {
		t.Error("Should have been previously deleted, and it should be deleted now")
	}
	if !file.FileExists(mon.Path()) {
		t.Error("Monitor file should exist")
	}

	wasDeleted = mon.UnDelete()
	t.Log(mon.ToJSON())
	if !wasDeleted || mon.Deleted {
		t.Error("Should have been previously deleted, but should no longer be")
	}
	if !file.FileExists(mon.Path()) {
		t.Error("Monitor file should exist")
	}

	wasDeleted = mon.Delete()
	t.Log(mon.ToJSON())
	if wasDeleted || !mon.Deleted {
		t.Error("Should not have been previously deleted, but it should be deleted now")
	}

	got = mon.String()
	expected = "0xf503017d7baf7fbc0fff7492b751025c6a781791\t3\t32\ttrue"
	if got != expected {
		t.Error("Expected:", expected, "Got:", got)
	}
}

func GetTestMonitor(t *testing.T) Monitor {
	// Create a new, empty monitor
	testAddr := "0xF503017d7bAf7fbc0fff7492b751025c6a781791"
	mon := NewMonitor("mainnet", testAddr, true /* create */, true /* testMode */)

	if file.FileExists(mon.Path()) {
		file.Remove(mon.Path())
		file.Touch(mon.Path())
		mon.Count = 0
	}

	if mon.Address != common.HexToAddress(testAddr) {
		t.Error("Expected:", common.HexToAddress(testAddr), "Got:", mon.Address)
	}

	if mon.GetAddrStr() != strings.ToLower(testAddr) {
		t.Error("Expected:", strings.ToLower(testAddr), "Got:", mon.GetAddrStr())
	}

	// The file should exist...
	if !file.FileExists(mon.Path()) {
		t.Error("File", mon.Path(), "should exist")
	}

	// and be empty
	if mon.Count != 0 {
		t.Error("New monitor file should be empty")
	}

	if len(testApps) != nTests {
		t.Error("Incorrect length for test data:", len(testApps), "should be ", nTests, ".")
	}

	// Append the appearances to the monitor
	count, err := mon.WriteAppearances(testApps, 2002002)
	if err != nil {
		t.Error(err)
	}
	if count != nTests {
		t.Error("Expected count", nTests, "for monitor, got:", count)
	}

	return mon
}

func RemoveTestMonitor(mon *Monitor, t *testing.T) {
	if !file.FileExists(mon.Path()) {
		t.Error("Monitor file should exist")
	}
	if mon.Count != nTests && mon.Count != nTests*2 {
		t.Error("Monitor should have three or six records, has:", mon.Count)
	}
	file.Remove(mon.Path())
}

const nTests = 3

var testApps = []index.AppearanceRecord{
	{BlockNumber: 1001001, TransactionId: 0},
	{BlockNumber: 1001002, TransactionId: 1},
	{BlockNumber: 1001003, TransactionId: 2},
}
