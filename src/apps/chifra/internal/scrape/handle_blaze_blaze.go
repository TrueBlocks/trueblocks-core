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
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/tslib"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

// scrapedData combines the block data, trace data, and log data into a single structure
type scrapedData struct {
	blockNumber base.Blknum
	traces      []types.SimpleTrace
	logs        []types.SimpleLog
}

type BlazeOptions struct {
	Chain        string                  `json:"chain"`
	NChannels    uint64                  `json:"nChannels"`
	StartBlock   uint64                  `json:"startBlock"`
	BlockCount   uint64                  `json:"blockCnt"`
	UnripeDist   uint64                  `json:"unripe"`
	RpcProvider  string                  `json:"rpcProvider"`
	AppsPerChunk uint64                  `json:"-"`
	RipeBlock    uint64                  `json:"ripeBlock"`
	NProcessed   uint64                  `json:"nProcessed"`
	Timestamps   []tslib.TimestampRecord `json:"-"`
	ProcessedMap map[base.Blknum]bool    `json:"-"`
	BlockWg      sync.WaitGroup          `json:"-"`
	AppearanceWg sync.WaitGroup          `json:"-"`
	TimestampsWg sync.WaitGroup          `json:"-"`
}

func (blazeOpts *BlazeOptions) String() string {
	copy := BlazeOptions{
		Chain:      blazeOpts.Chain,
		NChannels:  blazeOpts.NChannels,
		StartBlock: blazeOpts.StartBlock,
		BlockCount: blazeOpts.BlockCount,
		UnripeDist: blazeOpts.UnripeDist,
		RipeBlock:  blazeOpts.RipeBlock,
		NProcessed: blazeOpts.NProcessed,
	}
	b, _ := json.MarshalIndent(&copy, "", "  ")
	return string(b)
}

// HandleBlaze does the actual scraping, walking through block_cnt blocks and querying traces and logs
// and then extracting addresses and timestamps from those data structures.
func (blazeOpts *BlazeOptions) HandleBlaze(meta *rpc.MetaData) (ok bool, err error) {
	blocks := []int{}
	for block := int(blazeOpts.StartBlock); block < int(blazeOpts.StartBlock+blazeOpts.BlockCount); block++ {
		blocks = append(blocks, block)
	}
	return blazeOpts.HandleBlaze1(meta, blocks)
}

// TODO: We could, if we wished, use getLogs with a block range to retrieve all of the logs in the range
// TODO: with a single query. See closed issue #1829

func (blazeOpts *BlazeOptions) HandleBlaze1(meta *rpc.MetaData, blocks []int) (ok bool, err error) {
	//
	// We build a pipeline that takes block numbers in through the blockChannel which queries the chain
	// and sends the results through the appearanceChannel and the timestampChannel. The appearanceChannel
	// processes appearances and writes them to the ripe folder. The timestampChannel processes timestamps
	// and writes them to the timestamp database.
	//
	blockChannel := make(chan int)
	appearanceChannel := make(chan scrapedData)
	tsChannel := make(chan tslib.TimestampRecord)

	blazeOpts.BlockWg.Add(int(blazeOpts.NChannels))
	for i := 0; i < int(blazeOpts.NChannels); i++ {
		go func() {
			_ = blazeOpts.BlazeProcessBlocks(meta, blockChannel, appearanceChannel, tsChannel)
		}()
	}

	// TODO: These go routines may fail. Question -- how does one respond to an error inside a go routine?
	blazeOpts.AppearanceWg.Add(int(blazeOpts.NChannels))
	for i := 0; i < int(blazeOpts.NChannels); i++ {
		go func() {
			_ = blazeOpts.BlazeProcessAppearances(meta, appearanceChannel)
		}()
	}

	blazeOpts.TimestampsWg.Add(int(blazeOpts.NChannels))
	for i := 0; i < int(blazeOpts.NChannels); i++ {
		go func() {
			_ = blazeOpts.BlazeProcessTimestamps(tsChannel)
		}()
	}

	for _, block := range blocks {
		blockChannel <- block
	}

	close(blockChannel)
	blazeOpts.BlockWg.Wait()

	close(appearanceChannel)
	blazeOpts.AppearanceWg.Wait()

	close(tsChannel)
	blazeOpts.TimestampsWg.Wait()

	return true, nil
}

// BlazeProcessBlocks Processes the block channel and for each block query the node for both traces and logs. Send results down appearanceChannel.
func (blazeOpts *BlazeOptions) BlazeProcessBlocks(meta *rpc.MetaData, blockChannel chan int, appearanceChannel chan scrapedData, tsChannel chan tslib.TimestampRecord) (err error) {
	defer blazeOpts.BlockWg.Done()
	for bn := range blockChannel {

		sd := scrapedData{
			blockNumber: base.Blknum(bn),
		}

		chain := blazeOpts.Chain
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
func (blazeOpts *BlazeOptions) BlazeProcessAppearances(meta *rpc.MetaData, appearanceChannel chan scrapedData) (err error) {
	defer blazeOpts.AppearanceWg.Done()

	for sData := range appearanceChannel {
		addrMap := make(index.AddressBooleanMap)

		err = index.UniqFromTraces(blazeOpts.Chain, sData.traces, addrMap)
		if err != nil {
			return err
		}

		err = index.UniqFromLogs(blazeOpts.Chain, sData.logs, addrMap)
		if err != nil {
			return err
		}

		err = blazeOpts.WriteAppearancesBlaze(meta, sData.blockNumber, addrMap)
		if err != nil {
			return err
		}
	}

	return
}

// BlazeProcessTimestamps processes timestamp data (currently by printing to a temporary file)
func (blazeOpts *BlazeOptions) BlazeProcessTimestamps(tsChannel chan tslib.TimestampRecord) (err error) {
	defer blazeOpts.TimestampsWg.Done()

	for ts := range tsChannel {
		blazeMutex.Lock()
		blazeOpts.Timestamps = append(blazeOpts.Timestamps, ts)
		blazeMutex.Unlock()
	}

	return
}

var writeMutex sync.Mutex

func (blazeOpts *BlazeOptions) WriteAppearancesBlaze(meta *rpc.MetaData, bn base.Blknum, addrMap index.AddressBooleanMap) (err error) {
	if len(addrMap) > 0 {
		appearanceArray := make([]string, 0, len(addrMap))
		for record := range addrMap {
			appearanceArray = append(appearanceArray, record)
		}
		sort.Strings(appearanceArray)

		blockNumStr := utils.PadNum(int(bn), 9)
		fileName := config.GetPathToIndex(blazeOpts.Chain) + "ripe/" + blockNumStr + ".txt"
		if bn > base.Blknum(blazeOpts.RipeBlock) {
			fileName = config.GetPathToIndex(blazeOpts.Chain) + "unripe/" + blockNumStr + ".txt"
		}

		toWrite := []byte(strings.Join(appearanceArray[:], "\n") + "\n")
		err = os.WriteFile(fileName, toWrite, 0744) // Uses os.O_WRONLY|os.O_CREATE|os.O_TRUNC
		if err != nil {
			fmt.Println("call1 to WriteFile returned error", err)
			return err
		}
	}

	blazeOpts.syncedReporting(bn, false /* force */)
	writeMutex.Lock()
	blazeOpts.ProcessedMap[bn] = true
	writeMutex.Unlock()
	blazeOpts.NProcessed++

	return
}

var (
	locker uint32
)

func (blazeOpts *BlazeOptions) syncedReporting(bn base.Blknum, force bool) {
	if !atomic.CompareAndSwapUint32(&locker, 0, 1) {
		// Simply skip the update if someone else is already reporting
		return
	}
	// Make sure to clear the lock on exit
	defer atomic.StoreUint32(&locker, 0)

	// TODO: See issue https://github.com/TrueBlocks/trueblocks-core/issues/2238
	step := uint64(17)
	if blazeOpts.NProcessed%step == 0 || force {
		dist := uint64(0)
		if blazeOpts.RipeBlock > uint64(bn) {
			dist = (blazeOpts.RipeBlock - uint64(bn))
		}
		msg := fmt.Sprintf("Scraping %-04d of %-04d at block %d of %d (%d blocks from head)", blazeOpts.NProcessed, blazeOpts.BlockCount, bn, blazeOpts.RipeBlock, dist)
		logger.Progress(true, msg)
	}
}
