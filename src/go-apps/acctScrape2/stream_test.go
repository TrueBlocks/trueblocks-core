// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package main

// import (
// 	"encoding/csv"
// 	"encoding/json"
// 	"os"
// 	"testing"
// )

// // use in conjunction with stramtest.sh
// // where the data folder has 10,000 test files in it

// func testStreamJSON(t *testing.T, fileName string) {
// 	var modAppArray []modifiedAppearanceRecord
// 	f, err := os.Open(fileName)
// 	if err != nil {
// 		t.Fatal(err)
// 	}
// 	err = json.NewDecoder(f).Decode(&modAppArray)
// 	if err != nil {
// 		t.Fatal(err)
// 	}
// 	if len(modAppArray) != 230000 {
// 		t.Fatal("found ", len(modAppArray), "expected : 230,000 from 10,000 files tested")
// 	}
// }

// func TestStreamJSON(t *testing.T) {
// 	testStreamJSON(t, "./stream.json")
// }

// func TestNoStreamJSON(t *testing.T) {
// 	testStreamJSON(t, "./nostream.json")
// }

// func testStreamCSV(t *testing.T, fileName string) {
// 	var modAppArray []modifiedAppearanceRecord
// 	f, err := os.Open(fileName)
// 	if err != nil {
// 		t.Fatal(err)
// 	}
// 	records, err := csv.NewReader(f).ReadAll()
// 	if err != nil {
// 		t.Fatal(err)
// 	}
// 	if len(records) != 230001 {
// 		t.Fatal("found ", len(modAppArray), "expected : 230,000 from 10,000 files tested")
// 	}
// }

// func TestStreamCSV(t *testing.T) {
// 	testStreamCSV(t, "./stream.csv")
// }

// func TestNoStreamCSV(t *testing.T) {
// 	testStreamCSV(t, "./nostream.csv")
// }

// func TestStreamTXT(t *testing.T) {
// 	testStreamCSV(t, "./stream.txt")
// }

// func TestNoStreamTXT(t *testing.T) {
// 	testStreamCSV(t, "./nostream.txt")
// }
