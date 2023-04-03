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

	ctx := context.Background()
	fetchData := func(modelChan chan types.Modeler[types.RawModeler], errorChan chan error) {
		now := time.Now()
		if opts.Globals.TestMode {
			now = time.Unix(1438269975, 0)
		}

		filenameChan := make(chan cache.CacheFileInfo)
		var nRoutines int
		// var estimated bool

		counterMap := make(map[cache.CacheType]*simpleSingleCacheStats)
		types := cache.GetCacheTypes(opts.Modes)
		nRoutines = len(types)
		for _, t := range types {
			d := now.String()
			if opts.Globals.TestMode {
				d = "2015-07-31 23:59:35 +0000 UTC"
			}
			counterMap[t] = &simpleSingleCacheStats{CacheName: t.String() + "Cache", LastCached: d}
			// counterMap[cache.Cache_Abis].Estimated = estimated
			// var exp = regexp.MustCompile(`0[xX][c-fC-F]+`)
			go cache.WalkCacheFolder(chain, t, nil, filenameChan)
		}

		for result := range filenameChan {
			switch result.Type {
			case cache.Cache_NotACache:
				nRoutines--
				if nRoutines == 0 {
					close(filenameChan)
					fmt.Println()
				}
			default:
				if cache.IsCacheType(result.Path, result.Type, !result.IsDir /* checkExt */) {
					if result.IsDir {
						counterMap[result.Type].NFolders++
						counterMap[result.Type].Path = cache.GetRootPathFromCacheType(chain, result.Type)
					} else {
						counterMap[result.Type].NFiles++
						counterMap[result.Type].SizeInBytes += file.FileSize(result.Path)
					}
					if opts.Globals.TestMode {
						counterMap[result.Type].NFiles = 0xbeef
						counterMap[result.Type].NFolders = 0xdead
						counterMap[result.Type].SizeInBytes = 0xdeadbeef
						continue
					}
					logger.Progress(
						(counterMap[result.Type].NFiles+1)%100 == 0,
						fmt.Sprintf("Found %d %s files", counterMap[result.Type].NFiles, result.Type))
					// } else {
					// 	logger.Progress(true, fmt.Sprintf("Skipped %s", result.Path))
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

		for _, t := range types {
			if counterMap[t] != nil {
				s.Caches = append(s.Caches, *counterMap[t])
			}
		}

		modelChan <- &s
	}

	extra := map[string]interface{}{
		"showProgress": false,
		"testMode":     opts.Globals.TestMode,
	}

	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOptsWithExtra(extra))
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
	if extraOptions != nil && extraOptions["testMode"] == true {
		for i := 0; i < len(s.Caches); i++ {
			s.Caches[i].Path = "--paths--"
		}
	}
	model.Data["caches"] = s.Caches
	model.Order = append(model.Order, "caches")
	return model
}

type simpleSingleCacheStats struct {
	CacheName   string `json:"cacheName,omitempty"`
	Estimated   bool   `json:"estimated,omitempty"`
	LastCached  string `json:"lastCached,omitempty"`
	NFiles      int    `json:"nFiles,omitempty"`
	NFolders    int    `json:"nFolders,omitempty"`
	Path        string `json:"path,omitempty"`
	SizeInBytes int64  `json:"sizeInBytes,omitempty"`
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
