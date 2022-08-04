package chunksPkg

import (
	"bufio"
	"fmt"
	"os"
	"strings"
	"time"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index/bloom"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

func (opts *ChunksOptions) resetIndex(ctx *WalkContext, path string, first bool) (bool, error) {
	if strings.HasSuffix(path, ".gz") {
		os.Remove(path)
		return true, nil
	}

	rng, err := cache.RangeFromFilename(path)
	if err != nil {
		return false, err
	}
	testRange := cache.FileRange{First: opts.Reset, Last: utils.NOPOS}
	if rng.Intersects(testRange) {
		os.Remove(index.ToIndexPath(path))
		os.Remove(bloom.ToBloomPath(path))
	}

	// obj := types.SimpleIndex{
	// 	Range:           rng,
	// 	Magic:           header.Magic,
	// 	Hash:            header.Hash,
	// 	AddressCount:    header.AddressCount,
	// 	AppearanceCount: header.AppearanceCount,
	// 	Size:            file.FileSize(path),
	// }

	// err = opts.Globals.RenderObject(obj, first)
	// if err != nil {
	// 	return false, err
	// }

	return true, nil
}

func (opts *ChunksOptions) HandleReset(blockNums []uint64) error {
	reader := bufio.NewReader(os.Stdin)
	fmt.Printf("%s ", strings.Replace(warning, "{0}", fmt.Sprintf("%d", opts.Reset), -1))
	text, _ := reader.ReadString('\n')
	text = strings.Replace(text, "\n", "", -1)
	if text != "" && text != "y" && text != "Y" {
		fmt.Printf("Not resetting: [%s].", text)
		return nil
	}
	indexPath := config.GetPathToIndex(opts.Globals.Chain)
	index.CleanTemporaryFolders(indexPath, true)
	logger.Log(logger.Warning, colors.Red, "Resetting blooms and index chunks is not implemented. Only temp folders removed.", colors.Off)

	ctx := WalkContext{
		VisitFunc: opts.resetIndex,
	}
	err := opts.WalkIndexFiles(&ctx, cache.Index_Bloom, blockNums)
	if err != nil {
		return err
	}

	time.Sleep(1 * time.Second)
	return opts.HandleStatus(blockNums)
}

var warning = `Are sure you want to remove index chunks after and including block {0} (Yy)? `
