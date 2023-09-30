package chunksPkg

import (
	"context"
	"fmt"
	"io"
	"os"
	"path/filepath"
	"sort"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/walk"
)

func (opts *ChunksOptions) HandleDiff(blockNums []uint64) error {
	chain := opts.Globals.Chain
	testMode := opts.Globals.TestMode

	ctx, cancel := context.WithCancel(context.Background())
	fetchData := func(modelChan chan types.Modeler[types.RawModeler], errorChan chan error) {
		walker := walk.NewCacheWalker(
			chain,
			testMode,
			10000, /* maxTests */
			func(walker *walk.CacheWalker, path string, first bool) (bool, error) {
				return opts.handleDiff(chain, path)
			},
		)

		if err := walker.WalkBloomFilters(blockNums); err != nil {
			errorChan <- err
			cancel()
		}
	}

	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOpts())
}

func (opts *ChunksOptions) handleDiff(chain, path string) (bool, error) {
	thisPath := index.ToIndexPath(path)
	diffPath := getDiffPath(chain, thisPath)

	logger.Info("Comparing:")
	logger.Info(fmt.Sprintf("  existing: %s (%d)", thisPath, file.FileSize(thisPath)))
	logger.Info(fmt.Sprintf("  current:  %s (%d)", diffPath, file.FileSize(diffPath)))

	rng := base.RangeFromFilename(path)
	outFn := fmt.Sprintf("%d", rng.First+((rng.Last-rng.First)/2))
	if _, err := opts.exportTo("existing", thisPath, outFn); err != nil {
		return false, err
	}

	if _, err := opts.exportTo("proposed", diffPath, outFn); err != nil {
		return false, err
	}

	return true, nil
}

// getDiffPath returns the path to the diff folder.
func getDiffPath(chain, path string) (diffPath string) {
	rng := base.RangeFromFilename(path)
	diffPath = os.Getenv("TB_CHUNKS_DIFFPATH")
	if !strings.Contains(diffPath, "unchained/") {
		diffPath = filepath.Join(diffPath, "unchained/", chain, "finalized")
	}
	diffPath, _ = filepath.Abs(diffPath)
	diffPath, _ = index.FindFileByBlockNumber(chain, diffPath, rng.First+(rng.Last-rng.First)/2)
	if !file.FileExists(diffPath) {
		logger.Fatal(fmt.Sprintf("The diff path does not exist: [%s]", diffPath))
	}

	return
}

func (opts *ChunksOptions) exportTo(dest, source, outFn string) (bool, error) {
	outputFolder, _ := filepath.Abs("./" + dest)
	if !file.FolderExists(outputFolder) {
		if err := os.MkdirAll(outputFolder, os.ModePerm); err != nil {
			return false, err
		}
	}

	indexChunk, err := index.NewChunkData(source)
	if err != nil {
		return false, err
	}
	defer indexChunk.Close()

	_, err = indexChunk.File.Seek(int64(index.HeaderWidth), io.SeekStart)
	if err != nil {
		return false, err
	}

	apps := make([]types.SimpleAppearance, 0, 500000)
	for i := 0; i < int(indexChunk.Header.AddressCount); i++ {
		s := simpleAppearanceTable{}
		err := s.AddressRecord.ReadAddress(indexChunk.File)
		if err != nil {
			return false, err
		}
		if s.Appearances, err = indexChunk.ReadAppearanceRecordsAndResetOffset(&s.AddressRecord); err != nil {
			return false, err
		}
		for _, app := range s.Appearances {
			apps = append(apps, types.SimpleAppearance{
				Address:          s.AddressRecord.Address,
				BlockNumber:      app.BlockNumber,
				TransactionIndex: app.TransactionId,
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

	out := make([]string, 0, len(apps))
	for _, app := range apps {
		out = append(out, fmt.Sprintf("%d\t%d\t%s", app.BlockNumber, app.TransactionIndex, app.Address))
	}

	outputFile := filepath.Join(outputFolder, fmt.Sprintf("%s_apps.txt", outFn))
	if err = file.LinesToAsciiFile(outputFile, out); err != nil {
		return false, err
	}

	logger.Info(colors.Colored(fmt.Sprintf("Wrote {%d} lines to {%s}", len(out), outputFile)))

	return true, nil
}
