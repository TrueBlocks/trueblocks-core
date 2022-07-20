// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package file

import (
	"io"
	"os"
)

// TODO: BOGUS - FOR SOME REASON, GOLANG CHOSE TO REVERSE THE COMMAND LINE SEMANTICS - WE SHOULD BE GO-LIKE HERE AND REVERSE OUR SEMANTICS
func Copy(sourcePath, destPath string) (int64, error) {
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

	// TODO: BOGUS - FOR SOME REASON, GOLANG CHOSE TO REVERSE THE COMMAND LINE SEMANTICS - WE SHOULD BE GO-LIKE HERE AND REVERSE OUR SEMANTICS
	return io.Copy(dest, source)
}
