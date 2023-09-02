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
	blocks := []base.Blknum{}

	start := blazeMan.StartBlock()
	end := blazeMan.StartBlock() + blazeMan.BlockCount()

	for block := start; block < end; block++ {
		blocks = append(blocks, block)
	}

	return blazeMan.HandleBlaze1(meta, blocks)
}

func (blazeMan *BlazeManager) HandleBlaze1(meta *rpc.MetaData, blocks []base.Blknum) (ok bool, err error) {
	nChannels := int(blazeMan.opts.Settings.Channel_count)

	// We need three pipelines...we shove into blocks, blocks shoves into appearances and timestamps
	blockChannel := make(chan base.Blknum)
	appearanceChannel := make(chan scrapedData)
	tsChannel := make(chan tslib.TimestampRecord)

	// TODO: The go routines below may fail. Question -- how does one respond to an error inside a go routine?

	blockWg := sync.WaitGroup{}
	blockWg.Add(nChannels)
	for i := 0; i < nChannels; i++ {
		go func() {
			_ = blazeMan.BlazeProcessBlocks(meta, blockChannel, &blockWg, appearanceChannel, tsChannel)
		}()
	}

	appWg := sync.WaitGroup{}
	appWg.Add(nChannels)
	for i := 0; i < nChannels; i++ {
		go func() {
			_ = blazeMan.BlazeProcessAppearances(meta, appearanceChannel, &appWg)
		}()
	}

	tsWg := sync.WaitGroup{}
	tsWg.Add(nChannels)
	for i := 0; i < nChannels; i++ {
		go func() {
			_ = blazeMan.BlazeProcessTimestamps(tsChannel, &tsWg)
		}()
	}

	// Now we have three go routines waiting for data. Send it...
	for _, block := range blocks {
		blockChannel <- block
	}

	// ...and wait until we're done...
	close(blockChannel)
	blockWg.Wait()

	close(appearanceChannel)
	appWg.Wait()

	close(tsChannel)
	tsWg.Wait()

	return true, nil
}

// BlazeProcessBlocks processes the block channel and for each block query the node for both
// traces and logs. Send results down appearanceChannel.
func (blazeMan *BlazeManager) BlazeProcessBlocks(meta *rpc.MetaData, blockChannel chan base.Blknum, blockWg *sync.WaitGroup, appearanceChannel chan scrapedData, tsChannel chan tslib.TimestampRecord) (err error) {
	defer blockWg.Done()
	for bn := range blockChannel {

		sd := scrapedData{
			bn: bn,
		}

		chain := blazeMan.chain
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

		// TODO: BOGUS - This could use rawLogs so as to avoid unnecessary decoding
		if sd.receipts, err = conn.GetReceiptsByNumber(uint64(bn), base.Timestamp(ts.Ts)); err != nil {
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
func (blazeMan *BlazeManager) BlazeProcessAppearances(meta *rpc.MetaData, appearanceChannel chan scrapedData, appWg *sync.WaitGroup) (err error) {
	defer appWg.Done()

	for sData := range appearanceChannel {
		addrMap := make(index.AddressBooleanMap)

		err = index.UniqFromTraces(blazeMan.chain, sData.traces, addrMap)
		if err != nil {
			return err
		}

		err = index.UniqFromReceipts(blazeMan.chain, sData.receipts, addrMap)
		if err != nil {
			return err
		}

		err = blazeMan.WriteAppearancesBlaze(meta, sData.bn, addrMap)
		if err != nil {
			return err
		}
	}

	return
}

// BlazeProcessTimestamps processes timestamp data (currently by printing to a temporary file)
func (blazeMan *BlazeManager) BlazeProcessTimestamps(tsChannel chan tslib.TimestampRecord, tsWg *sync.WaitGroup) (err error) {
	defer tsWg.Done()

	for ts := range tsChannel {
		blazeMutex.Lock()
		blazeMan.timestamps = append(blazeMan.timestamps, ts)
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
		fileName := config.GetPathToIndex(blazeMan.chain) + "ripe/" + blockNumStr + ".txt"
		if bn > blazeMan.ripeBlock {
			fileName = config.GetPathToIndex(blazeMan.chain) + "unripe/" + blockNumStr + ".txt"
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
	blazeMan.processedMap[bn] = true
	writeMutex.Unlock()
	blazeMan.nProcessed++

	return
}

var (
	locker uint32
)

func (bm *BlazeManager) syncedReporting(bn base.Blknum, force bool) {
	if !atomic.CompareAndSwapUint32(&locker, 0, 1) {
		// Simply skip the update if someone else is already reporting
		return
	}
	// Make sure to clear the lock on exit
	defer atomic.StoreUint32(&locker, 0)

	// Only report once in a while (17 blocks)
	if bm.nProcessed%17 == 0 || force {
		dist := uint64(0)
		if bm.ripeBlock > bn {
			dist = (bm.ripeBlock - bn)
		}
		msg := fmt.Sprintf("Scraping %-04d of %-04d at block %d of %d (%d blocks from head)",
			bm.nProcessed,
			bm.BlockCount(),
			bn,
			bm.ripeBlock,
			dist)
		logger.Progress(true, msg)
	}
}
