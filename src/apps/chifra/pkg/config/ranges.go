package config

import (
	"path/filepath"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
)

// RangeFromRangeString returns a file range from a string
func RangeFromRangeString(rngStr string) base.FileRange {
	return base.RangeFromFilename(filepath.Join(PathToIndex("mainnet"), "finalized", rngStr+".bin")) // okay to use mainnet since we're only interested in range
}

// RangeToFilename returns a fileName and existence bool given a file range and a type
func RangeToFilename(chain string, r *base.FileRange) string {
	return filepath.Join(PathToIndex(chain), "finalized", r.String()+".bin")
}
