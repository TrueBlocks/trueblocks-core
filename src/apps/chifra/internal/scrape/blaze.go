package scrapePkg

import (
	"bytes"
	"encoding/json"
	"fmt"
	"io/ioutil"
	"log"
	"net/http"
	"os"
	"sort"
	"strconv"
	"strings"
	"sync"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
)

func (opts *ScrapeOptions) ScrapeBlocks() {
	// opts.Globals.LogLevel = 10

	blockChannel := make(chan int)
	addressChannel := make(chan tracesAndLogs)

	var blockWG sync.WaitGroup
	blockWG.Add(int(opts.BlockChanCnt))
	for i := 0; i < int(opts.BlockChanCnt); i++ {
		go opts.processBlocks(blockChannel, addressChannel, &blockWG)
	}

	var addressWG sync.WaitGroup
	addressWG.Add(int(opts.AddrChanCnt))
	for i := 0; i < int(opts.AddrChanCnt); i++ {
		go opts.extractAddresses(addressChannel, &addressWG)
	}

	for block := int(opts.StartBlock); block < int(opts.StartBlock+opts.BlockCnt); block++ {
		blockChannel <- block
	}

	close(blockChannel)
	blockWG.Wait()

	close(addressChannel)
	addressWG.Wait()
}

// tracesAndLogs combines traces and logs to make processing easier
type tracesAndLogs struct {
	block  int
	header []byte
	traces Trace
	logs   Log
}

// processBlocks Process the block channel and for each block query the node for both traces and logs. Send results to addressChannel
func (opts *ScrapeOptions) processBlocks(blockChannel chan int, addressChannel chan tracesAndLogs, blockWG *sync.WaitGroup) {

	for blockNum := range blockChannel {

		header, err := opts.getBlockHeader(blockNum)
		if err != nil {
			fmt.Println("processBlocks --> opts.getBlockHeader returned error")
			log.Fatal(err)
		}

		var traces Trace
		traceBytes, err := opts.getTracesFromBlock(blockNum)
		if err != nil {
			fmt.Println("processBlocks --> opts.getTracesFromBlock returned error")
			log.Fatal(err)
		}
		err = json.Unmarshal(traceBytes, &traces)
		if err != nil {
			fmt.Println("extractAddresses --> json.Unmarshal1 returned error")
			log.Fatal(err)
		}

		var logs Log
		logsBytes, err := opts.getLogsFromBlock(blockNum)
		if err != nil {
			fmt.Println("processBlocks --> opts.getLogsFromBlock returned error")
			log.Fatal(err)
		}
		err = json.Unmarshal(logsBytes, &logs)
		if err != nil {
			fmt.Println("extractAddresses --> json.Unmarshal2 returned error")
			log.Fatal(err)
		}

		addressChannel <- tracesAndLogs{blockNum, header, traces, logs}
	}
	blockWG.Done()
}

func (opts *ScrapeOptions) extractAddresses(addressChannel chan tracesAndLogs, addressWG *sync.WaitGroup) {

	for blockTraceAndLog := range addressChannel {
		bn := blockTraceAndLog.block
		addressMap := make(map[string]bool)
		opts.extractFromTraces(bn, addressMap, &blockTraceAndLog.traces)
		opts.extractFromLogs(bn, addressMap, &blockTraceAndLog.logs)
		opts.writeAddresses(bn, addressMap)
	}

	addressWG.Done()
}

func (opts *ScrapeOptions) extractFromTraces(bn int, addressMap map[string]bool, traces *Trace) {
	if traces.Result == nil || len(traces.Result) == 0 {
		return
	}

	blockNumStr := padLeft(strconv.Itoa(bn), 9)
	for i := 0; i < len(traces.Result); i++ {

		idx := padLeft(strconv.Itoa(traces.Result[i].TransactionPosition), 5)
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
				if author == "0x0" || author == "0x0000000000000000000000000000000000000000" {
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
				if author == "0x0" || author == "0x0000000000000000000000000000000000000000" {
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
						bytes, err := opts.getTransactionReceipt(traces.Result[i].TransactionHash)
						if err != nil {
							fmt.Println("extractFromTraces --> getTransactionReceipt returned error")
							log.Fatal(err)
						}
						var receipt Receipt
						err = json.Unmarshal(bytes, &receipt)
						if err != nil {
							fmt.Println("extractFromTraces --> json.Unmarshal returned error")
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
func (opts *ScrapeOptions) extractFromLogs(bn int, addressMap map[string]bool, logs *Log) {
	if logs.Result == nil || len(logs.Result) == 0 {
		return
	}

	blockNumStr := padLeft(strconv.Itoa(bn), 9)
	for i := 0; i < len(logs.Result); i++ {
		idxInt, err := strconv.ParseInt(logs.Result[i].TransactionIndex, 0, 32)
		if err != nil {
			fmt.Println("extractFromLogs --> strconv.ParseInt returned error")
			log.Fatal(err)
		}
		idx := padLeft(strconv.FormatInt(idxInt, 10), 5)

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

// TODO: BOGUS
var counter12 uint64 = 0

func (opts *ScrapeOptions) writeAddresses(bn int, addressMap map[string]bool) {
	if len(addressMap) == 0 {
		return
	}

	blockNumStr := padLeft(strconv.Itoa(bn), 9)
	addressArray := make([]string, len(addressMap))
	idx := 0
	for address := range addressMap {
		addressArray[idx] = address
		idx++
	}
	sort.Strings(addressArray)
	toWrite := []byte(strings.Join(addressArray[:], "\n") + "\n")

	bn, _ := strconv.Atoi(blockNumStr)
	fileName := config.GetPathToIndex(opts.Globals.Chain) + "ripe/" + blockNumStr + ".txt"
	if bn > int(opts.RipeBlock) {
		fileName = config.GetPathToIndex(opts.Globals.Chain) + "unripe/" + blockNumStr + ".txt"
	}

	err := ioutil.WriteFile(fileName, toWrite, 0744)
	if err != nil {
		fmt.Println("writeAddresses --> ioutil.WriteFile returned error")
		log.Fatal(err)
	}
	// Show fifty dots no matter how many blocks we're scraping
	// TODO: BOGUS
	step := uint64(7)
	counter12++
	if counter12%step == 0 {
		fmt.Fprintf(os.Stderr, "-------- ( ------)- <PROG>  : Scraping %-04d of %-04d at block %s\r", counter12, opts.BlockCnt, blockNumStr)
	}
}

func padLeft(str string, totalLen int) string {
	if len(str) >= totalLen {
		return str
	}
	zeros := ""
	for i := 0; i < totalLen-len(str); i++ {
		zeros += "0"
	}
	return zeros + str
}

// TODO: This "baddress"
// TODO:
// TODO: 0x00000000000004ee2d6d415371e298f092cc0000
// TODO:
// TODO: appears in the index but it is clearly not a real address. We know this because it appears only four
// TODO: times in the entire index and for each of those four times it appears in an event's data' section.
// TODO: Each of those events are either Transfer or Approval`.
// TODO:
// TODO: We could, if we wished, allow a tiny bit of non-chain knowledge leak into the scrape to avoid adding these
// TODO: 'false' badresses to the index. I'm not sure how many records this would remove, but it may be significant
// TODO: and it is very clearly true that these are not addresses.
// TODO:
// TODO: So, the rule:
// TODO:
// TODO: When looking at logs
// TODO:
// TODO: For some set of topics, (that is, topic[0] is one of Transfer, etc which are well known,
// TODO: Do not include the value even if it looks like an address
// TODO: This, of course, a very slippery slope as who's to say which topics are 'well known'?
// TODO:
// TODO: blockScrape: Easy way to eliminate false positive addresses during scrape hasno dependencies
// TODO:
// TODO: I will add this comment to the appropriate place in the code, but leave it commented out. Implementing this
// TODO: would require a full re-generation of the index and would change the hashes and the underlying files.
// TODO: In order to do this, we would require a migration that removes the 'old' index from the end user's
// TODO: machine and then downloads the new index. We can do this, but it feels quite precarious.
// TODO:
// TODO: My expectation is that we will eventually have to re-generate the index. We'll fix this then.
// TODO:

// getBlockHeader Returns all traces for a given block.
func (opts *ScrapeOptions) getBlockHeader(bn int) ([]byte, error) {
	payloadBytes, err := json.Marshal(RPCPayload{"2.0", "eth_getBlockByNumber", RPCParams{LogFilter{fmt.Sprintf("0x%x", bn), "false"}}, 2})
	if err != nil {
		fmt.Println(err)
		return nil, err
	}

	body := bytes.NewReader(payloadBytes)
	req, err := http.NewRequest("POST", config.GetRpcProvider(opts.Globals.Chain), body)
	if err != nil {
		fmt.Println(err)
		return nil, err
	}

	req.Header.Set("Content-Type", "application/json")
	resp, err := http.DefaultClient.Do(req)
	if err != nil {
		fmt.Println(err)
		return nil, err
	}
	defer resp.Body.Close()
	theBytes, err := ioutil.ReadAll(resp.Body)
	if err != nil {
		fmt.Println(err)
		return nil, err
	}

	return theBytes, nil
}

// getTracesFromBlock Returns all traces for a given block.
func (opts *ScrapeOptions) getTracesFromBlock(bn int) ([]byte, error) {
	payloadBytes, err := json.Marshal(RPCPayload{"2.0", "trace_block", RPCParams{fmt.Sprintf("0x%x", bn)}, 2})
	if err != nil {
		fmt.Println(err)
		return nil, err
	}

	body := bytes.NewReader(payloadBytes)
	req, err := http.NewRequest("POST", config.GetRpcProvider(opts.Globals.Chain), body)
	if err != nil {
		fmt.Println(err)
		return nil, err
	}

	req.Header.Set("Content-Type", "application/json")
	resp, err := http.DefaultClient.Do(req)
	if err != nil {
		fmt.Println(err)
		return nil, err
	}
	defer resp.Body.Close()
	theBytes, err := ioutil.ReadAll(resp.Body)
	if err != nil {
		fmt.Println(err)
		return nil, err
	}

	return theBytes, nil
}

// getLogsFromBlock Returns all logs for a given block.
func (opts *ScrapeOptions) getLogsFromBlock(bn int) ([]byte, error) {
	payloadBytes, err := json.Marshal(RPCPayload{"2.0", "eth_getLogs", RPCParams{LogFilter{fmt.Sprintf("0x%x", bn), fmt.Sprintf("0x%x", bn)}}, 2})
	if err != nil {
		fmt.Println(err)
		return nil, err
	}

	body := bytes.NewReader(payloadBytes)
	req, err := http.NewRequest("POST", config.GetRpcProvider(opts.Globals.Chain), body)
	if err != nil {
		fmt.Println(err)
		return nil, err
	}

	req.Header.Set("Content-Type", "application/json")
	resp, err := http.DefaultClient.Do(req)
	if err != nil {
		fmt.Println(err)
		return nil, err
	}
	defer resp.Body.Close()
	theBytes, err := ioutil.ReadAll(resp.Body)
	if err != nil {
		fmt.Println(err)
		return nil, err
	}

	return theBytes, nil
}

// getTransactionReceipt Returns recipt for a given transaction -- only used in errored contract creations
func (opts *ScrapeOptions) getTransactionReceipt(hash string) ([]byte, error) {
	payloadBytes, err := json.Marshal(RPCPayload{"2.0", "eth_getTransactionReceipt", RPCParams{hash}, 2})
	if err != nil {
		fmt.Println(err)
		return nil, err
	}

	body := bytes.NewReader(payloadBytes)
	req, err := http.NewRequest("POST", config.GetRpcProvider(opts.Globals.Chain), body)
	if err != nil {
		fmt.Println(err)
		return nil, err
	}

	req.Header.Set("Content-Type", "application/json")
	resp, err := http.DefaultClient.Do(req)
	if err != nil {
		fmt.Println(err)
		return nil, err
	}

	receiptBody, err := ioutil.ReadAll(resp.Body)
	if err != nil {
		fmt.Println(err)
		return nil, err
	}

	defer resp.Body.Close()
	return receiptBody, nil
}

// BlockHeader carries values returned by the `eth_getBlock` RPC command
type BlockHeader struct {
	Jsonrpc string `json:"jsonrpc"`
	Result  struct {
		Author           string   `json:"author"`
		Difficulty       string   `json:"difficulty"`
		ExtraData        string   `json:"extraData"`
		GasLimit         string   `json:"gasLimit"`
		GasUsed          string   `json:"gasUsed"`
		Hash             string   `json:"hash"`
		LogsBloom        string   `json:"logsBloom"`
		Miner            string   `json:"miner"`
		MixHash          string   `json:"mixHash"`
		Nonce            string   `json:"nonce"`
		Number           string   `json:"number"`
		ParentHash       string   `json:"parentHash"`
		ReceiptsRoot     string   `json:"receiptsRoot"`
		SealFields       []string `json:"sealFields"`
		Sha3Uncles       string   `json:"sha3Uncles"`
		Size             string   `json:"size"`
		StateRoot        string   `json:"stateRoot"`
		Timestamp        string   `json:"timestamp"`
		TransactionsRoot string   `json:"transactionsRoot"`
	} `json:"result"`
	ID int `json:"id"`
}

// Trace carries values returned by Parity's `trace_block` RPC command
type Trace struct {
	Jsonrpc string `json:"jsonrpc"`
	Result  []struct {
		Action struct {
			CallType      string `json:"callType"` // call
			From          string `json:"from"`
			Gas           string `json:"gas"`
			Input         string `json:"input"`
			To            string `json:"to"`
			Value         string `json:"value"`
			Author        string `json:"author"` // reward
			RewardType    string `json:"rewardType"`
			Address       string `json:"address"` // suicide
			Balance       string `json:"balance"`
			RefundAddress string `json:"refundAddress"`
			Init          string `json:"init"` // create
		} `json:"action,omitempty"`
		BlockHash   string `json:"blockHash"`
		BlockNumber int    `json:"blockNumber"`
		Error       string `json:"error"`
		Result      struct {
			GasUsed string `json:"gasUsed"` // call
			Output  string `json:"output"`
			Address string `json:"address"` // create
		} `json:"result"`
		Subtraces           int           `json:"subtraces"`
		TraceAddress        []interface{} `json:"traceAddress"`
		TransactionHash     string        `json:"transactionHash"`
		TransactionPosition int           `json:"transactionPosition"`
		Type                string        `json:"type"`
	} `json:"result"`
	ID int `json:"id"`
}

// Log carries values returned by the eth_getLogs RPC command
type Log struct {
	Jsonrpc string `json:"jsonrpc"`
	Result  []struct {
		Address             string   `json:"address"`
		BlockHash           string   `json:"blockHash"`
		BlockNumber         string   `json:"blockNumber"`
		Data                string   `json:"data"`
		LogIndex            string   `json:"logIndex"`
		Removed             bool     `json:"removed"`
		Topics              []string `json:"topics"`
		TransactionHash     string   `json:"transactionHash"`
		TransactionIndex    string   `json:"transactionIndex"`
		TransactionLogIndex string   `json:"transactionLogIndex"`
		Type                string   `json:"type"`
	} `json:"result"`
	ID int `json:"id"`
}

// Receipt carries values returned by the eth_getReceipt RPC call
type Receipt struct {
	Jsonrpc string `json:"jsonrpc"`
	Result  struct {
		BlockHash         string        `json:"blockHash"`
		BlockNumber       string        `json:"blockNumber"`
		ContractAddress   string        `json:"contractAddress"`
		CumulativeGasUsed string        `json:"cumulativeGasUsed"`
		From              string        `json:"from"`
		GasUsed           string        `json:"gasUsed"`
		Logs              []interface{} `json:"logs"`
		LogsBloom         string        `json:"logsBloom"`
		Root              string        `json:"root"`
		Status            interface{}   `json:"status"`
		To                interface{}   `json:"to"`
		TransactionHash   string        `json:"transactionHash"`
		TransactionIndex  string        `json:"transactionIndex"`
	} `json:"result"`
	ID int `json:"id"`
}

// RPCParams are used during calls to the RPC.
type RPCParams []interface{}

// RPCPayload is used during to make calls to the RPC.
type RPCPayload struct {
	Jsonrpc   string `json:"jsonrpc"`
	Method    string `json:"method"`
	RPCParams `json:"params"`
	ID        int `json:"id"`
}

// LogFilter is used the eth_getLogs RPC call to identify the block range to query
type LogFilter struct {
	Fromblock string `json:"fromBlock"`
	Toblock   string `json:"toBlock"`
}

// goodAddr Returns true if the address is not a precompile and not zero
func goodAddr(addr string) bool {
	// As per EIP 1352, all addresses less or equal to the following
	// value are reserved for pre-compiles. We don't index precompiles.
	// https://eips.ethereum.org/EIPS/eip-1352
	return addr > "0x000000000000000000000000000000000000ffff"
}

// potentialAddress Processing 'input' value, 'output' value or event 'data' value
// we do our best, but we don't include everything we could. We do the best we can
func potentialAddress(addr string) bool {
	// Any address smaller than this we call a 'baddress' and do not index
	small := "00000000000000000000000000000000000000ffffffffffffffffffffffffff"
	//        -------+-------+-------+-------+-------+-------+-------+-------+
	if addr <= small {
		return false
	}

	// Any address with less than this many leading zeros is not an left-padded 20-byte address
	largePrefix := "000000000000000000000000"
	//              -------+-------+-------+
	if !strings.HasPrefix(addr, largePrefix) {
		return false
	}

	if strings.HasSuffix(addr, "00000000") {
		return false
	}
	return true
}
