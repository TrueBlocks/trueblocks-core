// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package whenPkg

import (
	"testing"
)

func Test_Validate(t *testing.T) {
	var opts = WhenOptions{}
	opts.Blocks = append(opts.Blocks, "2014-01-01")
	err := opts.ValidateWhen()
	if err == nil {
		t.Error("Parsed an date too early", err)
	}
}
