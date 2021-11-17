package servePkg

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

// const output = "07807300 (  12097)- <PROG>  : Scanning 11403087 of 11854826 bloom hit false positive"

// func TestProgress(t *testing.T) {
// 	slices := MatchProgress(output)
// 	slicesLen := len(slices)

// 	if slicesLen != 4 {
// 		t.Errorf("Progress regexp should have 4 matches, but got %d", slicesLen)
// 	}

// 	if slices[1] != "Scanning" {
// 		t.Errorf("First match should be the command name, but got %s", slices[1])
// 	}

// 	if slices[2] != "11403087" {
// 		t.Errorf("Second match should be the 'done' number, but got %s", slices[2])
// 	}

// 	if slices[3] != "11854826" {
// 		t.Errorf("Third match should be total number, but got  %s", slices[3])
// 	}
// }

// func TestScan ForProgress(t *testing.T) {
// 	reader, writer := io.Pipe()

// 	go func() {
// 		fmt.Fprint(writer, output)
// 		writer.Close()
// 	}()

// 	Scan ForProgress(reader, func(cp *Comman dProgress) {
// 		if cp.Done != 11403087 {
// 			t.Errorf("Done should be 11403087, but got %d", cp.Done);
// 		}

// 		if cp.Total != 11854826 {
// 			t.Errorf("Total should be 11854826, but got %d", cp.Total);
// 		}

// 		if cp.Finished != false {
// 			t.Errorf("Finished should be false, but got %t", cp.Finished);
// 		}
// 	});
// }
