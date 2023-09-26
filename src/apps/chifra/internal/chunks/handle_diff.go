package chunksPkg

import (
	"context"
	"fmt"
	"io"
	"os"
	"path/filepath"
	"sort"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/walk"
)

func (opts *ChunksOptions) HandleDiff(blockNums []uint64) error {
	chain := opts.Globals.Chain

	ctx, cancel := context.WithCancel(context.Background())
	fetchData := func(modelChan chan types.Modeler[types.RawModeler], errorChan chan error) {
		showDiff := func(walker *walk.CacheWalker, path string, first bool) (bool, error) {
			return opts.handleDiff(modelChan, walker, path, first)
		}

		walker := walk.NewCacheWalker(
			chain,
			opts.Globals.TestMode,
			10000, /* maxTests */
			showDiff,
		)

		if err := walker.WalkBloomFilters(blockNums); err != nil {
			errorChan <- err
			cancel()
		}
	}

	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOpts())
}

func (opts *ChunksOptions) handleDiff(modelChan chan types.Modeler[types.RawModeler], walker *walk.CacheWalker, path string, first bool) (bool, error) {
	path = index.ToIndexPath(path)
	folder, name := filepath.Split(path)
	diffPath := os.Getenv("TB_CHUNKS_DIFFPATH")
	diffPath = filepath.Join(strings.Replace(folder, config.PathToIndex(opts.Globals.Chain), diffPath+"/", -1), name)
	if len(diffPath) > 0 && diffPath[0] != '/' {
		diffPath = "./" + diffPath
	}
	wd, _ := os.Getwd()
	diffPath = strings.Replace(diffPath, "./", wd+"/", -1)

	logger.Info("Comparing:")
	logger.Info(fmt.Sprintf("  existing: %s (%d)", path, file.FileSize(path)))
	logger.Info(fmt.Sprintf("  current:  %s (%d)", diffPath, file.FileSize(diffPath)))

	_, _ = opts.exportTo("one", path)
	_, _ = opts.exportTo("two", diffPath)
	return true, nil
}

func (opts *ChunksOptions) exportTo(dest, source string) (bool, error) {
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

	for _, app := range apps {
		fmt.Printf("%s\t%d\t%d\t%s", dest, app.BlockNumber, app.TransactionIndex, app.Address)
	}

	return true, nil
}
