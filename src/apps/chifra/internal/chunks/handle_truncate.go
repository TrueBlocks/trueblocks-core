package chunksPkg

import (
	"fmt"
	"os"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/user"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

func (opts *ChunksOptions) HandleTruncate(blockNums []uint64) error {
	if opts.Globals.TestMode {
		logger.Warn("Truncate option not tested.")
		return nil
	}

	if !user.QueryUser(strings.Replace(warning, "{0}", fmt.Sprintf("%d", opts.Truncate), -1), "Not truncating") {
		return nil
	}

	indexPath := config.GetPathToIndex(opts.Globals.Chain)
	index.CleanTemporaryFolders(indexPath, true)

	truncateIndex := func(walker *index.CacheWalker, path string, first bool) (bool, error) {
		if path != cache.ToBloomPath(path) {
			logger.Fatal("should not happen ==> we're spinning through the bloom filters")
		}

		if strings.HasSuffix(path, ".gz") {
			os.Remove(path)
			return true, nil
		}

		rng, err := base.RangeFromFilenameE(path)
		if err != nil {
			return false, err
		}
		testRange := base.FileRange{First: opts.Truncate, Last: utils.NOPOS}
		if rng.Intersects(testRange) {
			os.Remove(cache.ToIndexPath(path))
			os.Remove(cache.ToBloomPath(path))
		}

		return true, nil
	}

	walker := index.NewCacheWalker(
		opts.Globals.Chain,
		opts.Globals.TestMode,
		100, /* maxTests */
		truncateIndex,
	)
	if err := walker.WalkBloomFilters(blockNums); err != nil {
		return err
	}

	logger.Info("Trucated index to", opts.Truncate, "...")
	return nil
}

var warning = `Are sure you want to remove index chunks after and including block {0} (Yy)? `
