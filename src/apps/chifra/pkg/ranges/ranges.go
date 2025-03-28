// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package ranges

import (
	"errors"
	"fmt"
	"path/filepath"
	"regexp"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
)

type RecordRange struct {
	First uint64
	Last  uint64
}

type FileRange struct {
	First base.Blknum
	Last  base.Blknum
}
type BlockRange FileRange // sugar
type TimestampRange struct {
	First base.Timestamp
	Last  base.Timestamp
}

var NotARange = FileRange{First: base.NOPOSN, Last: base.NOPOSN}

// RangeFromFilename returns a FileRange and ignore any errors
func RangeFromFilename(path string) (blkRange FileRange) {
	rng, _ := RangeFromFilenameE(path)
	return rng
}

// RangeFromFilenameE returns a block range given a chunk filename. The format of filenames may be start-end.bin (start and end are nine digit
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
		trimmed0 := strings.TrimLeft(parts[0], "0")
		trimmed1 := strings.TrimLeft(parts[1], "0")
		blkRange.First = base.MustParseBlknum(trimmed0)
		blkRange.Last = base.MustParseBlknum(trimmed1)
	} else {
		trimmed0 := strings.TrimLeft(parts[0], "0")
		blkRange.First = base.MustParseBlknum(trimmed0)
		blkRange.Last = blkRange.First
	}

	return
}

// RangeFromRangeString returns a file range from a string
func RangeFromRangeString(rngStr string) FileRange {
	return RangeFromFilename(filepath.Join(config.PathToIndex("mainnet"), "finalized", rngStr+".bin")) // okay to use mainnet since we're only interested in range
}

func (r FileRange) String() string {
	return fmt.Sprintf("%09d-%09d", r.First, r.Last)
}

// RangeToFilename returns a fileName and existence bool given a file range and a type
func (r *FileRange) RangeToFilename(chain string) string {
	return filepath.Join(config.PathToIndex(chain), "finalized", r.String()+".bin")
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

// Precedes returns true if the range is strictly before the needle range.
// (If 'sequential' is true, then the last block in the range must be
// one less than the first block in the needle range.) If the needle range
// starts at zero, returns false (nothing is before the first range)
func (r *FileRange) Precedes(needle FileRange, sequential bool) bool {
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
func (r *FileRange) IntersectsB(bn base.Blknum) bool {
	return r.Intersects(FileRange{First: bn, Last: bn})
}

// EarlierThanB returns true if the range is strictly before the given block
func (r *FileRange) EarlierThanB(bn base.Blknum) bool {
	return r.EarlierThan(FileRange{First: bn, Last: bn})
}

// LaterThanB returns true if the range is strictly after the given block
func (r *FileRange) LaterThanB(bn base.Blknum) bool {
	return r.LaterThan(FileRange{First: bn, Last: bn})
}

// Equals returns true if the two ranges are equal
func (r *FileRange) Equals(needle FileRange) bool {
	return r.First == needle.First && r.Last == needle.Last
}

func (r *FileRange) Span() base.Blknum {
	return r.Last - r.First + 1
}

type RangeDiff struct {
	Min base.Blknum
	In  base.Blknum
	Mid base.Blknum
	Out base.Blknum
	Max base.Blknum
}

func (r *FileRange) Overlaps(test FileRange) (rd RangeDiff) {
	rd.Min = min(r.First, test.First)
	rd.In = max(r.First, test.First)
	rd.Out = min(r.Last, test.Last)
	rd.Max = max(r.Last, test.Last)
	rd.Mid = (rd.Max-rd.Min)/2 + rd.Min
	return
}
