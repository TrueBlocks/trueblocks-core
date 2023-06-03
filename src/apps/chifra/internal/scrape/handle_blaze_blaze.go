package scrapePkg

import (
	"encoding/json"
	"fmt"
	"os"
	"sort"
	"strings"
	"sync"
	"sync/atomic"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/tslib"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

// ScrapedData combines the block data, trace data, and log data into a single structure
type ScrapedData struct {
	blockNumber int
	traces      rpcClient.Traces
	logs        rpcClient.Logs
}

type BlazeOptions struct {
	Chain         string                     `json:"chain"`
	NChannels     uint64                     `json:"nChannels"`
	NProcessed    uint64                     `json:"nProcessed"`
	StartBlock    uint64                     `json:"startBlock"`
	BlockCount    uint64                     `json:"blockCnt"`
	RipeBlock     uint64                     `json:"ripeBlock"`
	UnripeDist    uint64                     `json:"unripe"`
	RpcProvider   string                     `json:"rpcProvider"`
	AppearanceMap index.AddressAppearanceMap `json:"-"`
	TsArray       []tslib.TimestampRecord    `json:"-"`
	ProcessedMap  map[int]bool               `json:"-"`
	BlockWg       sync.WaitGroup             `json:"-"`
	AppearanceWg  sync.WaitGroup             `json:"-"`
	TsWg          sync.WaitGroup             `json:"-"`
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

// var beenHere = false

// BlazeProcessBlocks Processes the block channel and for each block query the node for both traces and logs. Send results down appearanceChannel.
func (opts *BlazeOptions) BlazeProcessBlocks(meta *rpcClient.MetaData, blockChannel chan int, appearanceChannel chan ScrapedData, tsChannel chan tslib.TimestampRecord) (err error) {
	defer opts.BlockWg.Done()

	for blockNum := range blockChannel {

		//if ForceFail && !beenHere && blockNum == 802 {
		//	beenHere = true
		//	fmt.Println("Forcing failure for block", blockNum)
		//	return errors.New("Forcing failure")
		//}
		// rpc.Payload is used during to make calls to the RPC.
		// TODO: Use rpc.Query
		var traces rpcClient.Traces
		tracePayload := rpc.Payload{
			Method: "trace_block",
			Params: rpc.Params{fmt.Sprintf("0x%x", blockNum)},
		}
		err = rpc.FromRpc(opts.RpcProvider, &tracePayload, &traces)
		if err != nil {
			// fmt.Println("rpcCall failed at block", blockNum, err)
			return err
		}

		var logs rpcClient.Logs
		// TODO: Use rpc.Query
		logsPayload := rpc.Payload{
			Method: "eth_getLogs",
			Params: rpc.Params{rpcClient.LogFilter{Fromblock: fmt.Sprintf("0x%x", blockNum), Toblock: fmt.Sprintf("0x%x", blockNum)}},
		}
		err = rpc.FromRpc(opts.RpcProvider, &logsPayload, &logs)
		if err != nil {
			// fmt.Println("rpcCall failed at block", blockNum, err)
			return err
		}

		appearanceChannel <- ScrapedData{
			blockNumber: blockNum,
			traces:      traces,
			logs:        logs,
		}

		ts := tslib.TimestampRecord{
			Bn: uint32(blockNum),
			Ts: uint32(rpc.GetBlockTimestamp(opts.Chain, uint64(blockNum))),
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
		addressMap := make(index.AddressBooleanMap)
		err = index.ExtractUniqFromTraces(opts.Chain, sData.blockNumber, &sData.traces, opts.AppearanceMap, addressMap)
		if err != nil {
			return err
		}

		err = index.ExtractUniqFromLogs(opts.Chain, sData.blockNumber, &sData.logs, opts.AppearanceMap, addressMap)
		if err != nil {
			return err
		}

		err = opts.WriteAppearancesBlaze(meta, sData.blockNumber, addressMap)
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

func (opts *BlazeOptions) WriteAppearancesBlaze(meta *rpcClient.MetaData, bn int, addressMap index.AddressBooleanMap) (err error) {
	if len(addressMap) > 0 {
		appearanceArray := make([]string, 0, len(addressMap))
		for record := range addressMap {
			appearanceArray = append(appearanceArray, record)
		}
		sort.Strings(appearanceArray)

		blockNumStr := utils.PadNum(bn, 9)
		fileName := config.GetPathToIndex(opts.Chain) + "ripe/" + blockNumStr + ".txt"
		if bn > int(opts.RipeBlock) {
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

func (opts *BlazeOptions) syncedReporting(bn int, force bool) {
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
