// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package version

import (
	"testing"
)

func TestVersion(t *testing.T) {
	early := Version{
		Major:  0,
		Minor:  10,
		Build:  0,
		Aspect: "",
	}
	late := Version{
		Major:  0,
		Minor:  20,
		Build:  0,
		Aspect: "",
	}

	if !early.IsEarlierThan(late) {
		t.Error("early is not earlier than late")
	}

	if late.IsEarlierThan(early) {
		t.Error("late is earlier than early")
	}
}
