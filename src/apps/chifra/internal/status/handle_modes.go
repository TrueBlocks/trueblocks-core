package statusPkg

import (
	"context"
	"errors"
	"fmt"
	"strings"
	"time"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/ranges"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/tslib"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/walk"
)

func (opts *StatusOptions) HandleModes(rCtx *output.RenderCtx) error {
	chain := opts.Globals.Chain
	testMode := opts.Globals.TestMode

	fetchData := func(modelChan chan types.Modeler, errorChan chan error) {
		now := time.Now()

		filenameChan := make(chan walk.CacheFileInfo)
		var nRoutines int

		counterMap := make(map[walk.CacheType]*types.CacheItem)
		nRoutines = len(opts.ModeTypes)
		for _, mT := range opts.ModeTypes {
			counterMap[mT] = &types.CacheItem{
				CacheItemType: walk.WalkCacheName(mT),
				Items:         make([]any, 0),
				LastCached:    now.Format("2006-01-02 15:04:05"),
			}
			var t CacheWalker
			t.ctx, t.cancel = context.WithCancel(context.Background())
			go walk.WalkCacheFolder(t.ctx, chain, mT, &t, filenameChan)
		}

		for result := range filenameChan {
			cT := result.Type

			switch cT {
			case walk.Cache_NotACache:
				nRoutines--
				if nRoutines == 0 {
					close(filenameChan)
					logger.Progress(true, "                                           ")
				}
			default:
				isIndex := func(cT walk.CacheType) bool {
					m := map[walk.CacheType]bool{
						walk.Index_Bloom:   true,
						walk.Index_Final:   true,
						walk.Index_Ripe:    true,
						walk.Index_Staging: true,
						walk.Index_Unripe:  true,
						walk.Index_Maps:    true,
					}
					return m[cT]
				}
				if testMode && isIndex(cT) && (cT != walk.Index_Bloom && cT != walk.Index_Final) {
					continue
				} else if testMode && cT == walk.Cache_Results {
					continue
				}

				if walk.IsCacheType(result.Path, cT, !result.IsDir /* checkExt */) {
					if result.IsDir {
						counterMap[cT].NFolders++
						counterMap[cT].Path = walk.GetRootPathFromCacheType(chain, cT)
					} else {
						result.Data.(*CacheWalker).nSeen++
						if result.Data.(*CacheWalker).nSeen >= opts.FirstRecord {
							counterMap[cT].NFiles++
							counterMap[cT].SizeInBytes += file.FileSize(result.Path)
							if opts.Globals.Verbose && counterMap[cT].NFiles <= opts.MaxRecords {
								result.FileRange = ranges.RangeFromFilename(result.Path)
								result.TsRange.First, _ = tslib.FromBnToTs(chain, result.FileRange.First)
								result.TsRange.Last, _ = tslib.FromBnToTs(chain, result.FileRange.Last)
								cI, _ := walk.GetCacheItem(chain, testMode, cT, &result)
								if isIndex(cT) {
									bP := index.ToBloomPath(result.Path)
									cI["bloomSizeBytes"] = file.FileSize(bP)
									iP := index.ToIndexPath(result.Path)
									cI["indexSizeBytes"] = file.FileSize(iP)
								}
								counterMap[cT].Items = append(counterMap[cT].Items, cI)
							}
						}
					}

					if counterMap[cT].NFiles >= opts.MaxRecords {
						result.Data.(*CacheWalker).cancel()
					}

					smallMark := result.Data.(*CacheWalker).nSeen%100 == 0
					logger.Progress(
						smallMark && !utils.IsFuzzing(),
						fmt.Sprintf("Found %d %s files", counterMap[cT].NFiles, cT))

					if (result.Data.(*CacheWalker).nSeen+1)%100000 == 0 {
						logger.Info(colors.Green, "Progress:", colors.Off, "Found", counterMap[cT].NFiles, "files and", counterMap[cT].NFolders, "folders after", result.Data.(*CacheWalker).nSeen+1, "files")
					}

				} else {
					logger.Progress(!utils.IsFuzzing(), fmt.Sprintf("Skipped %s", result.Path))
				}
			}
		}

		status, err := opts.GetStatus(false)
		if err != nil {
			errorChan <- err
			return
		}

		totalRecords := uint64(0)
		for _, mT := range opts.ModeTypes {
			if counterMap[mT] != nil {
				status.Caches = append(status.Caches, *counterMap[mT])
				totalRecords += counterMap[mT].NFiles
			}
		}

		if totalRecords == 0 {
			str := ""
			for _, m := range opts.Modes {
				str += m + " "
			}
			errorChan <- errors.New("no files were found in the [" + strings.Trim(str, " ") + "] caches")
			return
		}

		modelChan <- status
	}

	extraOpts := map[string]any{
		"chains": opts.Chains,
	}

	return output.StreamMany(rCtx, fetchData, opts.Globals.OutputOptsWithExtra(extraOpts))
}

type CacheWalker struct {
	ctx    context.Context
	cancel context.CancelFunc
	nSeen  uint64
}
