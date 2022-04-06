// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package cache

import (
	"errors"
	"fmt"
	"path/filepath"
	"strconv"
	"strings"
)

// TODO: BOGUS I don't like using uint64s everywhere
type FileRange struct {
	First uint64
	Last  uint64
}

// RangeFromFilename returns a block range given a chunk filename. Format of filenames may be start-end.bin (start and end are nine digit
// and zero-padded to the left) or start.txt
func RangeFromFilename(path string) (blkRange FileRange, err error) {
	_, fn := filepath.Split(path)
	if !strings.Contains(fn, ".") {
		return blkRange, errors.New("invalid path does not contain extension:" + path)
	}
	fn = strings.Split(fn, ".")[0]

	parts := strings.Split(fn, "-")
	if len(parts) > 1 {
		blkRange.First, _ = strconv.ParseUint(parts[0], 10, 32)
		blkRange.Last, _ = strconv.ParseUint(parts[1], 10, 32)
	} else {
		blkRange.First = 0
		blkRange.Last, _ = strconv.ParseUint(parts[0], 10, 32)
	}

	return
}

// FilenameFromRange builds file name in form of `range.First-range.Last`, with numbers padded. If `extension` is present,
// then it will be added to the file name as well
func FilenameFromRange(fileRange FileRange, extension string) string {
	fileName := fmt.Sprintf(
		"%09d-%09d",
		fileRange.First,
		fileRange.Last,
	)

	if extension != "" {
		return fmt.Sprintf("%s.%s", fileName, extension)
	}

	return fileName
}

func Intersects(r1, r2 FileRange) bool {
	return !(r1.Last < r2.First || r1.First > r2.Last)
}

func BlockIntersects(r FileRange, blk uint64) bool {
	return !(blk < r.First || blk > r.Last)
}
