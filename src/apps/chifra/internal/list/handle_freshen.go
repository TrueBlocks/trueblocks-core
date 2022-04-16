// TODO: BOGUS -- Check out develop -- run chifra list <addr> -- check out update -- re-run -- should remove the old monitor files
// TODO: BOGUS - MUST REMOVE OLD .acct.bin files and auto upgrade to .mon.bin file

// TODO: BOGUS - if opts.Sleep != 14 || rpcClient.DistanceFromHead(opts.Globals.Chain) <= (2 * opts.UnripeDist) { DOESN'T WORK
// TODO: BOGUS - WALK THROUGH THE MONITOR FOLDER LOOKING FOR .acct.bin files report an error
// const needsMigration_30_0 string = `
// 	Old style monitor files found. Please run {0}.
// 	See https://github.com/TrueBlocks/trueblocks-core/blob/develop/MIGRATIONS.md
// 	[{VERSION}]
// `
// 	if !file.FileExists(config.GetPathToChainConfig("") + "migration.0.30.0") {
// 		msg := strings.Replace(needsMigration_30_0, "{0}", "{chifra status --migration all}", -1)
// 		msg = strings.Replace(msg, "[{VERSION}]", versionText, -1)
// 		msg = strings.Replace(msg, "{", colors.Green, -1)
// 		msg = strings.Replace(msg, "}", colors.Off, -1)
// 		log.Fatalf(msg)
// 	}
//--------------------------------------------------------------------------------
// bool migrateOne_30_0(const string_q& path, void* data) {
//     CMigrationChecker* checker = (CMigrationChecker*)data;
//     if (endsWith(path, "/")) {
//         return forEveryFileInFolder(path + "*", migrateOne_30_0, data);
//     } else {
//         checker->nSeen++;
//         if (!endsWith(path, ".acct.bin")) {
//             if (endsWith(path, ".mon.bin"))
//                 checker->nSkipped++;
// we only need to migrate if there's an old-fashioned monitor file...
//             return !shouldQuit();
//         }
//         checker->nMigrated++;
//         string_q monFile = substitute(path, ".acct.bin", ".mon.bin");
//         string_q delFile = substitute(path, ".acct.bin", ".acct.bin.deleted");
//         bool wasDeleted = fileExists(delFile);
//         Handle the situation differently if the new style monitor does not exists
//         if (!fileExists(monFile)) {
//             CStringArray parts;
//             explode(parts, substitute(path, ".acct.bin", ""), '/');

//             {  // keep the frame...
//                 ostringstream cmd;
//                 cmd << "chifra list --silent " << parts[parts.size() - 1] << " 2>&1 >/dev/null";
//                 if (system(cmd.str().c_str())) {
//                 }  // Don't remove cruft. Silences compiler warnings
//             }
//             if (wasDeleted) {
//                 ostringstream cmd;
//                 cmd << "chifra monitors --delete " << parts[parts.size() - 1] << " 2>/dev/null";
//                 if (system(cmd.str().c_str())) {
//                 }  // Don't remove cruft. Silences compiler warnings
//             }
//         }
//         if (fileExists(path)) {
//             ::remove(path.c_str());
//         }
//         if (fileExists(delFile)) {
//             ::remove(delFile.c_str());
//         }
//         string_q lastFile = substitute(path, ".acct.bin", ".last.txt");
//         if (fileExists(lastFile)) {
//             ::remove(lastFile.c_str());
//         }
//     }
//     return !shouldQuit();
// }
//--------------------------------------------------------------------------------
// bool COptions::handle_migrate_30_0(void) {
//     auto cache = cacheFolder_monitors;
//     LOG_INFO(cGreen, "Checking '", relativize(cache), "'", string_q(50, ' '), cOff);
//     CMigrationChecker checker(cache);
//     forEveryFileInFolder(cache, migrateOne_30_0, &checker);  // will quit early if it finds a migrate
//     LOG_INFO("  ", checker.Report() + string_q(30, ' '));
//     return false;
// }

package listPkg

// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

import (
	"fmt"
	"io/ioutil"
	"log"
	"os"
	"strconv"
	"sync"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/monitor"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/ethereum/go-ethereum/common"
)

// AddressMonitorMap carries arrays of appearances that have not yet been written to the monitor file
type AddressMonitorMap map[common.Address]*monitor.Monitor

// MonitorUpdate stores the original 'chifra list' command line options plus
type MonitorUpdate struct {
	MaxTasks   int
	MonitorMap AddressMonitorMap
	Options    *ListOptions
	FirstBlock uint64
}

func (opts *ListOptions) HandleFreshenMonitors(monitorArray *[]monitor.Monitor) error {
	var updater = MonitorUpdate{
		MaxTasks:   12,
		MonitorMap: make(AddressMonitorMap, len(opts.Addrs)),
		Options:    opts,
		FirstBlock: utils.NOPOS,
	}

	// This removes duplicates from the input array and keep a map from address to
	// a pointer to the monitors
	for _, addr := range opts.Addrs {
		if updater.MonitorMap[common.HexToAddress(addr)] == nil {
			mon, _ := monitor.NewStagedMonitor(opts.Globals.Chain, addr)
			mon.ReadHeader()
			if uint64(mon.LastScanned) < updater.FirstBlock {
				updater.FirstBlock = uint64(mon.LastScanned)
			}
			*monitorArray = append(*monitorArray, mon)
			// we need the address here because we want to modify this object below
			updater.MonitorMap[mon.Address] = &(*monitorArray)[len(*monitorArray)-1]
		}
	}

	chain := opts.Globals.Chain
	bloomPath := config.GetPathToIndex(chain) + "blooms/"
	files, err := ioutil.ReadDir(bloomPath)
	if err != nil {
		return err
	}

	var wg sync.WaitGroup
	resultChannel := make(chan []index.AppearanceResult, len(files))

	taskCount := 0
	for _, info := range files {
		if !info.IsDir() {
			fileName := bloomPath + "/" + info.Name()
			fileRange, err := cache.RangeFromFilename(fileName)
			if err != nil {
				// don't respond further -- there may be foreign files in the folder
				fmt.Println(err)
				continue
			}

			max := os.Getenv("FAKE_FINAL_BLOCK") // This is for testing only, please ignore
			if len(max) > 0 {
				m, _ := strconv.ParseUint(max, 10, 32)
				if fileRange.Last > m {
					continue
				}
			}

			if opts.Globals.TestMode && fileRange.Last > 5000000 {
				continue
			}

			if fileRange.BlockIsAfter(updater.FirstBlock) {
				continue
			}

			if taskCount >= updater.MaxTasks {
				resArray := <-resultChannel
				for _, r := range resArray {
					updater.updateMonitors(&r)
				}
				taskCount--
			}

			// Run a go routine for each index file
			taskCount++
			wg.Add(1)
			go updater.visitChunkToFreshenFinal(fileName, resultChannel, &wg)
		}
	}

	wg.Wait()
	close(resultChannel)

	for resArray := range resultChannel {
		for _, r := range resArray {
			updater.updateMonitors(&r)
		}
	}

	return updater.moveAllToProduction()
}

// visitChunkToFreshenFinal opens one index file, searches for all the address(es) we're looking for and pushes the resultRecords
// (even if empty) down the resultsChannel.
func (updater *MonitorUpdate) visitChunkToFreshenFinal(fileName string, resultChannel chan<- []index.AppearanceResult, wg *sync.WaitGroup) {
	var results []index.AppearanceResult
	defer func() {
		wg.Done()
		resultChannel <- results
	}()

	// TODO: BOGUS - Should only scan if we're not already seen the block
	bloomFilename := index.ToBloomPath(fileName)

	// We open the bloom filter and read its header but we do not read any of the
	// actual bits in the blooms. The IsMember function reads individual bytes to
	// check individual bits.
	bloom, err := index.NewChunkBloom(bloomFilename)
	if err != nil {
		results = append(results, index.AppearanceResult{Range: bloom.Range, Err: err})
		bloom.Close()
		return
	}

	// We check each address against the bloom to see if there are any hits...
	var bloomHits = false
	for _, mon := range updater.MonitorMap {
		if bloom.IsMember(mon.Address) {
			bloomHits = true
			break
		}
	}

	// We're done with the bloom and we want to close it as soon as we can (therefore
	// we don't defer this close, but close it right away -- otherwise too many files
	// are open and we get an error).
	// TODO: Must we always be closing these files?
	bloom.Close()

	// If none of the addresses hit, we're finished with this index chunk. We want the
	// caller to note this range even though there was no hit. In this way, we keep
	// track of the last index portion we've seen. Because none of the addresses hit,
	// we don't need to send a specific message.
	if !bloomHits {
		results = append(results, index.AppearanceResult{Range: bloom.Range})
		return
	}

	indexFilename := index.ToIndexPath(fileName)
	if !file.FileExists(indexFilename) {
		_, err := index.EstablishIndexChunk(updater.Options.Globals.Chain, bloom.Range)
		if err != nil {
			results = append(results, index.AppearanceResult{Range: bloom.Range, Err: err})
			return
		}
	}

	indexChunk, err := index.NewChunkData(indexFilename)
	if err != nil {
		results = append(results, index.AppearanceResult{Range: bloom.Range, Err: err})
		return
	}
	defer indexChunk.Close()

	for _, mon := range updater.MonitorMap {
		results = append(results, *indexChunk.GetAppearanceRecords(mon.Address))
	}
}

// updateMonitors writes an array of appearances to the Monitor file updating the header for lastScanned. It
// is called by 'chifra list' and 'chifra export' prior to reporting results
func (updater *MonitorUpdate) updateMonitors(result *index.AppearanceResult) {
	if result == nil {
		fmt.Println("Should not happen -- null result")
		return
	}

	if result.Err != nil {
		fmt.Println("Error processing index file:", result.Err)
		return
	}

	mon := updater.MonitorMap[result.Address]
	if mon == nil {
		// In this case, there were no errors, but all monitors were skipped. This would
		// happen, for example, due to false positives hits on the bloom filters. We want
		// to update the LastScanned values for each of these monitors. WriteAppendApps, when
		// given a nil appearance list simply updates the header. Note we update for the
		// start of the next chunk (plus 1 to current range).
		for _, mon := range updater.MonitorMap {
			// TODO: BOGUS - can we manage these files pointers so they aren't
			// copied and therefore there aren't too many of them and it crashes
			mon.Close()
			err := mon.WriteAppendApps(uint32(result.Range.Last)+1, nil)
			if err != nil {
				log.Println(err)
			}
		}

	} else {
		mon.Close()
		// Note that result.AppRecords may be nil here (because, for example, this
		// monitor had a false positive), but we do still want to update the header.
		mon.WriteMonHeader(mon.Deleted, uint32(result.Range.Last)+1)
		if result.AppRecords != nil {
			nWritten := len(*result.AppRecords)
			if nWritten > 0 {
				_, err := mon.WriteAppearances(*result.AppRecords, os.O_WRONLY|os.O_APPEND)
				if err != nil {
					log.Println(err)
				} else if !updater.Options.Globals.TestMode {
					log.Printf("%s appended %d apps at %s\n", mon.GetAddrStr(), nWritten, result.Range)
				}
			}
		}
	}
}
