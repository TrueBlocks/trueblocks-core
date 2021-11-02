/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
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
