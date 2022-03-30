package scrapePkg

import (
	"fmt"
	"io/ioutil"
	"log"
	"os"
	"sort"
	"strconv"
	"strings"
	"sync"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

// ScrapedData combines the block data, trace data, and log data into a single structure
type ScrapedData struct {
	block  int
	ts     uint64
	traces rpcClient.Trace
	logs   rpcClient.Log
}

func (opts *ScrapeOptions) ScrapeBlocks() {
	rpcProvider := config.GetRpcProvider(opts.Globals.Chain)

	blockChannel := make(chan int)
	addressChannel := make(chan ScrapedData)

	var blockWG sync.WaitGroup
	blockWG.Add(int(opts.BlockChanCnt))
	for i := 0; i < int(opts.BlockChanCnt); i++ {
		go opts.processBlocks(rpcProvider, blockChannel, addressChannel, &blockWG)
	}

	var addressWG sync.WaitGroup
	addressWG.Add(int(opts.AddrChanCnt))
	for i := 0; i < int(opts.AddrChanCnt); i++ {
		go opts.extractAddresses(rpcProvider, addressChannel, &addressWG)
	}

	for block := int(opts.StartBlock); block < int(opts.StartBlock+opts.BlockCnt); block++ {
		blockChannel <- block
	}

	close(blockChannel)
	blockWG.Wait()

	close(addressChannel)
	addressWG.Wait()
}

// processBlocks Process the block channel and for each block query the node for both traces and logs. Send results to addressChannel
func (opts *ScrapeOptions) processBlocks(rpcProvider string, blockChannel chan int, addressChannel chan ScrapedData, blockWG *sync.WaitGroup) {
	for blockNum := range blockChannel {

		var traces rpcClient.Trace
		tracePl := rpcClient.RPCPayload{"2.0", "trace_block", rpcClient.RPCParams{fmt.Sprintf("0x%x", blockNum)}, 1002}
		err := rpcClient.FromRpc(rpcProvider, &tracePl, &traces)
		if err != nil {
			fmt.Println("FromRpc(traces) returned error")
			log.Fatal(err)
		}

		var logs rpcClient.Log
		logsPl := rpcClient.RPCPayload{"2.0", "eth_getLogs", rpcClient.RPCParams{rpcClient.LogFilter{fmt.Sprintf("0x%x", blockNum), fmt.Sprintf("0x%x", blockNum)}}, 1003}
		err = rpcClient.FromRpc(rpcProvider, &logsPl, &logs)
		if err != nil {
			fmt.Println("FromRpc(logs) returned error")
			log.Fatal(err)
		}

		ts := rpcClient.GetBlockTimestamp(rpcProvider, uint64(blockNum))
		addressChannel <- ScrapedData{blockNum, ts, traces, logs}
	}

	blockWG.Done()
}

func (opts *ScrapeOptions) extractAddresses(rpcProvider string, addressChannel chan ScrapedData, addressWG *sync.WaitGroup) {

	for blockTraceAndLog := range addressChannel {
		bn := blockTraceAndLog.block
		addressMap := make(map[string]bool)
		opts.extractFromTraces(rpcProvider, bn, addressMap, &blockTraceAndLog.traces)
		opts.extractFromLogs(bn, addressMap, &blockTraceAndLog.logs)
		opts.writeAddresses(bn, addressMap)
	}

	addressWG.Done()
}

func (opts *ScrapeOptions) extractFromTraces(rpcProvider string, bn int, addressMap map[string]bool, traces *rpcClient.Trace) {
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
			if goodAddr(from) {
				addressMap[from+blockAndIdx] = true
			}
			to := traces.Result[i].Action.To
			if goodAddr(to) {
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
					if goodAddr(author) {
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
					if goodAddr(author) {
						addressMap[author+"\t"+blockNumStr+"\t"+"99998"] = true
					}
				}

			} else if traces.Result[i].Action.RewardType == "external" {
				// This only happens in xDai as far as we know...
				author := traces.Result[i].Action.Author
				if goodAddr(author) {
					addressMap[author+"\t"+blockNumStr+"\t"+"99996"] = true
				}

			} else {
				fmt.Println("New type of reward", traces.Result[i].Action.RewardType)
			}

		} else if traces.Result[i].Type == "suicide" {
			// add the contract that died, and where it sent it's money
			address := traces.Result[i].Action.Address
			if goodAddr(address) {
				addressMap[address+blockAndIdx] = true
			}
			refundAddress := traces.Result[i].Action.RefundAddress
			if goodAddr(refundAddress) {
				addressMap[refundAddress+blockAndIdx] = true
			}

		} else if traces.Result[i].Type == "create" {
			// add the creator, and the new address name
			from := traces.Result[i].Action.From
			if goodAddr(from) {
				addressMap[from+blockAndIdx] = true
			}
			address := traces.Result[i].Result.Address
			if goodAddr(address) {
				addressMap[address+blockAndIdx] = true
			}

			// If it's a top level trace, then the call data is the init,
			// so to match with TrueBlocks, we just parse init
			if len(traces.Result[i].TraceAddress) == 0 {
				if len(traces.Result[i].Action.Init) > 10 {
					initData := traces.Result[i].Action.Init[10:]
					for i := 0; i < len(initData)/64; i++ {
						addr := string(initData[i*64 : (i+1)*64])
						if potentialAddress(addr) {
							addr = "0x" + string(addr[24:])
							if goodAddr(addr) {
								addressMap[addr+blockAndIdx] = true
							}
						}
					}
				}
			}

			// Handle contract creations that error out
			if traces.Result[i].Action.To == "" {
				if traces.Result[i].Result.Address == "" {
					if traces.Result[i].Error != "" {
						var receipt rpcClient.Receipt
						var txReceiptPl = rpcClient.RPCPayload{"2.0", "eth_getTransactionReceipt", rpcClient.RPCParams{traces.Result[i].TransactionHash}, 1005}
						err := rpcClient.FromRpc(rpcProvider, &txReceiptPl, &receipt)
						if err != nil {
							fmt.Println("FromRpc(transReceipt) returned error")
							log.Fatal(err)
						}
						addr := receipt.Result.ContractAddress
						if goodAddr(addr) {
							addressMap[addr+blockAndIdx] = true
						}
					}
				}
			}

		} else {
			err := "New trace type:" + traces.Result[i].Type
			fmt.Println("extractFromTraces -->")
			log.Fatal(err)
		}

		// Try to get addresses from the input data
		if len(traces.Result[i].Action.Input) > 10 {
			inputData := traces.Result[i].Action.Input[10:]
			//fmt.Println("Input data:", inputData, len(inputData))
			for i := 0; i < len(inputData)/64; i++ {
				addr := string(inputData[i*64 : (i+1)*64])
				if potentialAddress(addr) {
					addr = "0x" + string(addr[24:])
					if goodAddr(addr) {
						addressMap[addr+blockAndIdx] = true
					}
				}
			}
		}

		// Parse output of trace
		if len(traces.Result[i].Result.Output) > 2 {
			outputData := traces.Result[i].Result.Output[2:]
			for i := 0; i < len(outputData)/64; i++ {
				addr := string(outputData[i*64 : (i+1)*64])
				if potentialAddress(addr) {
					addr = "0x" + string(addr[24:])
					if goodAddr(addr) {
						addressMap[addr+blockAndIdx] = true
					}
				}
			}
		}
	}
}

// extractFromLogs Extracts addresses from any part of the log data.
func (opts *ScrapeOptions) extractFromLogs(bn int, addressMap map[string]bool, logs *rpcClient.Log) {
	if logs.Result == nil || len(logs.Result) == 0 {
		return
	}

	blockNumStr := utils.PadLeft(strconv.Itoa(bn), 9)
	for i := 0; i < len(logs.Result); i++ {
		idxInt, err := strconv.ParseInt(logs.Result[i].TransactionIndex, 0, 32)
		if err != nil {
			fmt.Println("extractFromLogs --> strconv.ParseInt returned error")
			log.Fatal(err)
		}
		idx := utils.PadLeft(strconv.FormatInt(idxInt, 10), 5)

		blockAndIdx := "\t" + blockNumStr + "\t" + idx

		for j := 0; j < len(logs.Result[i].Topics); j++ {
			addr := string(logs.Result[i].Topics[j][2:])
			if potentialAddress(addr) {
				addr = "0x" + string(addr[24:])
				if goodAddr(addr) {
					addressMap[addr+blockAndIdx] = true
				}
			}
		}

		if len(logs.Result[i].Data) > 2 {
			inputData := logs.Result[i].Data[2:]
			for i := 0; i < len(inputData)/64; i++ {
				addr := string(inputData[i*64 : (i+1)*64])
				if potentialAddress(addr) {
					addr = "0x" + string(addr[24:])
					if goodAddr(addr) {
						addressMap[addr+blockAndIdx] = true
					}
				}
			}
		}
	}
}

var nProcessed uint64 = 0

func (opts *ScrapeOptions) writeAddresses(bn int, addressMap map[string]bool) {
	if len(addressMap) == 0 {
		return
	}

	blockNumStr := utils.PadLeft(strconv.Itoa(bn), 9)
	addressArray := make([]string, len(addressMap))
	idx := 0
	for address := range addressMap {
		addressArray[idx] = address
		idx++
	}
	sort.Strings(addressArray)

	toWrite := []byte(strings.Join(addressArray[:], "\n") + "\n")

	bn, _ = strconv.Atoi(blockNumStr)
	fileName := config.GetPathToIndex(opts.Globals.Chain) + "ripe/" + blockNumStr + ".txt"
	if bn > int(opts.RipeBlock) {
		fileName = config.GetPathToIndex(opts.Globals.Chain) + "unripe/" + blockNumStr + ".txt"
	}

	err := ioutil.WriteFile(fileName, toWrite, 0744)
	if err != nil {
		fmt.Println("writeAddresses --> ioutil.WriteFile returned error")
		log.Fatal(err)
	}

	step := uint64(7)
	if nProcessed%step == 0 {
		f := "-------- ( ------)- <PROG>  : Scraping %-04d of %-04d at block %s of %d (%d blocks from head)\r"
		fmt.Fprintf(os.Stderr, f, nProcessed, opts.BlockCnt, blockNumStr, opts.RipeBlock, (opts.RipeBlock - uint64(bn)))
	}
	nProcessed++
}

// goodAddr Returns true if the address is not a precompile and not the zero address
func goodAddr(addr string) bool {
	// As per EIP 1352, all addresses less or equal to the following value are reserved for pre-compiles.
	// We don't index precompiles. https://eips.ethereum.org/EIPS/eip-1352
	return addr > "0x000000000000000000000000000000000000ffff"
}

// potentialAddress processes a transaction's 'input' data and 'output' data or an event's data field. We call anything
// with 12 bytes of leading zeros but not more than 19 leading zeros (24 and 38 characters respectively).
func potentialAddress(addr string) bool {
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
	return true
}

// TODO:
// TODO: This "baddress"
// TODO:
// TODO: 0x00000000000004ee2d6d415371e298f092cc0000
// TODO:
// TODO: appears in the index but it is not an actual address. It appears only four times in the entire index.
// TODO: We know this is not an address because it only appears the event 'data' section for Transfers or Approvals
// TODO: which we know to be the value, not an address.
// TODO:
// TODO: The trouble is knowing this is a "non-chain knowledge leak." The chain itself knows nothing about
// TODO: ERC20 tokens. I'm not sure how many 'false records' (or baddresses) this would remove, but it may
// TODO: be significant given that Transfers and Approvals dominate the chain data.
// TODO:
// TODO: What we could do is this:
// TODO:
// TODO: If we're scraping a log, and
// TODO:
// TODO: 	If we see certain topics (topic[0] is a Transfer or Approval, we do not include the value
// TODO:	even if it looks like an address. This is a very slippery slope. What does 'well known' mean?
// TODO:
// TODO: Another downside; implementing this would require a full re-generation of the index and would
// TODO: change the hashes and the underlying files. In order to do this, we would require a migration that
// TODO: removes the 'old' index from the end user's machine and then downloads the new index. We can do this,
// TODO: but it feels quite precarious.
// TODO:
// TODO: My expectation is that we will eventually have to re-generate the index at some point (version 1.0?).
// TODO: We can this then.
// TODO:
