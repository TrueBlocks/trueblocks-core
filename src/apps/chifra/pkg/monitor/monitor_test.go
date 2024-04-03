// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package monitor

import (
	"encoding/json"
	"fmt"
	"strings"
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/filter"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func Test_Monitor_Print(t *testing.T) {
	mon := GetTestMonitor(t)
	defer func() {
		RemoveTestMonitor(&mon, t)
	}()

	// Append again, expect twice as many
	err := mon.WriteAppearancesAppend(2002003, &testApps)
	if err != nil {
		t.Error(err)
	}
	if mon.Count() != nTests*2 {
		t.Error("Expected count", nTests*2, "for monitor, got:", mon.Count())
	}

	// The monitor should report that it has two appearances
	got := testClean(fmt.Sprintln(mon.toJson()))
	expected := "{\"address\":\"0x049029dd41661e58f99271a0112dfd34695f7000\",\"deleted\":false,\"fileSize\":56,\"lastScanned\":2002003,\"nRecords\":6}"
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

	expected := index.AppearanceRecord{BlockNumber: 1001001, TransactionIndex: 0}
	err = mon.ReadAppearanceAt(1, &got)
	if got != expected || err != nil {
		t.Error("Expected:", expected, "Got:", got, err)
	}

	expected = index.AppearanceRecord{BlockNumber: 1001002, TransactionIndex: 1}
	err = mon.ReadAppearanceAt(2, &got)
	if got != expected || err != nil {
		t.Error("Expected:", expected, "Got:", got, err)
	}

	expected = index.AppearanceRecord{BlockNumber: 1001003, TransactionIndex: 2}
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

	err := mon.ReadMonitorHeader()
	if err != nil {
		t.Error(err)
	}

	if apps, _, err := mon.ReadAndFilterAppearances(filter.NewEmptyFilter(), true /* withCount */); err != nil {
		t.Error(err)
	} else {
		for i, app := range apps {
			tA := app
			tA.Address = mon.Address
			tA.BlockNumber = testApps[i].BlockNumber
			tA.TransactionIndex = testApps[i].TransactionIndex
			if tA != app {
				t.Error("Record", i, "as read (", app, ") is not equal to testApp", tA)
			}
		}
	}
}

func Test_Monitor_Delete(t *testing.T) {
	mon := GetTestMonitor(t)
	defer func() {
		RemoveTestMonitor(&mon, t)
	}()

	// The monitor should report that it has two appearances
	got := testClean(fmt.Sprintln(mon.toJson()))
	expected := "{\"address\":\"0x049029dd41661e58f99271a0112dfd34695f7000\",\"deleted\":false,\"fileSize\":32,\"lastScanned\":2002003,\"nRecords\":3}"
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
	t.Log(mon.toJson())
	if wasDeleted || !mon.Deleted {
		t.Error("Should not have been previously deleted, but it should be deleted now")
	}
	if !file.FileExists(mon.Path()) {
		t.Error("Monitor file should exist")
	}

	wasDeleted = mon.Delete()
	t.Log(mon.toJson())
	if !wasDeleted || !mon.Deleted {
		t.Error("Should have been previously deleted, and it should be deleted now")
	}
	if !file.FileExists(mon.Path()) {
		t.Error("Monitor file should exist")
	}

	wasDeleted = mon.UnDelete()
	t.Log(mon.toJson())
	if !wasDeleted || mon.Deleted {
		t.Error("Should have been previously deleted, but should no longer be")
	}
	if !file.FileExists(mon.Path()) {
		t.Error("Monitor file should exist")
	}

	wasDeleted = mon.Delete()
	t.Log(mon.toJson())
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
	mon, _ := NewMonitorStaged("mainnet", testAddr)
	file.Remove(mon.Path())
	file.Touch(mon.Path())

	if mon.Address.Hex() != strings.ToLower(testAddr) {
		t.Error("Expected:", strings.ToLower(testAddr), "Got:", mon.Address)
	}

	if mon.Address.Hex() != strings.ToLower(testAddr) {
		t.Error("Expected:", strings.ToLower(testAddr), "Got:", mon.Address.Hex())
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
		msg := fmt.Sprintf("Incorrect length for test data: %d should be %d.", len(testApps), nTests)
		t.Error(msg)
	}

	// Append the appearances to the monitor
	err := mon.WriteAppearancesAppend(2002003, &testApps)
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
	{BlockNumber: 1001001, TransactionIndex: 0},
	{BlockNumber: 1001002, TransactionIndex: 1},
	{BlockNumber: 1001003, TransactionIndex: 2},
}

func testClean(s string) string {
	return strings.Replace(strings.Replace(s, "\n", "", -1), " ", "", -1)
}

// TODO: ...and this - making this the String and the above ToTxt?
// toJson returns a JSON object from a Monitor
func (mon Monitor) toJson() string {
	sm := types.SimpleMonitor{
		Address:     mon.Address,
		NRecords:    mon.Count(),
		FileSize:    file.FileSize(mon.Path()),
		LastScanned: mon.LastScanned,
		Deleted:     mon.Deleted,
	}
	bytes, _ := json.MarshalIndent(sm, "", "  ")
	return string(bytes)
}
