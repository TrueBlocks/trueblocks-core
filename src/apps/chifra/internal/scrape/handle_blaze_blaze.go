package scrapePkg

import (
	"encoding/json"
	"fmt"
	"io/ioutil"
	"sort"
	"strconv"
	"strings"
	"sync"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
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
	copy := *opts
	copy.AppearanceMap = index.AddressAppearanceMap{}
	copy.TsArray = []tslib.Timestamp{}
	copy.ProcessedMap = make(map[int]bool)
	b, _ := json.MarshalIndent(copy, "", "  ")
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
	// Prepare three channels to process first blocks, then appearances and timestamps
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

var beenHere = false

// BlazeProcessBlocks Processes the block channel and for each block query the node for both traces and logs. Send results down appearanceChannel.
func (opts *BlazeOptions) BlazeProcessBlocks(meta *rpcClient.MetaData, blockChannel chan int, appearanceChannel chan ScrapedData, tsChannel chan tslib.Timestamp) (err error) {
	defer opts.BlockWg.Done()

	for blockNum := range blockChannel {

		//if ForceFail && !beenHere && blockNum == 802 {
		//	beenHere = true
		//	fmt.Println("Forcing failure for block", blockNum)
		//	return errors.New("Forcing failure")
		//}
		// RPCPayload is used during to make calls to the RPC.
		var traces rpcClient.Traces
		tracePayload := rpcClient.RPCPayload{
			Jsonrpc:   "2.0",
			Method:    "trace_block",
			RPCParams: rpcClient.RPCParams{fmt.Sprintf("0x%x", blockNum)},
			ID:        1002,
		}
		err = rpcClient.FromRpc(opts.RpcProvider, &tracePayload, &traces)
		if err != nil {
			// fmt.Println("rpcCall failed at block", blockNum, err)
			return err
		}

		var logs rpcClient.Logs
		logsPayload := rpcClient.RPCPayload{
			Jsonrpc:   "2.0",
			Method:    "eth_getLogs",
			RPCParams: rpcClient.RPCParams{rpcClient.LogFilter{Fromblock: fmt.Sprintf("0x%x", blockNum), Toblock: fmt.Sprintf("0x%x", blockNum)}},
			ID:        1003,
		}
		err = rpcClient.FromRpc(opts.RpcProvider, &logsPayload, &logs)
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

		err = opts.WriteAppearances(meta, sData.blockNumber, addressMap)
		if err != nil {
			// fmt.Println("WriteAppearances returned error", sData.blockNumber, err)
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
						var receipt rpcClient.Receipt
						var txReceiptPl = rpcClient.RPCPayload{
							Jsonrpc:   "2.0",
							Method:    "eth_getTransactionReceipt",
							RPCParams: rpcClient.RPCParams{traces.Result[i].TransactionHash},
							ID:        1005,
						}
						err = rpcClient.FromRpc(opts.RpcProvider, &txReceiptPl, &receipt)
						if err != nil {
							// fmt.Println("rpcCall failed at block", traces.Result[i].TransactionHash, err)
							return err
						}
						addr := receipt.Result.ContractAddress
						if isAddress(addr) {
							opts.AddToMaps(addr, bn, txid, addressMap)
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

func (opts *BlazeOptions) WriteAppearances(meta *rpcClient.MetaData, bn int, addressMap map[string]bool) (err error) {
	if len(addressMap) > 0 {
		appearanceArray := make([]string, 0, len(addressMap))
		for record := range addressMap {
			appearanceArray = append(appearanceArray, record)
		}
		sort.Strings(appearanceArray)

		blockNumStr := utils.PadLeft(strconv.Itoa(bn), 9)
		fileName := config.GetPathToIndex(opts.Chain) + "ripe/" + blockNumStr + ".txt"
		if bn > int(opts.RipeBlock) {
			fileName = config.GetPathToIndex(opts.Chain) + "unripe/" + blockNumStr + ".txt"
		}

		toWrite := []byte(strings.Join(appearanceArray[:], "\n") + "\n")
		err = ioutil.WriteFile(fileName, toWrite, 0744)
		if err != nil {
			fmt.Println("call1 to WriteFile returned error", err)
			return err
		}
	}

	// TODO: THIS IS A PERFORMANCE ISSUE PRINTING EVERY BLOCK
	if !Debugging {
		step := uint64(17)
		if opts.NProcessed%step == 0 {
			dist := uint64(0)
			if opts.RipeBlock > uint64(bn) {
				dist = (opts.RipeBlock - uint64(bn))
			}
			msg := fmt.Sprintf("Scraping %-04d of %-04d at block %d of %d (%d blocks from head)", opts.NProcessed, opts.BlockCount, bn, opts.RipeBlock, dist)
			logger.Log(logger.Progress, msg)
		}
	}

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

var Debugging = file.FileExists("./testing")
