package scrapePkg

import (
	"encoding/json"
	"fmt"
	"os"
	"sort"
	"strconv"
	"strings"
	"sync"
	"sync/atomic"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/tslib"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
	"github.com/ethereum/go-ethereum/common"
	"github.com/ethereum/go-ethereum/common/hexutil"
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
	TsArray       []tslib.Timestamp          `json:"-"`
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
	tsChannel := make(chan tslib.Timestamp)

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
func (opts *BlazeOptions) BlazeProcessBlocks(meta *rpcClient.MetaData, blockChannel chan int, appearanceChannel chan ScrapedData, tsChannel chan tslib.Timestamp) (err error) {
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

		ts := tslib.Timestamp{
			Bn: uint32(blockNum),
			Ts: uint32(rpcClient.GetBlockTimestamp(opts.RpcProvider, uint64(blockNum))),
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
		addressMap := make(map[string]bool)
		err = opts.BlazeExtractFromTraces(sData.blockNumber, &sData.traces, addressMap)
		if err != nil {
			// fmt.Println("BlazeExtractFromTraces returned error", sData.blockNumber, err)
			return err
		}

		err = opts.BlazeExtractFromLogs(sData.blockNumber, &sData.logs, addressMap)
		if err != nil {
			// fmt.Println("BlazeExtractFromLogs returned error", sData.blockNumber, err)
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
func (opts *BlazeOptions) BlazeProcessTimestamps(tsChannel chan tslib.Timestamp) (err error) {
	defer opts.TsWg.Done()

	for ts := range tsChannel {
		blazeMutex.Lock()
		opts.TsArray = append(opts.TsArray, ts)
		blazeMutex.Unlock()
	}
	return
}

var mapSync sync.Mutex

func (opts *BlazeOptions) AddToMaps(address string, bn, txid int, addressMap map[string]bool) {
	// Make sure we have a 20 byte '0x' prefixed string (implicit strings come in as 32-byte, non-0x-prefixed strings)
	if !strings.HasPrefix(address, "0x") {
		address = hexutil.Encode(common.HexToAddress(address).Bytes())
	}
	mapSync.Lock()
	key := fmt.Sprintf("%s\t%09d\t%05d", address, bn, txid)
	if !addressMap[key] {
		opts.AppearanceMap[address] = append(opts.AppearanceMap[address], index.AppearanceRecord{
			BlockNumber:   uint32(bn),
			TransactionId: uint32(txid),
		})
	}
	addressMap[key] = true
	mapSync.Unlock()
}

func (opts *BlazeOptions) BlazeExtractFromTraces(bn int, traces *rpcClient.Traces, addressMap map[string]bool) (err error) {
	if traces.Result == nil || len(traces.Result) == 0 {
		return
	}

	for i := 0; i < len(traces.Result); i++ {
		txid := traces.Result[i].TransactionPosition

		if traces.Result[i].Type == "call" {
			// If it's a call, get the to and from
			from := traces.Result[i].Action.From
			if isAddress(from) {
				opts.AddToMaps(from, bn, txid, addressMap)
			}
			to := traces.Result[i].Action.To
			if isAddress(to) {
				opts.AddToMaps(to, bn, txid, addressMap)
			}

		} else if traces.Result[i].Type == "reward" {
			if traces.Result[i].Action.RewardType == "block" {
				author := traces.Result[i].Action.Author
				if validate.IsZeroAddress(author) {
					// Early clients allowed misconfigured miner settings with address
					// 0x0 (reward got burned). We enter a false record with a false tx_id
					// to account for this.
					author = "0xdeaddeaddeaddeaddeaddeaddeaddeaddeaddead"
					opts.AddToMaps(author, bn, 99997, addressMap)

				} else {
					if isAddress(author) {
						opts.AddToMaps(author, bn, 99999, addressMap)
					}
				}

			} else if traces.Result[i].Action.RewardType == "uncle" {
				author := traces.Result[i].Action.Author
				if validate.IsZeroAddress(author) {
					// Early clients allowed misconfigured miner settings with address
					// 0x0 (reward got burned). We enter a false record with a false tx_id
					// to account for this.
					author = "0xdeaddeaddeaddeaddeaddeaddeaddeaddeaddead"
					opts.AddToMaps(author, bn, 99998, addressMap)

				} else {
					if isAddress(author) {
						opts.AddToMaps(author, bn, 99998, addressMap)
					}
				}

			} else if traces.Result[i].Action.RewardType == "external" {
				// This only happens in xDai as far as we know...
				author := traces.Result[i].Action.Author
				if isAddress(author) {
					opts.AddToMaps(author, bn, 99996, addressMap)
				}

			} else {
				fmt.Println("Unknown reward type", traces.Result[i].Action.RewardType)
				return err
			}

		} else if traces.Result[i].Type == "suicide" {
			// add the contract that died, and where it sent it's money
			address := traces.Result[i].Action.Address
			if isAddress(address) {
				opts.AddToMaps(address, bn, txid, addressMap)
			}
			refundAddress := traces.Result[i].Action.RefundAddress
			if isAddress(refundAddress) {
				opts.AddToMaps(refundAddress, bn, txid, addressMap)
			}

		} else if traces.Result[i].Type == "create" {
			// add the creator, and the new address name
			from := traces.Result[i].Action.From
			if isAddress(from) {
				opts.AddToMaps(from, bn, txid, addressMap)
			}
			address := traces.Result[i].Result.Address
			if isAddress(address) {
				opts.AddToMaps(address, bn, txid, addressMap)
			}

			// If it's a top level trace, then the call data is the init,
			// so to match with TrueBlocks, we just parse init
			if len(traces.Result[i].TraceAddress) == 0 {
				if len(traces.Result[i].Action.Init) > 10 {
					initData := traces.Result[i].Action.Init[10:]
					for i := 0; i < len(initData)/64; i++ {
						addr := string(initData[i*64 : (i+1)*64])
						if isImplicitAddress(addr) {
							opts.AddToMaps(addr, bn, txid, addressMap)
						}
					}
				}
			}

			// Handle contract creations that may have errored out
			if traces.Result[i].Action.To == "" {
				if traces.Result[i].Result.Address == "" {
					if traces.Result[i].Error != "" {
						// TODO: Why does this interface always accept nil and zero at the end?
						receipt, err := rpcClient.GetTransactionReceipt(opts.Chain, uint64(bn), uint64(txid), nil, 0)
						if err != nil {
							msg := fmt.Sprintf("rpcCall failed at block %d, tx %d hash %s err %s", bn, txid, traces.Result[i].TransactionHash, err)
							logger.Log(logger.Warning, colors.Red, msg, colors.Off)
							// TODO: This is possibly an error in Erigon - remove it when they fix this issue:
							// TODO: https://github.com/ledgerwatch/erigon/issues/6956. It may require a
							// TODO: full resync. Yes, the problem appears to be this specific. The follow
							// TODO: hack (which tries to correct the problem) may well not work, but
							// TODO: the hope is that these will have already been picked up by the traces.
							// TODO: When fixed, we need to re-scrape from block 16,600,000. This map (which was
							// TODO: retrieved from Nethermind) tries to repair the missing data by marking (for
							// TODO: each transaction) any smart contracts created.
							fixMap := map[string]string{
								"16616983-242": "0x6784d7583cf2528daa270b555a4cb5376648488f",
								"16618181-146": "0x86494c70df6d3416bb4f536a82533b6120c52cde",
								"16618196-18":  "0x40d7b756557d9f7a5655ff70b3253a07f714807a",
								"16620128-12":  "0xb8fb9a557d19d5266f1ba1724445ee2436e3c626",
								"16620182-35":  "0x708bf2bf05492a5644787c134cf8a64e82fa4c52",
								"16621080-107": "0x23c84318fb83ee62e059679cddb3914c923da871",
								"16623590-179": "0xe88d3857676adf23d8324231eabee6ac390f666e",
								"16623602-106": "0x473a0524a25c252bc65a023c8b8476b1eb6ac805",
								"16626181-115": "0x010d9eb886f5b1a0fbef58bca722079e9ac75275",
								"16627272-125": "0xdfd76821bebdbe589f74d311dff4f5859995cda4",
								"16628102-66":  "0xddec22d76cfb1aded71c2f7b64ff768d207d615d",
							}
							key := fmt.Sprintf("%d-%d", bn, txid)
							msg = err.Error()
							if msg != "empty hex string" {
								// not the error we're looking for
								return err
							}

							if len(fixMap[key]) > 0 {
								// both are true - the error is `empty hex string` and we have a fix
								msg = fmt.Sprintf("Corrected %d, tx %d adds %s", bn, txid, fixMap[key])
								logger.Log(logger.Warning, colors.Red, msg, colors.Off)
								opts.AddToMaps(fixMap[key], bn, txid, addressMap)
							}

						} else {
							addr := hexutil.Encode(receipt.ContractAddress.Bytes())
							if isAddress(addr) {
								opts.AddToMaps(addr, bn, txid, addressMap)
							}
						}
					}
				}
			}

		} else {
			fmt.Println("Unknown trace type", traces.Result[i].Type)
			return err
		}

		// Try to get addresses from the input data
		if len(traces.Result[i].Action.Input) > 10 {
			inputData := traces.Result[i].Action.Input[10:]
			//fmt.Println("Input data:", inputData, len(inputData))
			for i := 0; i < len(inputData)/64; i++ {
				addr := string(inputData[i*64 : (i+1)*64])
				if isImplicitAddress(addr) {
					opts.AddToMaps(addr, bn, txid, addressMap)
				}
			}
		}

		// Parse output of trace
		if len(traces.Result[i].Result.Output) > 2 {
			outputData := traces.Result[i].Result.Output[2:]
			for i := 0; i < len(outputData)/64; i++ {
				addr := string(outputData[i*64 : (i+1)*64])
				if isImplicitAddress(addr) {
					opts.AddToMaps(addr, bn, txid, addressMap)
				}
			}
		}
	}

	return
}

// extractFromLogs Extracts addresses from any part of the log data.
func (opts *BlazeOptions) BlazeExtractFromLogs(bn int, logs *rpcClient.Logs, addressMap map[string]bool) (err error) {
	if logs.Result == nil || len(logs.Result) == 0 {
		return
	}

	for i := 0; i < len(logs.Result); i++ {
		txid, _ := strconv.ParseInt(logs.Result[i].TransactionIndex, 0, 32)
		for j := 0; j < len(logs.Result[i].Topics); j++ {
			addr := string(logs.Result[i].Topics[j][2:])
			if isImplicitAddress(addr) {
				opts.AddToMaps(addr, bn, int(txid), addressMap)
			}
		}

		if len(logs.Result[i].Data) > 2 {
			inputData := logs.Result[i].Data[2:]
			for i := 0; i < len(inputData)/64; i++ {
				addr := string(inputData[i*64 : (i+1)*64])
				if isImplicitAddress(addr) {
					opts.AddToMaps(addr, bn, int(txid), addressMap)
				}
			}
		}
	}

	return
}

var writeMutex sync.Mutex

func (opts *BlazeOptions) WriteAppearancesBlaze(meta *rpcClient.MetaData, bn int, addressMap map[string]bool) (err error) {
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

// isAddress Returns true if the address is not a precompile and not the zero address
func isAddress(addr string) bool {
	// As per EIP 1352, all addresses less or equal to the following value are reserved for pre-compiles.
	// We don't index precompiles. https://eips.ethereum.org/EIPS/eip-1352
	return addr > "0x000000000000000000000000000000000000ffff"
}

// isImplicitAddress processes a transaction's 'input' data and 'output' data or an event's data field.
// Anything with 12 bytes of leading zeros but not more than 19 leading zeros (24 and 38 characters
// respectively).
func isImplicitAddress(addr string) bool {
	// Any 32-byte value smaller than this number is assumed to be a 'value'. We call them baddresses.
	// While this may seem like a lot of addresses being labeled as baddresses, it's not very many:
	// ---> 2 out of every 10000000000000000000000000000000000000000000000 are baddresses.
	small := "00000000000000000000000000000000000000ffffffffffffffffffffffffff"
	//        -------+-------+-------+-------+-------+-------+-------+-------+
	if addr <= small {
		return false
	}

	// Any 32-byte value with less than this many leading zeros is not an address (address are 20-bytes and
	// zero padded to the left)
	largePrefix := "000000000000000000000000"
	//              -------+-------+-------+
	if !strings.HasPrefix(addr, largePrefix) {
		return false
	}

	// Of the valid addresses, we assume any ending with this many trailing zeros is also a baddress.
	if strings.HasSuffix(addr, "00000000") {
		return false
	}

	// extract the potential address
	return isAddress("0x" + string(addr[24:]))
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
		logger.Log(logger.Progress, msg)
	}
}
