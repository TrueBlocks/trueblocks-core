package statusPkg

import (
	"context"
	"fmt"
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

		counterMap := make(map[cache.CacheType]*simpleSingleCacheStats)
		nRoutines = len(opts.ModeTypes)
		for _, mT := range opts.ModeTypes {
			mT := mT
			counterMap[mT] = NewSingleCacheStats(mT, now)
			var t Thing
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
				// fmt.Println(result.Path)
				if cache.IsCacheType(result.Path, cT, !result.IsDir /* checkExt */) {
					if result.IsDir {
						counterMap[cT].NFolders++
						counterMap[cT].Path = cache.GetRootPathFromCacheType(chain, cT)
					} else {
						result.Data.(*Thing).counter++
						if result.Data.(*Thing).counter >= opts.FirstRecord {
							counterMap[cT].NFiles++
							counterMap[cT].SizeInBytes += file.FileSize(result.Path)
							if opts.Globals.Verbose && counterMap[cT].NFiles <= int(opts.MaxRecords) {
								cI, _ := opts.getCacheItem(cT, result.Path)
								counterMap[cT].Items = append(counterMap[cT].Items, cI)
							}
						}
					}

					if counterMap[cT].NFiles > int(opts.MaxRecords) {
						result.Data.(*Thing).cancel()
					}

					logger.Progress(
						result.Data.(*Thing).counter%100 == 0,
						fmt.Sprintf("Found %d %s files", counterMap[cT].NFiles, cT))

					if (result.Data.(*Thing).counter+1)%100000 == 0 {
						logger.Info(colors.Green, "Progress:", colors.Off, counterMap[cT])
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

		s := simpleCacheStats{
			Status: *status,
		}

		for _, mT := range opts.ModeTypes {
			mT := mT
			if counterMap[mT] != nil {
				s.Caches = append(s.Caches, *counterMap[mT])
			}
		}

		modelChan <- &s
	}

	extra := map[string]interface{}{
		"showProgress": false,
		"testMode":     testMode,
		// "isApi":        isApi,
	}

	return output.StreamMany(renderCtx, fetchData, opts.Globals.OutputOptsWithExtra(extra))
}

type simpleCacheStats struct {
	Status simpleStatus             `json:"status,omitempty"`
	Caches []simpleSingleCacheStats `json:"caches,omitempty"`
}

func (s *simpleCacheStats) Raw() *types.RawModeler {
	return nil
}

func (s *simpleCacheStats) Model(showHidden bool, format string, extraOptions map[string]any) types.Model {
	model := s.Status.Model(showHidden, format, extraOptions)
	if extraOptions["testMode"] == true {
		for i := 0; i < len(s.Caches); i++ {
			s.Caches[i].Path = "--paths--"
			s.Caches[i].LastCached = "--lastCached--"
			s.Caches[i].NFiles = 123
			s.Caches[i].NFolders = 456
			s.Caches[i].SizeInBytes = 789
		}
	}
	model.Data["caches"] = s.Caches
	model.Order = append(model.Order, "caches")
	// if showHidden {
	// 	model.Data["chains"] = config.GetRootConfig().Chains
	// 	model.Order = append(model.Order, "chains")
	// }

	return model
}

func NewSingleCacheStats(t cache.CacheType, now time.Time) *simpleSingleCacheStats {
	return &simpleSingleCacheStats{
		CacheType:   t.CacheName(),
		Items:       make([]any, 0),
		LastCached:  now.Format("2006-01-02 15:04:05"),
		NFiles:      0,
		NFolders:    0,
		Path:        "",
		SizeInBytes: 0,
	}
}

type simpleSingleCacheStats struct {
	CacheType   string `json:"cacheType,omitempty"`
	Items       []any  `json:"items,"`
	LastCached  string `json:"lastCached,omitempty"`
	NFiles      int    `json:"nFiles"`
	NFolders    int    `json:"nFolders"`
	Path        string `json:"path"`
	SizeInBytes int64  `json:"sizeInBytes"`
}

type Thing struct {
	ctx     context.Context
	cancel  context.CancelFunc
	counter uint64
}
