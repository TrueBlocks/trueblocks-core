package chunksPkg

import (
	"bufio"
	"fmt"
	"os"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index/bloom"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

func (opts *ChunksOptions) truncateIndex(ctx *WalkContext, path string, first bool) (bool, error) {
	if strings.HasSuffix(path, ".gz") {
		os.Remove(path)
		return true, nil
	}

	rng, err := cache.RangeFromFilename(path)
	if err != nil {
		return false, err
	}
	testRange := cache.FileRange{First: opts.Truncate, Last: utils.NOPOS}
	if rng.Intersects(testRange) {
		os.Remove(index.ToIndexPath(path))
		os.Remove(bloom.ToBloomPath(path))
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

	ctx := WalkContext{
		VisitFunc: opts.truncateIndex,
	}
	err := opts.WalkIndexFiles(&ctx, cache.Index_Bloom, blockNums)
	if err != nil {
		return err
	}

	return opts.HandleStatus(blockNums)
}

var warning = `Are sure you want to remove index chunks after and including block {0} (Yy)? `
