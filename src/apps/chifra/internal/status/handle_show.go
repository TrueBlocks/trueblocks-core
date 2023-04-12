package statusPkg

import (
	"context"
	"fmt"
	"strings"
	"time"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (opts *StatusOptions) HandleShow() error {
	chain := opts.Globals.Chain
	testMode := opts.Globals.TestMode

	renderCtx := context.Background()

	fetchData := func(modelChan chan types.Modeler[types.RawModeler], errorChan chan error) {
		now := time.Now()

		filenameChan := make(chan cache.CacheFileInfo)
		var nRoutines int

		counterMap := make(map[cache.CacheType]*simpleCacheItem)
		nRoutines = len(opts.ModeTypes)
		for _, mT := range opts.ModeTypes {
			mT := mT
			counterMap[mT] = NewSingleCacheStats(mT, now)
			var t CacheWalker
			t.ctx, t.cancel = context.WithCancel(context.Background())
			go cache.WalkCacheFolder(t.ctx, chain, mT, &t, filenameChan)
		}

		for result := range filenameChan {
			cT := result.Type

			switch cT {
			case cache.Cache_NotACache:
				nRoutines--
				if nRoutines == 0 {
					close(filenameChan)
					logger.Progress(true, "                                           ")
				}
			default:
				if cache.IsCacheType(result.Path, cT, !result.IsDir /* checkExt */) {
					if result.IsDir {
						counterMap[cT].NFolders++
						counterMap[cT].Path = cache.GetRootPathFromCacheType(chain, cT)
					} else {
						result.Data.(*CacheWalker).nSeen++
						if result.Data.(*CacheWalker).nSeen >= opts.FirstRecord {
							counterMap[cT].NFiles++
							counterMap[cT].SizeInBytes += file.FileSize(result.Path)
							if opts.Globals.Verbose && counterMap[cT].NFiles <= opts.MaxRecords {
								cI, _ := opts.getCacheItem(cT, result.Path)
								counterMap[cT].Items = append(counterMap[cT].Items, cI)
							}
						}
					}

					if counterMap[cT].NFiles >= opts.MaxRecords {
						result.Data.(*CacheWalker).cancel()
					}

					logger.Progress(
						result.Data.(*CacheWalker).nSeen%100 == 0,
						fmt.Sprintf("Found %d %s files", counterMap[cT].NFiles, cT))

					if (result.Data.(*CacheWalker).nSeen+1)%100000 == 0 {
						logger.Info(colors.Green, "Progress:", colors.Off, "Found", counterMap[cT].NFiles, "files and", counterMap[cT].NFolders, "folders after", result.Data.(*CacheWalker).nSeen+1, "files")
					}

				} else {
					logger.Progress(true, fmt.Sprintf("Skipped %s", result.Path))
				}
			}
		}

		status, err := opts.GetSimpleStatus()
		if err != nil {
			errorChan <- err
			return
		}

		totalRecords := uint64(0)
		for _, mT := range opts.ModeTypes {
			mT := mT
			if counterMap[mT] != nil {
				status.Caches = append(status.Caches, *counterMap[mT])
				totalRecords += counterMap[mT].NFiles
			}
		}

		if totalRecords == 0 {
			str := ""
			for _, m := range opts.Modes {
				str += fmt.Sprintf("%s ", m)
			}
			errorChan <- fmt.Errorf("no files were found in the [" + strings.Trim(str, " ") + "] caches")
			return
		}

		modelChan <- status
	}

	extra := map[string]interface{}{
		"showProgress": false,
		"testMode":     testMode,
		// "isApi":        isApi,
	}

	return output.StreamMany(renderCtx, fetchData, opts.Globals.OutputOptsWithExtra(extra))
}

type CacheWalker struct {
	ctx    context.Context
	cancel context.CancelFunc
	nSeen  uint64
}
