package listPkg

// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

import (
	"context"
	"fmt"
	"os"
	"sort"
	"strconv"
	"strings"
	"sync"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index/bloom"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/monitor"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/sigintTrap"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/walk"
)

// AddressMonitorMap carries arrays of appearances that have not yet been written to the monitor file
type AddressMonitorMap map[base.Address]*monitor.Monitor

// MonitorUpdate stores the original 'chifra list' command line options plus
type MonitorUpdate struct {
	MaxTasks   int
	MonitorMap AddressMonitorMap
	Options    *ListOptions
	FirstBlock uint64
}

const maxTestingBlock = 17000000

// mutexesPerAddress map stores mutex for each address that is being/has been freshened
var mutexesPerAddress = make(map[string]*sync.Mutex)
var mutexesPerAddressMutex sync.Mutex

// lockForAddress locks the mutex for the given address
func lockForAddress(address string) {
	mutex, ok := mutexesPerAddress[address]
	if ok {
		mutex.Lock()
		return
	}

	mutexesPerAddressMutex.Lock()
	defer mutexesPerAddressMutex.Unlock()
	newMutex := &sync.Mutex{}
	mutexesPerAddress[address] = newMutex
	newMutex.Lock()
}

// unlockForAddress removes mutex lock for the given address
func unlockForAddress(address string) {
	mutex, ok := mutexesPerAddress[address]
	if !ok {
		panic("unlockForAddress called for address that has never been locked: " + address)
	}

	mutex.Unlock()
}

func (opts *ListOptions) HandleFreshenMonitors(monitorArray *[]monitor.Monitor) (bool, error) {
	chain := opts.Globals.Chain

	// TODO: There are special case addresses for the sender of mining rewards and
	// TODO: prefund allocations that get ignored here because they are baddresses.
	// TODO: We could, if we wished, create special cases here to (for example) report
	// TODO: the balance sheet for "Ethereum" as a whole. How much has been spent on mining?
	// TODO: How much was the original allocation worth?
	for _, address := range opts.Addrs {
		lockForAddress(address)
		defer unlockForAddress(address) // reminder: this defers until the function returns, not this loop
	}

	var m sync.Once
	canceled := false
	ctx, cancel := context.WithCancel(context.Background())
	cleanOnQuit := func() {
		canceled = true
		logger.Warn(colors.Yellow+"User hit control+c...", colors.Off)
		cancel()
	}
	trapChannel := sigintTrap.Enable(ctx, cancel, cleanOnQuit)
	defer sigintTrap.Disable(trapChannel)

	var updater = MonitorUpdate{
		MaxTasks:   12,
		MonitorMap: make(AddressMonitorMap, len(opts.Addrs)),
		Options:    opts,
		FirstBlock: utils.NOPOS,
	}

	// This removes dups and keeps a map from address to a pointer to the monitors
	for _, addr := range opts.Addrs {
		err := needsMigration(addr)
		if err != nil {
			return canceled, err
		}

		if updater.MonitorMap[base.HexToAddress(addr)] == nil {
			mon, _ := monitor.NewStagedMonitor(chain, addr)
			_ = mon.ReadMonitorHeader()
			if uint64(mon.LastScanned) < updater.FirstBlock {
				updater.FirstBlock = uint64(mon.LastScanned)
			}
			*monitorArray = append(*monitorArray, mon)
			// we need the address here because we want to modify this object below
			updater.MonitorMap[mon.Address] = &(*monitorArray)[len(*monitorArray)-1]
		}
	}

	bloomPath := config.GetPathToIndex(chain) + "blooms/"
	files, err := os.ReadDir(bloomPath)
	if err != nil {
		return canceled, err
	}

	var wg sync.WaitGroup
	resultChannel := make(chan []index.AppearanceResult, len(files))

	taskCount := 0
	for _, info := range files {
		if canceled {
			m.Do(func() { logger.Warn(colors.Yellow+"Finishing", taskCount, "current tasks...", colors.Off) })
			continue
		}
		if !info.IsDir() {
			fileName := bloomPath + "/" + info.Name()
			if !walk.IsCacheType(fileName, walk.Index_Bloom, true /* checkExt */) {
				continue // sometimes there are .gz files in this folder, for example
			}
			fileRange, err := base.RangeFromFilenameE(fileName)
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

			if opts.Globals.TestMode && fileRange.Last > maxTestingBlock {
				continue
			}

			if fileRange.EarlierThanB(updater.FirstBlock) {
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

	if !opts.Globals.TestMode {
		// TODO: Note we could actually test this if we had the concept of a FAKE_HEAD block
		stagePath := index.ToStagingPath(config.GetPathToIndex(chain) + "staging")
		stageFn, _ := file.LatestFileInFolder(stagePath)
		rng := base.RangeFromFilename(stageFn)
		lines := []string{}
		for addr, mon := range updater.MonitorMap {
			if !rng.LaterThanB(uint64(mon.LastScanned)) { // the range preceeds the block number
				if len(lines) == 0 {
					lines = file.AsciiFileToLines(stageFn)
					sort.Slice(lines, func(i, j int) bool {
						return lines[i] < lines[j]
					})
				}

				addrStr := addr.Hex()
				found := sort.Search(len(lines), func(i int) bool {
					return lines[i] >= addrStr
				})
				if found < len(lines) && strings.HasPrefix(lines[found], addrStr) {
					apps := getAppearances(addrStr, lines, mon.LastScanned, found)
					if apps != nil {
						stageResult := index.AppearanceResult{
							Address:    addr,
							Range:      rng,
							AppRecords: apps,
						}
						updater.updateMonitors(&stageResult)
					}
				}
			}
		}
	}

	return canceled, updater.moveAllToProduction()
}

// visitChunkToFreshenFinal opens an index file, searches for the address(es) we're looking for and pushes
// the appearance records down the resultsChannel (even if there are none).
func (updater *MonitorUpdate) visitChunkToFreshenFinal(fileName string, resultChannel chan<- []index.AppearanceResult, wg *sync.WaitGroup) {
	var results []index.AppearanceResult
	defer func() {
		resultChannel <- results
		wg.Done()
	}()

	bloomFilename := index.ToBloomPath(fileName)

	// We open the bloom filter and read its header but we do not read any of the
	// actual bits in the blooms. The IsMember function reads individual bytes to
	// check individual bits.
	bl, err := bloom.NewChunkBloom(bloomFilename)
	if err != nil {
		results = append(results, index.AppearanceResult{Range: bl.Range, Err: err})
		bl.Close()
		return
	}

	// We check each address against the bloom to see if there are any hits...
	var bloomHits = false
	for _, mon := range updater.MonitorMap {
		if bl.IsMember(mon.Address) {
			bloomHits = true
			break
		}
	}

	// We're done with the bloom and we want to close it as soon as we can (therefore
	// we don't defer this close, but close it right away -- otherwise too many files
	// are open and we get an error).
	// TODO: Must we always be closing these files?
	bl.Close()

	// If none of the addresses hit, we're finished with this index chunk. We want the
	// caller to note this range even though there was no hit. In this way, we keep
	// track of the last index portion we've seen. Because none of the addresses hit,
	// we don't need to send a specific message.
	if !bloomHits {
		results = append(results, index.AppearanceResult{Range: bl.Range})
		return
	}

	indexFilename := index.ToIndexPath(fileName)
	if !file.FileExists(indexFilename) {
		_, err := index.EstablishIndexChunk(updater.Options.Globals.Chain, bl.Range)
		if err != nil {
			results = append(results, index.AppearanceResult{Range: bl.Range, Err: err})
			return
		}
	}

	indexChunk, err := index.NewChunkData(indexFilename)
	if err != nil {
		results = append(results, index.AppearanceResult{Range: bl.Range, Err: err})
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

	lastScanned := uint32(result.Range.Last) + 1

	mon := updater.MonitorMap[result.Address]
	if mon == nil {
		// In this case, there were no errors, but all monitors were skipped. This would
		// happen, for example, due to false positives hits on the bloom filters. We want
		// to update the LastScanned values for each of these monitors. WriteAppearancesAppend, when
		// given a nil appearance list simply updates the header. Note we update for the
		// start of the next chunk (plus 1 to current range).
		for _, mon := range updater.MonitorMap {
			// TODO: Can we manage these file pointers so they aren't copied and
			// TODO: therefore don't need to be Closed so or else it crashes
			mon.Close()
			err := mon.WriteAppearancesAppend(lastScanned, nil)
			if err != nil {
				logger.Error(err)
			}
		}

	} else {
		mon.Close()
		// Note that result.AppRecords may be nil here (because, for example, this
		// monitor had a false positive), but we do still want to update the header.
		_ = mon.WriteMonHeader(mon.Deleted, lastScanned, false /* force */)
		if result.AppRecords != nil {
			nWritten := len(*result.AppRecords)
			if nWritten > 0 {
				_, err := mon.WriteAppearances(*result.AppRecords, true /* append */)
				if err != nil {
					logger.Error(err)
				} else if !updater.Options.Globals.TestMode {
					msg := fmt.Sprintf("%s appended %d apps at %s", mon.Address.Hex(), nWritten, result.Range)
					logger.Info(msg)
				}
			}
		}
	}
}

func needsMigration(addr string) error {
	mon := monitor.Monitor{Address: base.HexToAddress(addr)}
	path := strings.Replace(mon.Path(), ".mon.bin", ".acct.bin", -1)
	if file.FileExists(path) {
		path = strings.Replace(path, config.GetPathToCache(mon.Chain), "./", -1)
		return validate.Usage("Old style monitor found at {0}. Please run '{1}'", path, "chifra config --migrate cache")
	}
	return nil
}

// TODO: Somewhat iffy code here
const (
	asciiAddressSize    = 42
	asciiBlockNumSize   = 9
	asciiTxIdSize       = 5
	asciiAppearanceSize = 59
	startOfBlockNum     = 42 + 1
	endOfBlockNum       = 42 + 1 + 9
	startOfTxId         = 42 + 1 + 9 + 1
	endOfTxId           = 42 + 1 + 9 + 1 + 5
)

func getAppearances(addrStr string, lines []string, lastVisited uint32, found int) *[]index.AppearanceRecord {
	results := make([]index.AppearanceRecord, 0, 1000)
	for idx := found; idx < len(lines); idx++ {
		if !strings.HasPrefix(lines[idx], addrStr) {
			break
		}
		r := index.AppearanceRecord{
			BlockNumber:   uint32(globals.ToUint64(lines[idx][startOfBlockNum:endOfBlockNum])),
			TransactionId: uint32(globals.ToUint64(lines[idx][startOfTxId:endOfTxId])),
		}
		if r.BlockNumber > lastVisited {
			results = append(results, r)
		}
	}
	if len(results) == 0 {
		return nil
	}
	return &results
}
