// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package file

import "os"

// Empty is a shortcut function to seek to the beginning
// of a file and truncate it to 0. It does not call
// file.Sync though, making all operations in-memory only.
func Empty(file *os.File) (err error) {
	if _, err = file.Seek(0, 0); err != nil {
		return
	}
	if err = file.Truncate(0); err != nil {
		return
	}
	return
}
