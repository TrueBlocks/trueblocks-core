package scrapePkg

import (
	"encoding/json"
	"fmt"
	"io/ioutil"
	"log"
	"os"
	"sort"
	"strconv"
	"strings"
	"sync"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/tslib"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

// ScrapedData combines the block data, trace data, and log data into a single structure
type ScrapedData struct {
	blockNumber int
	traces      rpcClient.Traces
	logs        rpcClient.Logs
}

type BlazeOptions struct {
	Chain       string         `json:"chain"`
	NChannels   uint64         `json:"nChannels"`
	NProcessed  uint64         `json:"nProcessed"`
	StartBlock  uint64         `json:"startBlock"`
	BlockCount  uint64         `json:"blockCnt"`
	RipeBlock   uint64         `json:"ripeBlock"`
	UnripeDist  uint64         `json:"unripe"`
	RpcProvider string         `json:"rpcProvider"`
	BlockWG     sync.WaitGroup `json:"-"`
	AddressWG   sync.WaitGroup `json:"-"`
	TsWG        sync.WaitGroup `json:"-"`
}

// String implements the stringer interface
func (opts *BlazeOptions) String() string {
	b, _ := json.MarshalIndent(opts, "", "\t")
	return string(b)
}

// HandleBlaze does the actual scraping, walking through block_cnt blocks and querying traces and logs
// and then extracting addresses and timestamps from those data structures.
func (opts *BlazeOptions) HandleBlaze(meta *rpcClient.MetaData) (ok bool, err error) {

	if utils.OnOff {
		fmt.Println(opts.String())
	}
	blockChannel := make(chan int)
	addressChannel := make(chan ScrapedData)
	tsChannel := make(chan tslib.Timestamp)

	opts.BlockWG.Add(int(opts.NChannels))
	for i := 0; i < int(opts.NChannels); i++ {
		go opts.BlazeProcessBlocks(meta, blockChannel, addressChannel, tsChannel)
	}

	opts.AddressWG.Add(int(opts.NChannels))
	for i := 0; i < int(opts.NChannels); i++ {
		go opts.BlazeProcessAddresses(meta, addressChannel)
	}

	// TODO: BOGUS IS USING THIS FILE THE BEST WAY - IS THIS A GOOD FILENAME
	tsFilename := config.GetPathToCache(opts.Chain) + "tmp/tempTsFile.txt"
	tsFile, err := os.Create(tsFilename)
	if err != nil {
		log.Fatalf("Unable to create file: %v", err)
	}
	defer func() {
		tsFile.Close()
		file.Copy(tsFilename, "./file.save")
	}()

	opts.TsWG.Add(int(opts.NChannels))
	for i := 0; i < int(opts.NChannels); i++ {
		go opts.BlazeProcessTimestamps(tsChannel, tsFile)
	}

	for block := int(opts.StartBlock); block < int(opts.StartBlock+opts.BlockCount); block++ {
		blockChannel <- block
	}

	close(blockChannel)
	opts.BlockWG.Wait()

	close(addressChannel)
	opts.AddressWG.Wait()

	close(tsChannel)
	opts.TsWG.Wait()

	return true, nil
}

// BlazeProcessBlocks Processes the block channel and for each block query the node for both traces and logs. Send results down addressChannel.
func (opts *BlazeOptions) BlazeProcessBlocks(meta *rpcClient.MetaData, blockChannel chan int, addressChannel chan ScrapedData, tsChannel chan tslib.Timestamp) {
	for blockNum := range blockChannel {

		// RPCPayload is used during to make calls to the RPC.
		var traces rpcClient.Traces
		tracePayload := rpcClient.RPCPayload{
			Jsonrpc:   "2.0",
			Method:    "trace_block",
			RPCParams: rpcClient.RPCParams{fmt.Sprintf("0x%x", blockNum)},
			ID:        1002,
		}
		err := rpcClient.FromRpc(opts.RpcProvider, &tracePayload, &traces)
		if err != nil {
			fmt.Println("FromRpc(traces) returned error", err)
			os.Exit(1)
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
			// TODO: BOGUS - RETURN VALUE FROM BLAZE
			fmt.Println("FromRpc(logs) returned error", err)
			os.Exit(1)
		}

		addressChannel <- ScrapedData{
			blockNumber: blockNum,
			traces:      traces,
			logs:        logs,
		}

		// TODO: BOGUS The timeStamp value is not used here (the Consolidation Loop calls into
		// TODO: BOGUS the same routine again). We should use this value here and remove the
		// TODO: BOGUS call from the Consolidation Loop
		tsChannel <- tslib.Timestamp{
			Ts: uint32(rpcClient.GetBlockTimestamp(opts.RpcProvider, uint64(blockNum))),
			Bn: uint32(blockNum),
		}
	}

	opts.BlockWG.Done()
}

// BlazeProcessAddresses processes ScrapedData objects shoved down the addressChannel
func (opts *BlazeOptions) BlazeProcessAddresses(meta *rpcClient.MetaData, addressChannel chan ScrapedData) {
	for sData := range addressChannel {
		addressMap := make(map[string]bool)
		opts.BlazeExtractFromTraces(sData.blockNumber, &sData.traces, addressMap)
		opts.BlazeExtractFromLogs(sData.blockNumber, &sData.logs, addressMap)
		opts.BlazeWriteAddresses(meta, sData.blockNumber, addressMap)
	}
	opts.AddressWG.Done()
}

var blazeMutex sync.Mutex

// BlazeProcessTimestamps processes timestamp data (currently by printing to a temporary file)
func (opts *BlazeOptions) BlazeProcessTimestamps(tsChannel chan tslib.Timestamp, tsFile *os.File) {
	for ts := range tsChannel {
		blazeMutex.Lock()
		// TODO: BOGUS - THIS COULD EASILY WRITE TO AN ARRAY NOT A FILE
		fmt.Fprintf(tsFile, "%s-%s\n", utils.PadLeft(strconv.Itoa(int(ts.Bn)), 9), utils.PadLeft(strconv.Itoa(int(ts.Ts)), 9))
		blazeMutex.Unlock()
	}
	opts.TsWG.Done()
}

func (opts *BlazeOptions) BlazeExtractFromTraces(bn int, traces *rpcClient.Traces, addressMap map[string]bool) {
	if traces.Result == nil || len(traces.Result) == 0 {
		return
	}

	blockNumStr := utils.PadLeft(strconv.Itoa(bn), 9)
	for i := 0; i < len(traces.Result); i++ {

		idx := utils.PadLeft(strconv.Itoa(traces.Result[i].TransactionPosition), 5)
		blockAndIdx := "\t" + blockNumStr + "\t" + idx

		if traces.Result[i].Type == "call" {
			// If it's a call, get the to and from
			from := traces.Result[i].Action.From
			if isAddress(from) {
				addressMap[from+blockAndIdx] = true
			}
			to := traces.Result[i].Action.To
			if isAddress(to) {
				addressMap[to+blockAndIdx] = true
			}

		} else if traces.Result[i].Type == "reward" {
			if traces.Result[i].Action.RewardType == "block" {
				author := traces.Result[i].Action.Author
				if validate.IsZeroAddress(author) {
					// Early clients allowed misconfigured miner settings with address
					// 0x0 (reward got burned). We enter a false record with a false tx_id
					// to account for this.
					author = "0xdeaddeaddeaddeaddeaddeaddeaddeaddeaddead"
					addressMap[author+"\t"+blockNumStr+"\t"+"99997"] = true

				} else {
					if isAddress(author) {
						addressMap[author+"\t"+blockNumStr+"\t"+"99999"] = true
					}
				}

			} else if traces.Result[i].Action.RewardType == "uncle" {
				author := traces.Result[i].Action.Author
				if validate.IsZeroAddress(author) {
					// Early clients allowed misconfigured miner settings with address
					// 0x0 (reward got burned). We enter a false record with a false tx_id
					// to account for this.
					author = "0xdeaddeaddeaddeaddeaddeaddeaddeaddeaddead"
					addressMap[author+"\t"+blockNumStr+"\t"+"99998"] = true

				} else {
					if isAddress(author) {
						addressMap[author+"\t"+blockNumStr+"\t"+"99998"] = true
					}
				}

			} else if traces.Result[i].Action.RewardType == "external" {
				// This only happens in xDai as far as we know...
				author := traces.Result[i].Action.Author
				if isAddress(author) {
					addressMap[author+"\t"+blockNumStr+"\t"+"99996"] = true
				}

			} else {
				fmt.Println("New type of reward", traces.Result[i].Action.RewardType)
			}

		} else if traces.Result[i].Type == "suicide" {
			// add the contract that died, and where it sent it's money
			address := traces.Result[i].Action.Address
			if isAddress(address) {
				addressMap[address+blockAndIdx] = true
			}
			refundAddress := traces.Result[i].Action.RefundAddress
			if isAddress(refundAddress) {
				addressMap[refundAddress+blockAndIdx] = true
			}

		} else if traces.Result[i].Type == "create" {
			// add the creator, and the new address name
			from := traces.Result[i].Action.From
			if isAddress(from) {
				addressMap[from+blockAndIdx] = true
			}
			address := traces.Result[i].Result.Address
			if isAddress(address) {
				addressMap[address+blockAndIdx] = true
			}

			// If it's a top level trace, then the call data is the init,
			// so to match with TrueBlocks, we just parse init
			if len(traces.Result[i].TraceAddress) == 0 {
				if len(traces.Result[i].Action.Init) > 10 {
					initData := traces.Result[i].Action.Init[10:]
					for i := 0; i < len(initData)/64; i++ {
						addr := string(initData[i*64 : (i+1)*64])
						if isImplicitAddress(addr) {
							addressMap[addr+blockAndIdx] = true
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
						err := rpcClient.FromRpc(opts.RpcProvider, &txReceiptPl, &receipt)
						if err != nil {
							// TODO: BOGUS - RETURN VALUE FROM BLAZE
							fmt.Println("FromRpc(transReceipt) returned error", err)
							os.Exit(1)
						}
						addr := receipt.Result.ContractAddress
						if isAddress(addr) {
							addressMap[addr+blockAndIdx] = true
						}
					}
				}
			}

		} else {
			err := "New trace type:" + traces.Result[i].Type
			// TODO: BOGUS - RETURN VALUE FROM BLAZE
			fmt.Println("extractFromTraces -->", err)
			os.Exit(1)
		}

		// Try to get addresses from the input data
		if len(traces.Result[i].Action.Input) > 10 {
			inputData := traces.Result[i].Action.Input[10:]
			//fmt.Println("Input data:", inputData, len(inputData))
			for i := 0; i < len(inputData)/64; i++ {
				addr := string(inputData[i*64 : (i+1)*64])
				if isImplicitAddress(addr) {
					addressMap[addr+blockAndIdx] = true
				}
			}
		}

		// Parse output of trace
		if len(traces.Result[i].Result.Output) > 2 {
			outputData := traces.Result[i].Result.Output[2:]
			for i := 0; i < len(outputData)/64; i++ {
				addr := string(outputData[i*64 : (i+1)*64])
				if isImplicitAddress(addr) {
					addressMap[addr+blockAndIdx] = true
				}
			}
		}
	}
}

// extractFromLogs Extracts addresses from any part of the log data.
func (opts *BlazeOptions) BlazeExtractFromLogs(bn int, logs *rpcClient.Logs, addressMap map[string]bool) {
	if logs.Result == nil || len(logs.Result) == 0 {
		return
	}

	blockNumStr := utils.PadLeft(strconv.Itoa(bn), 9)
	for i := 0; i < len(logs.Result); i++ {
		// Note: Maybe a bug? Does not process Log.Address (i.e., the emitter of the log)
		// Probably captured by the trace processing, but would be missed if we were
		// processing traces. Won't hurt to add (since the map removes dups) so it
		// should be added. Would be interested to test.

		idxInt, err := strconv.ParseInt(logs.Result[i].TransactionIndex, 0, 32)
		if err != nil {
			// TODO: BOGUS - RETURN VALUE FROM BLAZE
			fmt.Println("extractFromLogs --> strconv.ParseInt returned error", err)
			os.Exit(1)
		}
		idx := utils.PadLeft(strconv.FormatInt(idxInt, 10), 5)

		blockAndIdx := "\t" + blockNumStr + "\t" + idx

		for j := 0; j < len(logs.Result[i].Topics); j++ {
			addr := string(logs.Result[i].Topics[j][2:])
			if isImplicitAddress(addr) {
				addressMap[addr+blockAndIdx] = true
			}
		}

		if len(logs.Result[i].Data) > 2 {
			inputData := logs.Result[i].Data[2:]
			for i := 0; i < len(inputData)/64; i++ {
				addr := string(inputData[i*64 : (i+1)*64])
				if isImplicitAddress(addr) {
					addressMap[addr+blockAndIdx] = true
				}
			}
		}
	}
}

func (opts *BlazeOptions) BlazeWriteAddresses(meta *rpcClient.MetaData, bn int, addressMap map[string]bool) {
	if len(addressMap) == 0 {
		return
	}

	blockNumStr := utils.PadLeft(strconv.Itoa(bn), 9)
	addressArray := make([]string, 0, len(addressMap))
	for record := range addressMap {
		addressArray = append(addressArray, record)
	}
	sort.Strings(addressArray)

	toWrite := []byte(strings.Join(addressArray[:], "\n") + "\n")

	fileName := config.GetPathToIndex(opts.Chain) + "ripe/" + blockNumStr + ".txt"
	if bn > int(opts.RipeBlock) {
		fileName = config.GetPathToIndex(opts.Chain) + "unripe/" + blockNumStr + ".txt"
	}

	err := ioutil.WriteFile(fileName, toWrite, 0744)
	if err != nil {
		// TODO: BOGUS - RETURN VALUE FROM BLAZE
		fmt.Println("writeAddresses --> ioutil.WriteFile returned error", err)
		os.Exit(1)
	}

	// TODO: BOGUS - TESTING SCRAPING
	if !utils.OnOff {
		step := uint64(1)
		if opts.NProcessed%step == 0 {
			dist := uint64(0)
			if opts.RipeBlock > uint64(bn) {
				dist = (opts.RipeBlock - uint64(bn))
			}
			f := "-------- ( ------)- <PROG>  : Scraping %-04d of %-04d at block %d of %d (%d blocks from head)\r"
			fmt.Fprintf(os.Stderr, f, opts.NProcessed, opts.BlockCount, bn, opts.RipeBlock, dist)
		}
	}
	opts.NProcessed++
}
