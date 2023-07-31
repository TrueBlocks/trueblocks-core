package scrapePkg

import (
	"encoding/json"
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
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/tslib"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

// ScrapedData combines the block data, trace data, and log data into a single structure
type ScrapedData struct {
	blockNumber base.Blknum
	traces      []types.SimpleTrace
	logs        []types.SimpleLog
}

type BlazeOptions struct {
	Chain        string                  `json:"chain"`
	NChannels    uint64                  `json:"nChannels"`
	NProcessed   uint64                  `json:"nProcessed"`
	StartBlock   uint64                  `json:"startBlock"`
	BlockCount   uint64                  `json:"blockCnt"`
	RipeBlock    uint64                  `json:"ripeBlock"`
	UnripeDist   uint64                  `json:"unripe"`
	RpcProvider  string                  `json:"rpcProvider"`
	TsArray      []tslib.TimestampRecord `json:"-"`
	ProcessedMap map[base.Blknum]bool    `json:"-"`
	BlockWg      sync.WaitGroup          `json:"-"`
	AppearanceWg sync.WaitGroup          `json:"-"`
	TsWg         sync.WaitGroup          `json:"-"`
	AppsPerChunk uint64                  `json:"-"`
}

func (opts *BlazeOptions) String() string {
	copy := BlazeOptions{
		Chain:      opts.Chain,
		NChannels:  opts.NChannels,
		NProcessed: opts.NProcessed,
		StartBlock: opts.StartBlock,
		BlockCount: opts.BlockCount,
		RipeBlock:  opts.RipeBlock,
		UnripeDist: opts.UnripeDist,
	}
	b, _ := json.MarshalIndent(&copy, "", "  ")
	return string(b)
}

// HandleBlaze does the actual scraping, walking through block_cnt blocks and querying traces and logs
// and then extracting addresses and timestamps from those data structures.
func (opts *BlazeOptions) HandleBlaze(meta *rpcClient.MetaData) (ok bool, err error) {
	blocks := []int{}
	for block := int(opts.StartBlock); block < int(opts.StartBlock+opts.BlockCount); block++ {
		blocks = append(blocks, block)
	}
	return opts.HandleBlaze1(meta, blocks)
}

// TODO: We could, if we wished, use getLogs with a block range to retrieve all of the logs in the range
// TODO: with a single query. See closed issue #1829

func (opts *BlazeOptions) HandleBlaze1(meta *rpcClient.MetaData, blocks []int) (ok bool, err error) {
	//
	// We build a pipeline that takes block numbers in through the blockChannel which queries the chain
	// and sends the results through the appearanceChannel and the timestampChannel. The appearanceChannel
	// processes appearances and writes them to the ripe folder. The timestampChannel processes timestamps
	// and writes them to the timestamp database.
	//
	blockChannel := make(chan int)
	appearanceChannel := make(chan ScrapedData)
	tsChannel := make(chan tslib.TimestampRecord)

	opts.BlockWg.Add(int(opts.NChannels))
	for i := 0; i < int(opts.NChannels); i++ {
		go opts.BlazeProcessBlocks(meta, blockChannel, appearanceChannel, tsChannel)
	}

	// TODO: These go routines may fail. Question -- how does one respond to an error inside a go routine?
	opts.AppearanceWg.Add(int(opts.NChannels))
	for i := 0; i < int(opts.NChannels); i++ {
		go opts.BlazeProcessAppearances(meta, appearanceChannel)
	}

	opts.TsWg.Add(int(opts.NChannels))
	for i := 0; i < int(opts.NChannels); i++ {
		go opts.BlazeProcessTimestamps(tsChannel)
	}

	for _, block := range blocks {
		blockChannel <- block
	}

	close(blockChannel)
	opts.BlockWg.Wait()

	close(appearanceChannel)
	opts.AppearanceWg.Wait()

	close(tsChannel)
	opts.TsWg.Wait()

	return true, nil
}

// BlazeProcessBlocks Processes the block channel and for each block query the node for both traces and logs. Send results down appearanceChannel.
func (opts *BlazeOptions) BlazeProcessBlocks(meta *rpcClient.MetaData, blockChannel chan int, appearanceChannel chan ScrapedData, tsChannel chan tslib.TimestampRecord) (err error) {
	defer opts.BlockWg.Done()
	for bn := range blockChannel {

		sd := ScrapedData{
			blockNumber: base.Blknum(bn),
		}

		// TODO: BOGUS - This could use rawTraces so as to avoid unnecessary decoding
		if sd.traces, err = rpcClient.GetTracesByNumber(opts.Chain, uint64(bn)); err != nil {
			// TODO: BOGUS - we should send in an errorChannel and send the error down that channel and continue here
			return err
		}

		// TODO: BOGUS - This could use rawTraces so as to avoid unnecessary decoding
		if sd.logs, err = rpcClient.GetLogsByNumber(opts.Chain, uint64(bn)); err != nil {
			// TODO: BOGUS - we should send in an errorChannel and send the error down that channel and continue here
			return err
		}

		appearanceChannel <- sd

		ts := tslib.TimestampRecord{
			Bn: uint32(bn),
			Ts: uint32(rpcClient.GetBlockTimestamp(opts.Chain, utils.PointerOf(uint64(bn)))),
		}

		tsChannel <- ts
	}

	return
}

var blazeMutex sync.Mutex

// BlazeProcessAppearances processes ScrapedData objects shoved down the appearanceChannel
func (opts *BlazeOptions) BlazeProcessAppearances(meta *rpcClient.MetaData, appearanceChannel chan ScrapedData) (err error) {
	defer opts.AppearanceWg.Done()

	for sData := range appearanceChannel {
		addrMap := make(index.AddressBooleanMap)

		err = index.UniqFromTraces(opts.Chain, sData.traces, addrMap)
		if err != nil {
			return err
		}

		err = index.UniqFromLogs(opts.Chain, sData.logs, addrMap)
		if err != nil {
			return err
		}

		err = opts.WriteAppearancesBlaze(meta, sData.blockNumber, addrMap)
		if err != nil {
			return err
		}
	}

	return
}

// BlazeProcessTimestamps processes timestamp data (currently by printing to a temporary file)
func (opts *BlazeOptions) BlazeProcessTimestamps(tsChannel chan tslib.TimestampRecord) (err error) {
	defer opts.TsWg.Done()

	for ts := range tsChannel {
		blazeMutex.Lock()
		opts.TsArray = append(opts.TsArray, ts)
		blazeMutex.Unlock()
	}
	return
}

var writeMutex sync.Mutex

func (opts *BlazeOptions) WriteAppearancesBlaze(meta *rpcClient.MetaData, bn base.Blknum, addrMap index.AddressBooleanMap) (err error) {
	if len(addrMap) > 0 {
		appearanceArray := make([]string, 0, len(addrMap))
		for record := range addrMap {
			appearanceArray = append(appearanceArray, record)
		}
		sort.Strings(appearanceArray)

		blockNumStr := utils.PadNum(int(bn), 9)
		fileName := config.GetPathToIndex(opts.Chain) + "ripe/" + blockNumStr + ".txt"
		if bn > base.Blknum(opts.RipeBlock) {
			fileName = config.GetPathToIndex(opts.Chain) + "unripe/" + blockNumStr + ".txt"
		}

		toWrite := []byte(strings.Join(appearanceArray[:], "\n") + "\n")
		err = os.WriteFile(fileName, toWrite, 0744) // Uses os.O_WRONLY|os.O_CREATE|os.O_TRUNC
		if err != nil {
			fmt.Println("call1 to WriteFile returned error", err)
			return err
		}
	}

	opts.syncedReporting(bn, false /* force */)
	writeMutex.Lock()
	opts.ProcessedMap[bn] = true
	writeMutex.Unlock()
	opts.NProcessed++

	return
}

var (
	locker uint32
)

func (opts *BlazeOptions) syncedReporting(bn base.Blknum, force bool) {
	if !atomic.CompareAndSwapUint32(&locker, 0, 1) {
		// Simply skip the update if someone else is already reporting
		return
	}
	// Make sure to clear the lock on exit
	defer atomic.StoreUint32(&locker, 0)

	// TODO: See issue https://github.com/TrueBlocks/trueblocks-core/issues/2238
	step := uint64(17)
	if opts.NProcessed%step == 0 || force {
		dist := uint64(0)
		if opts.RipeBlock > uint64(bn) {
			dist = (opts.RipeBlock - uint64(bn))
		}
		msg := fmt.Sprintf("Scraping %-04d of %-04d at block %d of %d (%d blocks from head)", opts.NProcessed, opts.BlockCount, bn, opts.RipeBlock, dist)
		logger.Progress(true, msg)
	}
}
