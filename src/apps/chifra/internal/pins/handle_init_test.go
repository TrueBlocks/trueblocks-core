package pinsPkg

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

import (
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/pinlib/manifest"
)

func Test_retry(t *testing.T) {
	callCounter := 0
	failingPins := []manifest.PinDescriptor{
		{
			FileName: "first",
		},
		{
			FileName: "second",
		},
		{
			FileName: "third",
		},
	}
	fakeDownload := func(p []manifest.PinDescriptor) []manifest.PinDescriptor {
		defer func() { callCounter++ }()
		switch callCounter {
		case 0:
			return failingPins
		case 1:
			if len(p) != len(failingPins) {
				t.Error("Wrong count for 2nd call")
			}
			return failingPins[2:]
		case 2:
			if len(p) != len(failingPins[2:]) {
				t.Error("Wrong count for 3nd call", len(p))
			}
			return []manifest.PinDescriptor{}
		}

		t.Fatal("Too many calls")
		return nil
	}

	totalFailed := retry(failingPins, 3, fakeDownload)

	if totalFailed != 0 {
		t.Error("Still failing", totalFailed)
	}
}
