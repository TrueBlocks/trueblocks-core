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

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

type FileRange struct {
	// TODO I don't like using uint64s and uint32s everywhere - can't we use just ints sometimes?
	First uint64
	Last  uint64
}

var NotARange = FileRange{First: utils.NOPOS, Last: utils.NOPOS}

// RangeFromFilename returns a block range given a chunk filename. The format of filenames may be start-end.bin (start and end are nine digit
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

func (fR FileRange) String() string {
	return fmt.Sprintf("%09d-%09d", fR.First, fR.Last)
}

// FilenameFromRange builds file name in form of `range.First-range.Last`, with numbers padded. If `extension` is present,
// then it will be added to the file name as well
func FilenameFromRange(fileRange FileRange, extension string) string {
	fileName := fileRange.String()
	if extension != "" {
		return fmt.Sprintf("%s.%s", fileName, extension)
	}
	return fileName
}

// Intersects returns true if the two ranges intersect
func (r *FileRange) Intersects(r2 FileRange) bool {
	return !(r.Last < r2.First || r.First > r2.Last)
}

// BlockIntersects returns true if block is inside the range (inclusive on both ends)
func (r *FileRange) BlockIntersects(blk uint64) bool {
	return !(blk < r.First || blk > r.Last)
}

// EarlierThan returns true if the last block in the file is less than the given block
func (r *FileRange) EarlierThan(blk uint64) bool {
	return r.Last < blk
}

// LaterThan returns true if the first block in the file is greater than the given block
func (r *FileRange) LaterThan(blk uint64) bool {
	return r.First > blk
}

// RangeToFilename returns a fileName and and existance bool given a file range and a type
func (r *FileRange) RangeToFilename(chain string, mode CacheType) (bool, string) {
	rangeStr := FilenameFromRange(*r, "")
	chunkPath := NewCachePath(chain, mode)
	fileName := chunkPath.GetFullPath(rangeStr)
	return file.FileExists(fileName), fileName
}

// Follows returns true if the last block in the previous range is one less than
// the first block on the given range or if sequential is false, if last is
// prev.smaller than curr.first
func (curr *FileRange) Follows(prev FileRange, sequential bool) bool {
	if sequential && false {
		return curr.First == prev.Last+1
	}
	return curr.First > prev.Last
}
