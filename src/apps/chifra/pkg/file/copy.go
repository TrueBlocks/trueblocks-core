// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package file

import (
	"io"
	"os"
)

func Copy(destPath, sourcePath string) (int64, error) {
	source, err := os.Open(sourcePath)
	if err != nil {
		return 0, err
	}
	defer source.Close()

	dest, err := os.OpenFile(destPath, os.O_WRONLY|os.O_CREATE, 0644)
	if err != nil {
		return 0, err
	}
	defer dest.Close()

	return io.Copy(dest, source)
}
