// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package paths

import (
	"errors"
	"fmt"
	"path/filepath"
	"regexp"
	"strconv"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

type FileRange struct {
	First uint64
	Last  uint64
}

var NotARange = FileRange{First: utils.NOPOS, Last: utils.NOPOS}

// RangeFromFilename returns a FileRange and ignore any errors
func RangeFromFilename(path string) (blkRange FileRange) {
	rng, _ := RangeFromFilenameE(path)
	return rng
}

// RangeFromFilename returns a block range given a chunk filename. The format of filenames may be start-end.bin (start and end are nine digit
// and zero-padded to the left) or start.txt
func RangeFromFilenameE(path string) (blkRange FileRange, err error) {
	_, fn := filepath.Split(path)
	if strings.Contains(fn, ".") {
		fn = strings.Split(fn, ".")[0]
	} else {
		var digitCheck = regexp.MustCompile(`^[0-9]+$`)
		if !digitCheck.MatchString(strings.Replace(fn, "-", "", -1)) {
			return blkRange, errors.New("not a valid range " + fn)
		}
	}

	parts := strings.Split(fn, "-")
	if len(parts) > 1 {
		blkRange.First, _ = strconv.ParseUint(parts[0], 10, 32)
		blkRange.Last, _ = strconv.ParseUint(parts[1], 10, 32)
	} else {
		blkRange.First, _ = strconv.ParseUint(parts[0], 10, 32)
		blkRange.Last = blkRange.First
	}

	return
}

// RangeFromRangeString returns a file range from a string
func RangeFromRangeString(rngStr string) FileRange {
	return RangeFromFilename(config.GetPathToIndex("mainnet") + "finalized/" + rngStr + ".bin") // okay to use mainnet since we're only interested in range
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

// RangeToFilename returns a fileName and and existance bool given a file range and a type
func (r *FileRange) RangeToFilename(chain string, mode CacheType) (bool, string) {
	rangeStr := FilenameFromRange(*r, "")
	chunkPath := NewCachePath(chain, mode)
	fileName := chunkPath.GetFullPath(rangeStr)
	return file.FileExists(fileName), fileName
}

// Follows returns true if the range is strictly after the test range.
// (If 'sequential' is true, then the first block in the range must be
// one more than the last block in the test range.)
func (r *FileRange) Follows(test FileRange, sequential bool) bool {
	if sequential {
		return r.First == test.Last+1
	}
	return r.LaterThan(test)
}

// Preceeds returns true if the range is strictly before the test range.
// (If 'sequential' is true, then the last block in the range must be
// one less than the first block in the test range.) If the test range
// starts at zero, returns false (nothing is before the first range)
func (r *FileRange) Preceeds(test FileRange, sequential bool) bool {
	if sequential {
		if test.First == 0 {
			return false
		}
		return r.Last == test.First-1
	}
	return r.EarlierThan(test)
}

// Intersects returns true if the two ranges intersect
func (r *FileRange) Intersects(test FileRange) bool {
	return !r.EarlierThan(test) && !r.LaterThan(test)
}

// EarlierThan returns true if range is strictly before the given test range
func (r *FileRange) EarlierThan(test FileRange) bool {
	return r.Last < test.First
}

// LaterThan returns true if range is strictly after the given test range
func (r *FileRange) LaterThan(test FileRange) bool {
	return r.First > test.Last
}

// IntersectsB returns true if the block is inside the range (inclusive on both ends)
func (r *FileRange) IntersectsB(blk uint64) bool {
	return r.Intersects(FileRange{First: blk, Last: blk})
}

// EarlierThanB returns true if the range is strictly before the given block
func (r *FileRange) EarlierThanB(blk uint64) bool {
	return r.EarlierThan(FileRange{First: blk, Last: blk})
}

// LaterThanB returns true if the range is strictly after the given block
func (r *FileRange) LaterThanB(blk uint64) bool {
	return r.LaterThan(FileRange{First: blk, Last: blk})
}
