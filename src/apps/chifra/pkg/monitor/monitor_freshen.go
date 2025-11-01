package monitor

// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

import (
	"context"
	"fmt"
	"os"
	"path/filepath"
	"sort"
	"strings"
	"sync"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/manifest"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/ranges"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/sigintTrap"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/validate"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/walk"
)

// MonitorUpdate stores the original 'chifra list' command line options plus
type MonitorUpdate struct {
	MaxTasks      int
	MonitorMap    map[base.Address]*Monitor
	Chain         string
	PublisherAddr base.Address
	TestMode      bool
	SkipFreshen   bool
	FirstBlock    base.Blknum
	Addrs         []string
}

func NewUpdater(chain string, testMode, skipFreshen bool, addrs []string) MonitorUpdate {
	return MonitorUpdate{
		MaxTasks:      12,
		FirstBlock:    base.NOPOSN,
		Chain:         chain,
		PublisherAddr: base.Address{},
		TestMode:      testMode,
		SkipFreshen:   skipFreshen,
		Addrs:         addrs,
		MonitorMap:    make(map[base.Address]*Monitor, len(addrs)),
	}
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
		logger.Panic("unlockForAddress called for address that has never been locked: " + address)
	}
	mutex.Unlock()
}

func (updater *MonitorUpdate) FreshenMonitors(monitorArray *[]Monitor) (bool, error) {

	// TODO: There are special case addresses for the sender of mining rewards and
	// TODO: prefund allocations that get ignored here because they are baddresses.
	// TODO: We could, if we wished, create special cases here to (for example) report
	// TODO: the balance sheet for "Ethereum" as a whole. How much has been spent on mining?
	// TODO: How much was the original allocation worth?
	for _, address := range updater.Addrs {
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

	// This removes dups and keeps a map from address to a pointer to the monitors
	for _, addr := range updater.Addrs {
		err := needsMigration(addr)
		if err != nil {
			return canceled, err
		}

		if updater.MonitorMap[base.HexToAddress(addr)] == nil {
			mon, _ := NewMonitorStaged(updater.Chain, addr)
			_ = mon.ReadMonitorHeader()
			bn := base.Blknum(mon.LastScanned)
			if bn < updater.FirstBlock {
				updater.FirstBlock = bn
			}
			*monitorArray = append(*monitorArray, mon)
			// we need the address here because we want to modify this object below
			updater.MonitorMap[mon.Address] = &(*monitorArray)[len(*monitorArray)-1]
		}
	}

	if updater.SkipFreshen {
		return canceled, nil
	}

	bloomPath := filepath.Join(config.PathToIndex(updater.Chain), "blooms")
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
			fileName := filepath.Join(bloomPath, info.Name())
			if !walk.IsCacheType(fileName, walk.Index_Bloom, true /* checkExt */) {
				continue // sometimes there are .gz files in this folder, for example
			}
			fileRange, err := ranges.RangeFromFilenameE(fileName)
			if err != nil {
				// don't respond further -- there may be foreign files in the folder
				fmt.Println(err)
				continue
			}

			max := os.Getenv("FAKE_FINAL_BLOCK") // This is for testing only, please ignore
			if len(max) > 0 {
				if fileRange.Last > base.MustParseBlknum(max) {
					continue
				}
			}

			if updater.TestMode && fileRange.Last > maxTestingBlock {
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

	if !updater.TestMode {
		// TODO: Note we could actually test this if we had the concept of a FAKE_HEAD block
		stagePath := index.ToStagingPath(filepath.Join(config.PathToIndex(updater.Chain), "staging"))
		stageFn, _ := file.LatestFileInFolder(stagePath)
		rng := ranges.RangeFromFilename(stageFn)
		lines := []string{}
		for addr, mon := range updater.MonitorMap {
			bn := base.Blknum(mon.LastScanned)
			if !rng.LaterThanB(bn) { // the range precedes the block number
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
	results := make([]index.AppearanceResult, 0, len(updater.MonitorMap))
	defer func() {
		resultChannel <- results
		wg.Done()
	}()

	bloomFilename := index.ToBloomPath(fileName)

	// We open the bloom filter and read its header but we do not read any of the
	// actual bits in the blooms. The IsMember function reads individual bytes to
	// check individual bits.
	bl, err := index.OpenBloom(bloomFilename, true /* check */)
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
		chain := updater.Chain
		var man *manifest.Manifest
		man, err = manifest.LoadManifest(chain, updater.PublisherAddr, manifest.LocalCache)
		if err != nil {
			results = append(results, index.AppearanceResult{Range: bl.Range, Err: err})
			return
		}

		err = index.DownloadOneChunk(chain, man, bl.Range)
		if err != nil {
			results = append(results, index.AppearanceResult{Range: bl.Range, Err: err})
			return
		}
	}

	indexChunk, err := index.OpenIndex(indexFilename, true /* check */)
	if err != nil {
		results = append(results, index.AppearanceResult{Range: bl.Range, Err: err})
		return
	}
	defer indexChunk.Close()

	for _, mon := range updater.MonitorMap {
		results = append(results, *indexChunk.ReadAppearances(mon.Address))
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
				} else {
					msg := fmt.Sprintf("%s%s appended %5d apps at %s%s", colors.Green, mon.Address.Hex(), nWritten, result.Range, colors.Off)
					logger.Progress(!updater.TestMode, msg)
				}
			}
		}
	}
}

func needsMigration(addr string) error {
	mon := Monitor{Address: base.HexToAddress(addr)}
	path := strings.ReplaceAll(mon.Path(), ".mon.bin", ".acct.bin")
	if file.FileExists(path) {
		path = filepath.Clean(strings.ReplaceAll(path, config.PathToCache(mon.Chain), "./"))
		return validate.Usage("Old style monitor found at {0}. Please run '{1}'", path, "chifra config --migrate cache")
	}
	return nil
}

// TODO: Somewhat iffy code here
const (
	startOfBlockNum = 42 + 1
	endOfBlockNum   = 42 + 1 + 9
	startOfTxId     = 42 + 1 + 9 + 1
	endOfTxId       = 42 + 1 + 9 + 1 + 5
)

func getAppearances(addrStr string, lines []string, lastVisited uint32, found int) *[]types.AppRecord {
	results := make([]types.AppRecord, 0, 1000)
	for idx := found; idx < len(lines); idx++ {
		if !strings.HasPrefix(lines[idx], addrStr) {
			break
		}
		strippedBn := strings.TrimLeft(lines[idx][startOfBlockNum:endOfBlockNum], "0")
		strippedTxId := strings.TrimLeft(lines[idx][startOfTxId:endOfTxId], "0")
		r := types.AppRecord{
			BlockNumber:      uint32(base.MustParseUint64(strippedBn)),
			TransactionIndex: uint32(base.MustParseUint64(strippedTxId)),
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

// moveAllToProduction completes the update by moving the monitor files from ./cache/<chain>/monitors/staging to
// ./cache/<chain>/monitors.
func (updater *MonitorUpdate) moveAllToProduction() error {
	for _, mon := range updater.MonitorMap {
		err := mon.MoveToProduction()
		if err != nil {
			return err
		}
	}
	return nil
}
