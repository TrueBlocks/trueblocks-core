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
	ctx := context.Background()
	fetchData := func(modelChan chan types.Modeler[types.RawModeler], errorChan chan error) {
		now := time.Now()
		if opts.Globals.TestMode {
			now = time.Unix(1438269975, 0)
		}

		filenameChan := make(chan cache.CacheFileInfo)
		q := 1
		var nRoutines int

		counterMap := make(map[cache.CacheType]*simpleSingleCacheStats)
		types := []cache.CacheType{
			// cache.Cache_Abis,
			// cache.Cache_Blocks,
			// cache.Cache_Monitors,
			cache.Cache_Names,
			// cache.Cache_Recons,
			// cache.Cache_Slurps,
			// cache.Cache_Tmp,
			// cache.Cache_Traces,
			// cache.Cache_Transactions,
			// cache.Index_Bloom,
			// cache.Index_Final,
			// cache.Index_Ripe,
			// cache.Index_Staging,
			// cache.Index_Unripe,
			// cache.Index_Maps,
		}
		nRoutines = len(types)
		for _, t := range types {
			counterMap[t] = &simpleSingleCacheStats{CacheName: t.String() + "Cache", LastCached: now}
			// counterMap[cache.Cache_Abis].Estimated = estimate
			// var exp = regexp.MustCompile(`0[xX][c-fC-F]+`)
			go cache.WalkCacheFolder(opts.Globals.Chain, t, nil, filenameChan)
		}

		for result := range filenameChan {
			switch result.Type {
			case cache.None:
				nRoutines--
				if nRoutines == 0 {
					close(filenameChan)
					fmt.Println()
				}
			default:
				if cache.IsCacheType(result.Path, result.Type) {
					if opts.Globals.TestMode {
						if counterMap[result.Type].NumItems == 0 {
							counterMap[result.Type].NumItems += 100
							counterMap[result.Type].TotalSize += 1000000
						}
						continue
					} else {
						counterMap[result.Type].NumItems += q
						counterMap[result.Type].TotalSize += (int64(q) * file.FileSize(result.Path))
					}
					logger.Progress(counterMap[result.Type].NumItems%100 == 0, fmt.Sprintf("Found %d %s files", counterMap[result.Type].NumItems, result.Type))
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
	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOpts())
}

type simpleCacheStats struct {
	Status simpleStatus             `json:"status"`
	Caches []simpleSingleCacheStats `json:"caches"`
}

func (s *simpleCacheStats) Raw() *types.RawModeler {
	return nil
}

func (s *simpleCacheStats) Model(showHidden bool, format string, extraOptions map[string]any) types.Model {
	return types.Model{
		Data: map[string]interface{}{
			"status": s.Status,
			"caches": s.Caches,
		},
		Order: []string{
			"status",
			"caches",
		},
	}
}

type simpleSingleCacheStats struct {
	CacheName  string    `json:"cacheName"`
	NumItems   int       `json:"numItems"`
	TotalSize  int64     `json:"totalSize"`
	LastCached time.Time `json:"lastCached"`
	Newest     time.Time `json:"newest"`
	Oldest     time.Time `json:"oldest"`
	Estimated  bool      `json:"estimated"`
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
