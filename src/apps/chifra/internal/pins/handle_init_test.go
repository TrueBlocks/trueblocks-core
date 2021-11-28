// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package pinsPkg

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
