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
	walkContext, cancel := context.WithCancel(context.Background())

	nSeen := uint64(0)

	fetchData := func(modelChan chan types.Modeler[types.RawModeler], errorChan chan error) {
		now := time.Now()

		filenameChan := make(chan cache.CacheFileInfo)
		var nRoutines int

		counterMap := make(map[cache.CacheType]*simpleSingleCacheStats)
		nRoutines = len(opts.ModeTypes)
		for _, mT := range opts.ModeTypes {
			mT := mT
			counterMap[mT] = NewSingleCacheStats(mT, now)
			go cache.WalkCacheFolderWithContext(walkContext, chain, mT, nil, filenameChan)
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
						nSeen++
						if nSeen >= opts.FirstRecord {
							counterMap[cT].NFiles++
							counterMap[cT].SizeInBytes += file.FileSize(result.Path)
							if opts.Globals.Verbose {
								cI, _ := opts.getCacheItem(cT, result.Path)
								counterMap[cT].Items = append(counterMap[cT].Items, cI)
							}
						}
					}

					if counterMap[cT].NFiles > (int(opts.MaxRecords) - 2) { // not sure why
						cancel()
					}

					logger.Progress(
						nSeen%100 == 0,
						fmt.Sprintf("Found %d %s files", counterMap[cT].NFiles, cT))

					if (nSeen+1)%100000 == 0 {
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
		CacheType:   t.CacheTypeToStr() + "Cache",
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

// // Notes - c++ code did not require the node for chifra status
// establishIndexFolders();
// establishCacheFolders();
// char hostname[HOST_NAME_MAX + 1] = {0};
// gethostname(hostname, HOST_NAME_MAX);
// char username[LOGIN_NAME_MAX + 1] = {0};
// if (getlogin_r(username, LOGIN_NAME_MAX) != 0)
//     strncpy(username, "nobody", 7);
// report if node is not running
// used to show chains and keys
// used to cache results, but it may no longer be necassary
// no longer reports on all stuff. Use maxRecords
// when counting, do not count files in monitors/staging
// //-----------------------------------------------------------------------
// Monitors
//   Non-verbose   show the addresses (if a field called addrs?) noteObject_light
//   Verbose       name the address, show deleted, show stats by opening file
// Slurps
//   Non-verbose   show the addresses (if a field called addrs?) noteObject_light
//   Verbose       do nothing as it's not a monitor path
// Names
//   Non-verbose   show the addresses (if a field called addrs?) noteObject_light
//   Verbose       do nothing as it's not a monitor path
// Blooms | Index
//   Non-verbose   just count
//   Verbose       get bounds from path, report on hashes, read internal file report on nAddrs and nApps
// Blocks | Transactions | Traces
//   Non-verbose   just count
//   Verbose       used to protect by depth (no longer)
// Abis
//   Non-verbose   just count
//   Verbose
// Abis
//     uint64_t nFunctions;
//     uint64_t nEvents;
//     uint64_t nOther;
// bool noteABI(const string_q& path, void* data) {
//     if (contains(path, "/staging"))
//         return !shouldQuit();
//     if (endsWith(path, '/')) {
//         return forEveryFileInFolder(path + "*", noteABI, data);
//     } else if (endsWith(path, ".bin") || endsWith(path, ".json")) {
//         CItemCounter* counter = reinterpret_cast<CItemCounter*>(data);
//         ASSERT(counter->options);
//         CAbiCacheItem abii;
//         abii.type = abii.getRuntimeClass()->m_ClassName;
//         string_q addr = substitute(path, "/0x", "|");
//         nextTokenClear(addr, '|');
//         abii.address = "0x" + nextTokenClear(addr, '.');
//         abii.petname = addr_2_Petname(abii.address, '-');
//         CName n;
//         findName(abii.address, n);
//         if (isTestMode()) {
//             abii.address = "---address---";
//             abii.petname = "--petname--";
//             abii.name = "--name--";
//             abii.nFunctions = abii.nEvents = abii.nOther = abii.sizeInBytes = 36963;
//         } else {
//             abii.name = n.name;
//             counter->options->abi_spec = CAbi();  // reset
//             loadAbiFile(path, &counter->options->abi_spec);
//             abii.nFunctions = counter->options->abi_spec.nFunctions();
//             abii.nEvents = counter->options->abi_spec.nEvents();
//             abii.nOther = counter->options->abi_spec.nOther();
//             abii.sizeInBytes = fileSize(path);
//         }
//         counter->abiArray->push_back(abii);
//         if (isTestMode())
//             return false;
//     }
//     return !shouldQuit();
// }
// readBinaryCache
//   looks like it had a bug. It was not refreshing or always refreshing
// writeBinaryCache
//   keeps a seperate file for regular and details
// needsRefresh
//   if testing --> true
//   dateInCacheTmp <- from the tmp folder
//   newestFileInFolder <- from the cachePath
//   modifyDateOnTrueBlocksToml <- from the config file
//   if dateInCacheTmp < newestFileInFolder --> true
//   if dateInCacheTmp < modifyDateOnTrueBlocksToml --> true
//   --> false
