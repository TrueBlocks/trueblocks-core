package scrapePkg

import (
	"fmt"
	"os"
	"sort"
	"strings"
	"sync"
	"sync/atomic"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/tslib"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

// HandleBlaze does the actual scraping, walking through block_cnt blocks and querying traces and logs
// and then extracting addresses and timestamps from those data structures.
func (blazeMan *BlazeManager) HandleBlaze(meta *rpc.MetaData) (ok bool, err error) {
	blocks := []int{}
	for block := int(blazeMan.StartBlock); block < int(blazeMan.StartBlock+blazeMan.BlockCount); block++ {
		blocks = append(blocks, block)
	}
	return blazeMan.HandleBlaze1(meta, blocks)
}

// TODO: We could, if we wished, use getLogs with a block range to retrieve all of the logs in the range
// TODO: with a single query. See closed issue #1829

func (blazeMan *BlazeManager) HandleBlaze1(meta *rpc.MetaData, blocks []int) (ok bool, err error) {
	//
	// We build a pipeline that takes block numbers in through the blockChannel which queries the chain
	// and sends the results through the appearanceChannel and the timestampChannel. The appearanceChannel
	// processes appearances and writes them to the ripe folder. The timestampChannel processes timestamps
	// and writes them to the timestamp database.
	//
	blockChannel := make(chan int)
	appearanceChannel := make(chan scrapedData)
	tsChannel := make(chan tslib.TimestampRecord)

	blazeMan.BlockWg.Add(int(blazeMan.NChannels))
	for i := 0; i < int(blazeMan.NChannels); i++ {
		go func() {
			_ = blazeMan.BlazeProcessBlocks(meta, blockChannel, appearanceChannel, tsChannel)
		}()
	}

	// TODO: These go routines may fail. Question -- how does one respond to an error inside a go routine?
	blazeMan.AppearanceWg.Add(int(blazeMan.NChannels))
	for i := 0; i < int(blazeMan.NChannels); i++ {
		go func() {
			_ = blazeMan.BlazeProcessAppearances(meta, appearanceChannel)
		}()
	}

	blazeMan.TimestampsWg.Add(int(blazeMan.NChannels))
	for i := 0; i < int(blazeMan.NChannels); i++ {
		go func() {
			_ = blazeMan.BlazeProcessTimestamps(tsChannel)
		}()
	}

	for _, block := range blocks {
		blockChannel <- block
	}

	close(blockChannel)
	blazeMan.BlockWg.Wait()

	close(appearanceChannel)
	blazeMan.AppearanceWg.Wait()

	close(tsChannel)
	blazeMan.TimestampsWg.Wait()

	return true, nil
}

// BlazeProcessBlocks Processes the block channel and for each block query the node for both traces and logs. Send results down appearanceChannel.
func (blazeMan *BlazeManager) BlazeProcessBlocks(meta *rpc.MetaData, blockChannel chan int, appearanceChannel chan scrapedData, tsChannel chan tslib.TimestampRecord) (err error) {
	defer blazeMan.BlockWg.Done()
	for bn := range blockChannel {

		sd := scrapedData{
			blockNumber: base.Blknum(bn),
		}

		chain := blazeMan.Chain
		conn := rpc.TempConnection(chain)

		ts := tslib.TimestampRecord{
			Bn: uint32(bn),
			Ts: uint32(conn.GetBlockTimestamp(uint64(bn))),
		}

		// TODO: BOGUS - This could use rawTraces so as to avoid unnecessary decoding
		if sd.traces, err = conn.GetTracesByBlockNumber(uint64(bn)); err != nil {
			// TODO: BOGUS - we should send in an errorChannel and send the error down that channel and continue here
			return err
		}

		// TODO: BOGUS - This could use rawTraces so as to avoid unnecessary decoding
		if sd.logs, err = conn.GetLogsByNumber(uint64(bn), base.Timestamp(ts.Ts)); err != nil {
			// TODO: BOGUS - we should send in an errorChannel and send the error down that channel and continue here
			return err
		}

		appearanceChannel <- sd
		tsChannel <- ts
	}

	return
}

var blazeMutex sync.Mutex

// BlazeProcessAppearances processes scrapedData objects shoved down the appearanceChannel
func (blazeMan *BlazeManager) BlazeProcessAppearances(meta *rpc.MetaData, appearanceChannel chan scrapedData) (err error) {
	defer blazeMan.AppearanceWg.Done()

	for sData := range appearanceChannel {
		addrMap := make(index.AddressBooleanMap)

		err = index.UniqFromTraces(blazeMan.Chain, sData.traces, addrMap)
		if err != nil {
			return err
		}

		err = index.UniqFromLogs(blazeMan.Chain, sData.logs, addrMap)
		if err != nil {
			return err
		}

		err = blazeMan.WriteAppearancesBlaze(meta, sData.blockNumber, addrMap)
		if err != nil {
			return err
		}
	}

	return
}

// BlazeProcessTimestamps processes timestamp data (currently by printing to a temporary file)
func (blazeMan *BlazeManager) BlazeProcessTimestamps(tsChannel chan tslib.TimestampRecord) (err error) {
	defer blazeMan.TimestampsWg.Done()

	for ts := range tsChannel {
		blazeMutex.Lock()
		blazeMan.Timestamps = append(blazeMan.Timestamps, ts)
		blazeMutex.Unlock()
	}

	return
}

var writeMutex sync.Mutex

func (blazeMan *BlazeManager) WriteAppearancesBlaze(meta *rpc.MetaData, bn base.Blknum, addrMap index.AddressBooleanMap) (err error) {
	if len(addrMap) > 0 {
		appearanceArray := make([]string, 0, len(addrMap))
		for record := range addrMap {
			appearanceArray = append(appearanceArray, record)
		}
		sort.Strings(appearanceArray)

		blockNumStr := utils.PadNum(int(bn), 9)
		fileName := config.GetPathToIndex(blazeMan.Chain) + "ripe/" + blockNumStr + ".txt"
		if bn > base.Blknum(blazeMan.RipeBlock) {
			fileName = config.GetPathToIndex(blazeMan.Chain) + "unripe/" + blockNumStr + ".txt"
		}

		toWrite := []byte(strings.Join(appearanceArray[:], "\n") + "\n")
		err = os.WriteFile(fileName, toWrite, 0744) // Uses os.O_WRONLY|os.O_CREATE|os.O_TRUNC
		if err != nil {
			fmt.Println("call1 to WriteFile returned error", err)
			return err
		}
	}

	blazeMan.syncedReporting(bn, false /* force */)
	writeMutex.Lock()
	blazeMan.ProcessedMap[bn] = true
	writeMutex.Unlock()
	blazeMan.NProcessed++

	return
}

var (
	locker uint32
)

func (blazeMan *BlazeManager) syncedReporting(bn base.Blknum, force bool) {
	if !atomic.CompareAndSwapUint32(&locker, 0, 1) {
		// Simply skip the update if someone else is already reporting
		return
	}
	// Make sure to clear the lock on exit
	defer atomic.StoreUint32(&locker, 0)

	// TODO: See issue https://github.com/TrueBlocks/trueblocks-core/issues/2238
	step := uint64(17)
	if blazeMan.NProcessed%step == 0 || force {
		dist := uint64(0)
		if blazeMan.RipeBlock > uint64(bn) {
			dist = (blazeMan.RipeBlock - uint64(bn))
		}
		msg := fmt.Sprintf("Scraping %-04d of %-04d at block %d of %d (%d blocks from head)", blazeMan.NProcessed, blazeMan.BlockCount, bn, blazeMan.RipeBlock, dist)
		logger.Progress(true, msg)
	}
}
