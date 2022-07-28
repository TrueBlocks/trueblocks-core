package scrapePkg

import (
	"fmt"
	"os"
	"path"

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
					// TODO: BOGUS THIS IS NEARLY IDENTICAL TO CleanIndexFolder BUT DOESN'T REMOVE STAGING - MAKE CLEANING ON FAILED PROCESSING CONSISTENET
					for _, f := range []string{"ripe", "unripe", "maps"} {
						folder := path.Join(indexPath, f)
						err := os.RemoveAll(folder)
						if err != nil {
							return false
						}
					}
					return false
				}
			}
		}
		prev = fR
	}

	return true
}
