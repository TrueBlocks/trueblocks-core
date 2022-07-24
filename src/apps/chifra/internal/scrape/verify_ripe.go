package scrapePkg

import (
	"fmt"
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config/scrape"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

func (opts *ScrapeOptions) verifyRipeFiles() bool {
	indexPath := config.GetPathToIndex(opts.Globals.Chain)
	files, err := os.ReadDir(indexPath + "ripe/")
	if err != nil {
		fmt.Println(err.Error())
		return false
	}

	prev := cache.NotARange
	for _, file := range files {
		fR, _ := cache.RangeFromFilename(file.Name())
		if prev != cache.NotARange {
			if prev != fR {
				// fmt.Println("fR:", fR.First, fR.Last, "prev:", prev.First, prev.Last)
				if !fR.Follows(prev, !scrape.AllowMissing(opts.Globals.Chain)) {
					logger.Log(logger.Error, "Current file (", file.Name(), ") does not sequentially follow previous file ", prev.First, ".")
					return false
				}
			}
		}
		prev = fR
	}

	return true
}
