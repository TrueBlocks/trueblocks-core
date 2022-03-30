// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package blockRange

import (
	"errors"
	"path/filepath"
	"strconv"
	"strings"
)

type FileRange struct {
	First int
	Last  int
}

// RangeFromFilename returns a block range given a chunk filename. Format of filenames may be start-end.acct.bin (start and end are nine digit
// and zero-padded to the left) or start.txt
func RangeFromFilename(path string) (blkRange FileRange, err error) {
	if !strings.Contains(path, ".") {
		return blkRange, errors.New("invalid filename")
	}
	_, fn := filepath.Split(path)
	if strings.Contains(fn, ".") {
		fn = strings.Split(fn, ".")[0]
	}
	parts := strings.Split(fn, "-")

	if len(parts) > 1 {
		blkRange.First, _ = strconv.Atoi(parts[0])
		blkRange.Last, _ = strconv.Atoi(parts[1])
	} else {
		blkRange.First = 0
		blkRange.Last, _ = strconv.Atoi(parts[0])
	}

	return
}
