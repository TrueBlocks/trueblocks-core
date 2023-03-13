package chunksPkg

import (
	"bufio"
	"fmt"
	"os"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/paths"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

func (opts *ChunksOptions) HandleTruncate(blockNums []uint64) error {
	if opts.Globals.TestMode {
		logger.Warn("Truncate option not tested.")
		return nil
	}

	reader := bufio.NewReader(os.Stdin)
	fmt.Printf("%s ", strings.Replace(warning, "{0}", fmt.Sprintf("%d", opts.Truncate), -1))
	text, _ := reader.ReadString('\n')
	text = strings.Replace(text, "\n", "", -1)
	if text != "" && text != "y" && text != "Y" {
		fmt.Printf("Not truncating: [%s].", text)
		return nil
	}
	indexPath := config.GetPathToIndex(opts.Globals.Chain)
	index.CleanTemporaryFolders(indexPath, true)

	truncateIndex := func(walker *index.IndexWalker, path string, first bool) (bool, error) {
		if path != paths.ToBloomPath(path) {
			logger.Fatal("should not happen ==> we're spinning through the bloom filters")
		}

		if strings.HasSuffix(path, ".gz") {
			os.Remove(path)
			return true, nil
		}

		rng, err := paths.RangeFromFilenameE(path)
		if err != nil {
			return false, err
		}
		testRange := paths.FileRange{First: opts.Truncate, Last: utils.NOPOS}
		if rng.Intersects(testRange) {
			os.Remove(paths.ToIndexPath(path))
			os.Remove(paths.ToBloomPath(path))
		}

		return true, nil
	}

	walker := index.NewIndexWalker(
		opts.Globals.Chain,
		opts.Globals.TestMode,
		100, /* maxTests */
		truncateIndex,
	)
	if err := walker.WalkBloomFilters(blockNums); err != nil {
		return err
	}

	return opts.HandleStatus(blockNums)
}

var warning = `Are sure you want to remove index chunks after and including block {0} (Yy)? `
