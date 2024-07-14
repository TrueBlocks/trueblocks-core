package chunksPkg

import (
	"context"
	"encoding/binary"
	"fmt"
	"io"
	"os"
	"path/filepath"
	"sort"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/walk"
)

var nVisited int

func (opts *ChunksOptions) HandleDiff(rCtx output.RenderCtx, blockNums []base.Blknum) error {
	chain := opts.Globals.Chain
	testMode := opts.Globals.TestMode

	ctx, cancel := context.WithCancel(context.Background())
	fetchData := func(modelChan chan types.Modeler, errorChan chan error) {
		walker := walk.NewCacheWalker(
			chain,
			testMode,
			10000, /* maxTests */
			func(walker *walk.CacheWalker, path string, first bool) (bool, error) {
				return opts.handleDiff(chain, path)
			},
		)

		logger.Info("Walking the bloom files at...", config.PathToIndex(chain))
		if !file.FolderExists(config.PathToIndex(chain)) {
			logger.Fatal(fmt.Sprintf("The index folder does not exist: [%s]", config.PathToIndex(chain)))
		}

		if err := walker.WalkBloomFilters(blockNums); err != nil {
			errorChan <- err
			cancel()
		}

		if nVisited == 0 {
			logger.Warn("No bloom filters were visited. Does the block number exist in the finalized folder?")
		}
	}

	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOpts())
}

func (opts *ChunksOptions) handleDiff(chain, path string) (bool, error) {
	nVisited++

	srcPath, diffPath, rd := opts.getParams(chain, path)

	logger.Info("Comparing:")
	logger.Info(fmt.Sprintf("  current (one):  %s (%d)", srcPath, file.FileSize(srcPath)))
	logger.Info(fmt.Sprintf("  diffPath (two): %s (%d)", diffPath, file.FileSize(diffPath)))

	if _, err := opts.exportTo("one", srcPath, rd); err != nil {
		return false, err
	}

	if _, err := opts.exportTo("two", diffPath, rd); err != nil {
		return false, err
	}

	return true, nil
}

func writeArray(disp, dest, fn string, lines []string) error {
	outputFolder, _ := filepath.Abs("./" + dest)
	if !file.FolderExists(outputFolder) {
		if err := os.MkdirAll(outputFolder, os.ModePerm); err != nil {
			return err
		}
	}
	outputFile := filepath.Join(outputFolder, fmt.Sprintf("%s_%s.txt", fn, disp))
	if err := file.LinesToAsciiFile(outputFile, lines); err != nil {
		return err
	}
	logger.Info(colors.Colored(fmt.Sprintf("Wrote {%d} lines to {%s}", len(lines), outputFile)))
	return nil
}

func (opts *ChunksOptions) exportTo(dest, source string, rd base.RangeDiff) (bool, error) {
	indexChunk, err := index.OpenIndex(source, true /* check */)
	if err != nil {
		return false, err
	}
	defer indexChunk.Close()

	_, err = indexChunk.File.Seek(int64(index.HeaderWidth), io.SeekStart)
	if err != nil {
		return false, err
	}

	apps := make([]types.Appearance, 0, 500000)
	for i := 0; i < int(indexChunk.Header.AddressCount); i++ {
		s := types.AppearanceTable{}
		if err := binary.Read(indexChunk.File, binary.LittleEndian, &s.AddressRecord); err != nil {
			return false, err
		}
		if s.Appearances, err = indexChunk.ReadAppearancesAndReset(&s.AddressRecord); err != nil {
			return false, err
		}
		s.AddressRecord.Count = uint32(len(s.Appearances))
		for _, app := range s.Appearances {
			apps = append(apps, types.Appearance{
				Address:          s.AddressRecord.Address,
				BlockNumber:      app.BlockNumber,
				TransactionIndex: app.TransactionIndex,
			})
		}
	}

	sort.Slice(apps, func(i, j int) bool {
		if apps[i].BlockNumber == apps[j].BlockNumber {
			if apps[i].TransactionIndex == apps[j].TransactionIndex {
				return apps[i].Address.Hex() < apps[j].Address.Hex()
			}
			return apps[i].TransactionIndex < apps[j].TransactionIndex
		}
		return apps[i].BlockNumber < apps[j].BlockNumber
	})

	filtered := func(app types.Appearance) bool {
		return base.Txnum(app.TransactionIndex) == types.WithdrawalAmt
		// return false
	}

	pre := make([]string, 0, len(apps))
	out := make([]string, 0, len(apps))
	post := make([]string, 0, len(apps))
	for _, app := range apps {
		if !filtered(app) &&
			(app.Address != base.SentinalAddr || base.Txnum(app.TransactionIndex) != types.MisconfigReward) {
			line := fmt.Sprintf("%d\t%d\t%s", app.BlockNumber, app.TransactionIndex, app.Address)
			bn := base.Blknum(app.BlockNumber)
			if bn < rd.In {
				pre = append(pre, line)
			} else if bn > rd.Out {
				post = append(post, line)
			} else {
				out = append(out, line)
			}
		}
	}

	outFn := os.Getenv("TB_CHUCKS_DIFFOUT")
	if len(outFn) == 0 {
		outFn = fmt.Sprintf("%d", rd.Mid)
	}
	if err = writeArray("apps", dest, outFn, out); err != nil {
		return false, err
	}

	if len(pre) > 0 {
		preFn := os.Getenv("TB_CHUNKS_PREOUT")
		dets := ""
		if len(preFn) == 0 {
			preFn = fmt.Sprintf("%d-%d", rd.Min, rd.In-1)
			dets = "pre"
		}
		if err = writeArray(dets, dest, preFn, pre); err != nil {
			return false, err
		}
	}

	if len(post) > 0 {
		postFn := os.Getenv("TB_CHUNKS_POSTOUT")
		dets := ""
		if len(postFn) == 0 {
			postFn = fmt.Sprintf("%d-%d", rd.Out+1, rd.Max)
			dets = "post"
		}
		if err = writeArray(dets, dest, postFn, post); err != nil {
			return false, err
		}
	}

	return true, nil
}

// findFileByBlockNumber returns the path to a file whose range intersects the given block number.
func findFileByBlockNumber(chain, path string, bn base.Blknum) (fileName string, err error) {
	walker := walk.NewCacheWalker(
		chain,
		false,
		10000, /* maxTests */
		func(walker *walk.CacheWalker, path string, first bool) (bool, error) {
			rng := base.RangeFromFilename(path)
			if rng.IntersectsB(bn) {
				fileName = index.ToIndexPath(path)
				return false, nil // stop walking
			}
			return true, nil // continue walking
		},
	)
	return fileName, walker.WalkRegularFolder(path, []base.Blknum{bn})
}

func (opts *ChunksOptions) getParams(chain, path string) (string, string, base.RangeDiff) {
	srcPath := index.ToIndexPath(path)
	thisRange := base.RangeFromFilename(srcPath)
	middleMark := thisRange.First + (thisRange.Last-thisRange.First)/2 // this mark is used to find the diffPath
	diffPath := toDiffPath(chain, middleMark)
	diffRange := base.RangeFromFilename(diffPath)

	return srcPath, diffPath, thisRange.Overlaps(diffRange)
}

func toDiffPath(chain string, middleMark base.Blknum) string {
	diffPath := os.Getenv("TB_CHUNKS_DIFFPATH")
	if !strings.Contains(diffPath, "unchained/") {
		diffPath = filepath.Join(diffPath, "unchained/", chain, "finalized")
	}
	diffPath, _ = filepath.Abs(diffPath)
	diffPath, _ = findFileByBlockNumber(chain, diffPath, middleMark)
	if !file.FileExists(diffPath) {
		logger.Fatal(fmt.Sprintf("The diff path does not exist: [%s]", diffPath))
	}
	return diffPath
}
