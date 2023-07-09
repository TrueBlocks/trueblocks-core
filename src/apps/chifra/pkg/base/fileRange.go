// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package base

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
type BlockRange FileRange  // sugar
type RecordRange FileRange // sugar

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

func (r FileRange) String() string {
	return fmt.Sprintf("%09d-%09d", r.First, r.Last)
}

// RangeToFilename returns a fileName and and existance bool given a file range and a type
func (r *FileRange) RangeToFilename(chain string) (bool, string) {
	fileName := config.GetPathToIndex(chain) + "finalized/" + r.String() + ".bin"
	return file.FileExists(fileName), fileName
}

// Follows returns true if the range is strictly after the needle range.
// (If 'sequential' is true, then the first block in the range must be
// one more than the last block in the needle range.)
func (r *FileRange) Follows(needle FileRange, sequential bool) bool {
	if sequential {
		return r.First == needle.Last+1
	}
	return r.LaterThan(needle)
}

// Preceeds returns true if the range is strictly before the needle range.
// (If 'sequential' is true, then the last block in the range must be
// one less than the first block in the needle range.) If the needle range
// starts at zero, returns false (nothing is before the first range)
func (r *FileRange) Preceeds(needle FileRange, sequential bool) bool {
	if sequential {
		if needle.First == 0 {
			return false
		}
		return r.Last == needle.First-1
	}
	return r.EarlierThan(needle)
}

// Intersects returns true if the two ranges intersect
func (r *FileRange) Intersects(needle FileRange) bool {
	return !r.EarlierThan(needle) && !r.LaterThan(needle)
}

// EarlierThan returns true if range is strictly before the given needle range
func (r *FileRange) EarlierThan(needle FileRange) bool {
	return r.Last < needle.First
}

// LaterThan returns true if range is strictly after the given needle range
func (r *FileRange) LaterThan(needle FileRange) bool {
	return r.First > needle.Last
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

// Equals returns true if the two ranges are equal
func (r *FileRange) Equals(needle FileRange) bool {
	return r.First == needle.First && r.Last == needle.Last
}
