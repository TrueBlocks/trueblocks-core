package scrapePkg

import (
	"context"
	"fmt"
	"os"
	"sort"
	"strings"
	"sync"
	"sync/atomic"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/notify"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/tslib"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/uniq"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

// HandleBlaze does the actual scraping, walking through block_cnt blocks and querying traces and logs
// and then extracting addresses and timestamps from those data structures.
func (bm *BlazeManager) HandleBlaze(ctx context.Context, blocks []base.Blknum) (err error) {

	// clear this out
	bm.errors = make([]scrapeError, 0)

	// We need three pipelines...we shove into blocks, blocks shoves into appearances and timestamps
	blockChannel := make(chan base.Blknum)
	appearanceChannel := make(chan scrapedData)
	tsChannel := make(chan tslib.TimestampRecord)

	// TODO: The go routines below may fail. Question -- how does one respond to an error inside a go routine?

	blockWg := sync.WaitGroup{}
	blockWg.Add(bm.nChannels)
	for i := 0; i < bm.nChannels; i++ {
		go func() {
			_ = bm.ProcessBlocks(blockChannel, &blockWg, appearanceChannel)
		}()
	}

	appWg := sync.WaitGroup{}
	appWg.Add(bm.nChannels)
	for i := 0; i < bm.nChannels; i++ {
		go func() {
			_ = bm.ProcessAppearances(appearanceChannel, &appWg, tsChannel)
		}()
	}

	tsWg := sync.WaitGroup{}
	tsWg.Add(bm.nChannels)
	for i := 0; i < bm.nChannels; i++ {
		go func() {
			_ = bm.ProcessTimestamps(tsChannel, &tsWg)
		}()
	}

	// Now we have three go routines waiting for data. Send it...
	for _, block := range blocks {
		if ctx.Err() != nil {
			// This means the context got cancelled, i.e. we got a SIGINT.
			return nil
		}

		blockChannel <- block
	}

	// ...and wait until we're done...
	close(blockChannel)
	blockWg.Wait()

	close(appearanceChannel)
	appWg.Wait()

	close(tsChannel)
	tsWg.Wait()

	return nil
}

// ProcessBlocks processes the block channel and for each block query the node for both
// traces and logs. Send results down appearanceChannel.
func (bm *BlazeManager) ProcessBlocks(blockChannel chan base.Blknum, blockWg *sync.WaitGroup, appearanceChannel chan scrapedData) (err error) {
	defer blockWg.Done()
	for bn := range blockChannel {
		sd := scrapedData{
			bn: bn,
			ts: tslib.TimestampRecord{
				Bn: uint32(bn),
				Ts: uint32(bm.opts.Conn.GetBlockTimestamp(bn)),
			},
		}

		// TODO: BOGUS - we should send in an errorChannel and send the error down that channel and continue here
		var err error
		if sd.traces, err = bm.opts.Conn.GetTracesByBlockNumber(bn); err != nil {
			bm.errors = append(bm.errors, scrapeError{block: bn, err: err})
		} else if sd.receipts, _, err = bm.opts.Conn.GetReceiptsByNumber(bn, base.Timestamp(sd.ts.Ts)); err != nil {
			bm.errors = append(bm.errors, scrapeError{block: bn, err: err})
		} else if sd.withdrawals, sd.miner, err = bm.opts.Conn.GetMinerAndWithdrawals(bn); err != nil {
			bm.errors = append(bm.errors, scrapeError{block: bn, err: err})
		} else {
			appearanceChannel <- sd
		}
	}
	return
}

var blazeMutex sync.Mutex

// ProcessAppearances processes scrapedData objects shoved down the appearanceChannel
func (bm *BlazeManager) ProcessAppearances(appearanceChannel chan scrapedData, appWg *sync.WaitGroup, tsChannel chan tslib.TimestampRecord) (err error) {
	defer appWg.Done()

	for sData := range appearanceChannel {
		addrMap := make(uniq.AddressBooleanMap)
		if err = uniq.UniqFromTraces(bm.chain, sData.traces, addrMap); err != nil {
			bm.errors = append(bm.errors, scrapeError{block: sData.bn, err: err})

		} else if err = uniq.UniqFromReceipts(bm.chain, sData.receipts, addrMap); err != nil {
			bm.errors = append(bm.errors, scrapeError{block: sData.bn, err: err})

		} else if err = uniq.UniqFromWithdrawals(bm.chain, sData.withdrawals, sData.bn, addrMap); err != nil {
			bm.errors = append(bm.errors, scrapeError{block: sData.bn, err: err})

		} else {
			_ = uniq.AddMiner(bm.chain, sData.miner, sData.bn, addrMap)
			if err = bm.WriteAppearances(sData.bn, addrMap); err != nil {
				bm.errors = append(bm.errors, scrapeError{block: sData.bn, err: err})
			}
		}
		tsChannel <- sData.ts
	}

	return
}

// ProcessTimestamps processes timestamp data (currently by printing to a temporary file)
func (bm *BlazeManager) ProcessTimestamps(tsChannel chan tslib.TimestampRecord, tsWg *sync.WaitGroup) (err error) {
	defer tsWg.Done()
	for ts := range tsChannel {
		blazeMutex.Lock()
		bm.timestamps[base.Blknum(ts.Bn)] = ts
		bm.nTimestamps++
		blazeMutex.Unlock()
	}
	return
}

var writeMutex sync.Mutex

// TODO: The original intent of creating files was so that we could start over where we left off
// if we failed. But this isn't how it works. We cleanup any temp files if we fail, which means
// we write these files and if we fail, we remove them. If we don't fail, we've written them out,
// but only to re-read them and delete them in this round. Would could have easily just kept them
// in an in-memory cache. This would also allow us to not have to stringify the data and just store
// pointers to structs in memory. We wouldn't have to keep a seperate timestamps database nor a
// processedMap (the pointer would serve that purpose).

// WriteAppearances writes the appearance for a chunk to a file
func (bm *BlazeManager) WriteAppearances(bn base.Blknum, addrMap uniq.AddressBooleanMap) (err error) {
	ripePath := config.PathToIndex(bm.chain) + "ripe" + string(os.PathSeparator)
	unripePath := config.PathToIndex(bm.chain) + "unripe" + string(os.PathSeparator)
	appendScrapeError := func(err error) {
		bm.errors = append(bm.errors, scrapeError{block: bn, err: err})
	}
	notificationPayload := make([]notify.NotificationPayloadAppearance, 0, len(addrMap))

	if len(addrMap) > 0 {
		appearanceArray := make([]string, 0, len(addrMap))
		for record := range addrMap {
			appearanceArray = append(appearanceArray, record)
			if bn <= bm.ripeBlock {
				// Only notify about ripe block's appearances
				payloadItem := notify.NotificationPayloadAppearance{}
				err := payloadItem.FromString(record)
				if err != nil {
					return fmt.Errorf("implementation error - unexpected record format: %s", err)
				}
				notificationPayload = append(notificationPayload, payloadItem)
			}
		}
		sort.Strings(appearanceArray)

		blockNumStr := utils.PadNum(int(bn), 9)
		fileName := ripePath + blockNumStr + ".txt"
		if bn > bm.ripeBlock {
			fileName = unripePath + blockNumStr + ".txt"
		}

		toWrite := []byte(strings.Join(appearanceArray[:], "\n") + "\n")
		err = os.WriteFile(fileName, toWrite, 0744) // Uses os.O_WRONLY|os.O_CREATE|os.O_TRUNC
		if err != nil {
			appendScrapeError(err)
			return err
		}
	}

	if bm.opts.Notify && bn <= bm.ripeBlock {
		err = Notify(notify.Notification[[]notify.NotificationPayloadAppearance]{
			Msg:     notify.MessageAppearance,
			Meta:    bm.meta,
			Payload: notificationPayload,
		})
		if err != nil {
			// We need this warning, otherwise errors don't show up for 2,000 blocks
			logger.Error("error sending notification", err)
			return err
		}
	}

	bm.syncedReporting(bn, false /* force */)
	writeMutex.Lock()
	bm.processedMap[bn] = true
	if bn > bm.ripeBlock {
		bm.nUnripe++
	} else {
		bm.nRipe++
	}
	writeMutex.Unlock()

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
	if bm.nProcessed()%17 == 0 || force {
		dist := base.Blknum(0)
		if bm.ripeBlock > bn {
			dist = (bm.ripeBlock - bn)
		}
		msg := fmt.Sprintf("Scraping %-04d of %-04d at block %d of %d (%d blocks from head)",
			bm.nProcessed(),
			bm.BlockCount(),
			bn,
			bm.ripeBlock,
			dist)
		logger.Progress(true, msg)
	}
}

// scrapedData combines the extracted block data, trace data, and log data into a
// structure that is passed through to the AddressChannel for further processing.
type scrapedData struct {
	bn          base.Blknum
	ts          tslib.TimestampRecord
	traces      []types.Trace
	receipts    []types.Receipt
	withdrawals []types.Withdrawal
	miner       base.Address
}
