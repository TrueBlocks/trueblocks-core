// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package main

import (
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/go-apps/blaze/cmd"
)

func main() {
	cmd.Execute()
	os.Exit(0)
}
