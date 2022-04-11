// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package monitor

import (
	"fmt"
	"strings"
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/ethereum/go-ethereum/common"
)

func Test_Monitor_Print(t *testing.T) {
	mon := GetTestMonitor(t)
	defer func() {
		RemoveTestMonitor(&mon, t)
	}()

	// Append again, expect twice as many
	err := mon.WriteAppendApps(2002003, &testApps)
	if err != nil {
		t.Error(err)
	}
	if mon.Count() != nTests*2 {
		t.Error("Expected count", nTests*2, "for monitor, got:", mon.Count())
	}

	// The monitor should report that it has two appearances
	got := fmt.Sprintln(mon.ToJSON())
	expected := "{\"address\":\"0x049029dd41661e58f99271a0112dfd34695f7000\",\"nRecords\":6,\"fileSize\":56,\"lastScanned\":2002003}\n"
	if got != expected {
		t.Error("Expected:", expected, "Got:", got)
	}

	got = mon.String()
	expected = "0x049029dd41661e58f99271a0112dfd34695f7000\t6\t56\t2002003"
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
	err = mon.ReadAppearanceAt(mon.Count(), &got)
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

	if mon.Count() != nTests {
		t.Error("Number of records in monitor", mon.Count(), "is not as expected", nTests)
	}

	err := mon.ReadMonHeader()
	if err != nil {
		t.Error(err)
	}
	// TODO: read the header

	apps := make([]index.AppearanceRecord, mon.Count())
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
	expected := "{\"address\":\"0x049029dd41661e58f99271a0112dfd34695f7000\",\"nRecords\":3,\"fileSize\":32,\"lastScanned\":2002003}\n"
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
	expected = "0x049029dd41661e58f99271a0112dfd34695f7000\t3\t32\t2002003\ttrue"
	if got != expected {
		t.Error("Expected:", expected, "Got:", got)
	}
}

func GetTestMonitor(t *testing.T) Monitor {
	// Create a new, empty monitor
	testAddr := "0x049029dd41661e58f99271a0112dfd34695f7000"
	mon, _ := NewStagedMonitor("mainnet", testAddr, true /* testMode */)
	file.Remove(mon.Path())
	file.Touch(mon.Path())

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
	if mon.Count() != 0 {
		t.Error("New monitor file should be empty")
	}

	if len(testApps) != nTests {
		t.Error("Incorrect length for test data:", len(testApps), "should be ", nTests, ".")
	}

	// Append the appearances to the monitor
	err := mon.WriteAppendApps(2002003, &testApps)
	if err != nil {
		t.Error(err)
	}
	if mon.Count() != nTests {
		t.Error("Expected count", nTests, "for monitor, got:", mon.Count())
	}

	return mon
}

func RemoveTestMonitor(mon *Monitor, t *testing.T) {
	if !file.FileExists(mon.Path()) {
		t.Error("Monitor file should exist")
	}
	if mon.Count() != nTests && mon.Count() != nTests*2 {
		t.Error("Monitor should have three or six records, has:", mon.Count())
	}
	file.Remove(mon.Path())
}

const nTests = 3

var testApps = []index.AppearanceRecord{
	{BlockNumber: 1001001, TransactionId: 0},
	{BlockNumber: 1001002, TransactionId: 1},
	{BlockNumber: 1001003, TransactionId: 2},
}
