package statusPkg

import (
	"context"
	"fmt"
	"time"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (opts *StatusOptions) HandleShow() error {
	chain := opts.Globals.Chain
	nProcessed := 0
	nSeen := 0
	firstRecord := 0
	maxRecords := 5000
	if opts.Globals.TestMode {
		maxRecords = 100
	}

	renderCtx := context.Background()
	walkContext, cancel := context.WithCancel(context.Background())

	fetchData := func(modelChan chan types.Modeler[types.RawModeler], errorChan chan error) {
		now := time.Now()
		if opts.Globals.TestMode {
			now = time.Unix(1438269975, 0)
		}

		filenameChan := make(chan cache.CacheFileInfo)
		var nRoutines int

		counterMap := make(map[cache.CacheType]*simpleSingleCacheStats)
		nRoutines = len(opts.ModeTypes)
		for _, t := range opts.ModeTypes {
			counterMap[t] = &simpleSingleCacheStats{CacheName: t.String() + "Cache", LastCached: now.String()}
			go cache.WalkCacheFolderWithContext(walkContext, chain, t, nil, filenameChan)
		}

		for result := range filenameChan {
			switch result.Type {
			case cache.Cache_NotACache:
				nRoutines--
				if nRoutines == 0 {
					close(filenameChan)
					logger.Progress(true, "                                           ")
				}
			default:
				if nSeen >= firstRecord {
					if cache.IsCacheType(result.Path, result.Type, !result.IsDir /* checkExt */) {
						if result.IsDir {
							counterMap[result.Type].NFolders++
							counterMap[result.Type].Path = cache.GetRootPathFromCacheType(chain, result.Type)
						} else {
							counterMap[result.Type].NFiles++
							counterMap[result.Type].SizeInBytes += file.FileSize(result.Path)
						}

						logger.Progress(
							nSeen%100 == 0,
							fmt.Sprintf("Found %d %s files: %d %d %d %d", counterMap[result.Type].NFiles, result.Type, nSeen, firstRecord, nProcessed, maxRecords))

						nProcessed++

					} else {
						logger.Progress(true, fmt.Sprintf("Skipped %s", result.Path))
					}

				}
				nSeen++
			}

			if nProcessed >= maxRecords {
				cancel()
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

		for _, t := range opts.ModeTypes {
			if counterMap[t] != nil {
				s.Caches = append(s.Caches, *counterMap[t])
			}
		}

		modelChan <- &s
	}

	extra := map[string]interface{}{
		"showProgress": false,
		"testMode":     opts.Globals.TestMode,
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
			// if showHidden {
			// 	s.Caches[i].Items = append(s.Caches[i].Items, "item1", "item2", "item3")
			// }
		}
	}
	model.Data["caches"] = s.Caches
	model.Order = append(model.Order, "caches")
	return model
}

type simpleSingleCacheStats struct {
	CacheName string `json:"cacheName,omitempty"`
	// Items       []string `json:"items,omitempty"`
	LastCached  string `json:"lastCached,omitempty"`
	NFiles      int    `json:"nFiles"`
	NFolders    int    `json:"nFolders"`
	Path        string `json:"path"`
	SizeInBytes int64  `json:"sizeInBytes"`
}

//     if (!names.readBinaryCache(cacheFolder_names, "names", verbose)) {
//         string_q thePath = cacheFolder_names;
//         names.type = names.getRuntimeClass()->m_ClassName;
//         names.path = pathName("names", cacheFolder_names);
//         forEveryFileInFolder(thePath, countFiles, &names);
//         CItemCounter counter(this);
//         counter.cachePtr = &names;
//         counter.monitorArray = &names.items;
//         if (verbose) {
//             forEveryFileInFolder(thePath, noteMonitor, &counter);
//         } else {
//             forEveryFileInFolder(thePath, noteMonitor_light, &counter);
//         }
//         LOG8("\tre-writing names cache");
//         names.writeBinaryCache("names", verbose);
//     }
//     status.caches.push_back(&names);
