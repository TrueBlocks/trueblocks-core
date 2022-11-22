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

func truncateIndex(walker *index.IndexWalker, path string, first bool) (bool, error) {
	var castOk bool
	var opts *ChunksOptions
	if opts, castOk = walker.GetOpts().(*ChunksOptions); !castOk {
		logger.Fatal("should not happen ==> cannot cast ChunksOptions in truncateIndex")
		return false, nil
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

func (opts *ChunksOptions) HandleTruncate(blockNums []uint64) error {
	if opts.Globals.TestMode {
		logger.Log(logger.Warning, "Truncate option not tested.")
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

	walker := index.NewIndexWalker(
		opts.Globals.Chain,
		opts.Globals.TestMode,
		100, /* maxTests */
		opts,
		truncateIndex,
		nil,
	)
	if err := walker.WalkBloomFilters(blockNums); err != nil {
		return err
	}

	return opts.HandleStatus(blockNums)
}

var warning = `Are sure you want to remove index chunks after and including block {0} (Yy)? `
