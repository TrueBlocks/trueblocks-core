package main

import (
	"bufio"
	"bytes"
	"encoding/json"
	"fmt"
	"io/ioutil"
	"net/http"
	"os"
	"sort"
	"strconv"
	"strings"
	"sync"
)

// Params - used in calls to the RPC
type Params []interface{}

// JSONPayload - used to build requests to the RPC
type JSONPayload struct {
	Jsonrpc string `json:"jsonrpc"`
	Method  string `json:"method"`
	Params  `json:"params"`
	ID      int `json:"id"`
}

// getTracesForBlock - Returns all traces for a given block
func getTracesForBlock(blockNum int) ([]byte, error) {
	payloadBytes, err := json.Marshal(JSONPayload{"2.0", "trace_block", Params{fmt.Sprintf("0x%x", blockNum)}, 2})
	if err == nil {
		body := bytes.NewReader(payloadBytes)
		req, err := http.NewRequest("POST", "http://localhost:8545", body)
		if err == nil {
			req.Header.Set("Content-Type", "application/json")
			resp, err := http.DefaultClient.Do(req)
			if err == nil {
				tracesBody, err := ioutil.ReadAll(resp.Body)
				if err == nil {
					defer resp.Body.Close()
					return tracesBody, nil
				}
			}
		}
	}
	return nil, err
}

// Filter - Sent to getLogs to specify block range
type Filter struct {
	Fromblock string `json:"fromBlock"`
	Toblock   string `json:"toBlock"`
}

// Returns all logs for a given block
func getLogsForBlock(blockNum int) ([]byte, error) {
	payloadBytes, err := json.Marshal(JSONPayload{"2.0", "eth_getLogs", Params{Filter{fmt.Sprintf("0x%x", blockNum), fmt.Sprintf("0x%x", blockNum)}}, 2})
	if err == nil {
		body := bytes.NewReader(payloadBytes)
		req, err := http.NewRequest("POST", "http://localhost:8545", body)
		if err == nil {
			req.Header.Set("Content-Type", "application/json")
			resp, err := http.DefaultClient.Do(req)
			if err == nil {
				logsBody, err := ioutil.ReadAll(resp.Body)
				if err == nil {
					defer resp.Body.Close()
					return logsBody, nil
				}
			}
		}
	}
	return nil, err
}

// Returns recipt for a given transaction -- only used in errored contract creations
func getTransactionReceipt(hash string) ([]byte, error) {
	payloadBytes, err := json.Marshal(JSONPayload{"2.0", "eth_getTransactionReceipt", Params{hash}, 2})
	if err == nil {
		body := bytes.NewReader(payloadBytes)
		req, err := http.NewRequest("POST", "http://localhost:8545", body)
		if err == nil {
			req.Header.Set("Content-Type", "application/json")
			resp, err := http.DefaultClient.Do(req)
			if err == nil {
				receiptBody, err := ioutil.ReadAll(resp.Body)
				if err == nil {
					defer resp.Body.Close()
					return receiptBody, nil
				}
			}
		}
	}
	return nil, err
}

// TraceAndLogs - carries both the traces and the logs for a block
type TraceAndLogs struct {
	Traces []byte
	Logs   []byte
}

func getTraceAndLogs(blocks chan int, traceAndLogs chan TraceAndLogs, tracewg *sync.WaitGroup) {
	// Get blocks received on the blocks channel
	for blockNum := range blocks {
		traces, err := getTracesForBlock(blockNum)
		if err != nil {
			panic(err)
		}
		logs, err := getLogsForBlock(blockNum)
		if err != nil {
			panic(err)
		}
		traceAndLogs <- TraceAndLogs{traces, logs}
	}
	// decrement wait group
	tracewg.Done()
}

// BlockTraces - all traces in a block
type BlockTraces struct {
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

// BlockLogs - all logs in a block
type BlockLogs struct {
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

// TransReceipt - a given transaction's receipt
type TransReceipt struct {
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

func leftZero(str string, totalLen int) string {
	// Assume len(str) < totalLen
	zeros := ""
	for i := 0; i < totalLen-len(str); i++ {
		zeros += "0"
	}
	return zeros + str
}

func isGood(addr string) bool {
	if addr < "0x0000000000000000000000000000000000000009" {
		return false
	}
	return true
}

func isPotentialAddress(addr string) bool {

	small := "00000000000000000000000000000000000000ffffffffffffffffffffffffff"
	largePrefix := "000000000000000000000000"

	if addr <= small || !strings.HasPrefix(addr, largePrefix) {
		return false
	}

	if strings.HasSuffix(addr, "00000000") {
		return false
	}

	return true
}

func getTraceAddresses(addresses map[string]bool, traces *BlockTraces, blockNum string) {

	for i := 0; i < len(traces.Result); i++ {

		idx := leftZero(strconv.Itoa(traces.Result[i].TransactionPosition), 5)

		blockAndIdx := "\t" + blockNum + "\t" + idx
		// Try to get addresses from the input data
		if len(traces.Result[i].Action.Input) > 10 {
			inputData := traces.Result[i].Action.Input[10:]
			//fmt.Println("Input data:", inputData, len(inputData))
			for i := 0; i < len(inputData)/64; i++ {
				addr := string(inputData[i*64 : (i+1)*64])
				if isPotentialAddress(addr) {
					addr = "0x" + string(addr[24:])
					if isGood(addr) {
						addresses[addr+blockAndIdx] = true
					}
				}
			}
		}

		if traces.Result[i].Type == "call" {
			// If it's a call, get the to and from
			from := traces.Result[i].Action.From
			if isGood(from) {
				addresses[from+blockAndIdx] = true
			}
			to := traces.Result[i].Action.To
			if isGood(to) {
				addresses[to+blockAndIdx] = true
			}

		} else if traces.Result[i].Type == "reward" {
			if traces.Result[i].Action.RewardType == "block" {
				author := traces.Result[i].Action.Author
				if isGood(author) {
					addresses[author+"\t"+blockNum+"\t"+"99999"] = true
				}
			} else if traces.Result[i].Action.RewardType == "uncle" {

				//author := traces.Result[i].Action.Author
				//if isGood(author) {
				//  addresses[author + "\t" + blockNum + "\t" + "99998"] = true
				//}
			} else {
				fmt.Println("New type of reward", traces.Result[i].Action.RewardType)
			}
		} else if traces.Result[i].Type == "suicide" {
			// add the contract that died, and where it sent it's money
			address := traces.Result[i].Action.Address
			if isGood(address) {
				addresses[address+blockAndIdx] = true
			}
			refundAddress := traces.Result[i].Action.RefundAddress
			if isGood(refundAddress) {
				addresses[refundAddress+blockAndIdx] = true
			}

		} else if traces.Result[i].Type == "create" {
			// add the creator, and the new address name
			from := traces.Result[i].Action.From
			if isGood(from) {
				addresses[from+blockAndIdx] = true
			}
			address := traces.Result[i].Result.Address
			if isGood(address) {
				addresses[address+blockAndIdx] = true
			}

			// If it's a top level trace, then the call data is the init,
			// so to match with quickblocks, we just parse init
			if len(traces.Result[i].TraceAddress) == 0 {
				if len(traces.Result[i].Action.Init) > 10 {
					initData := traces.Result[i].Action.Init[10:]
					for i := 0; i < len(initData)/64; i++ {
						addr := string(initData[i*64 : (i+1)*64])
						if isPotentialAddress(addr) {
							addr = "0x" + string(addr[24:])
							if isGood(addr) {
								addresses[addr+blockAndIdx] = true
							}
						}
					}
				}
			}

			// How can we check if the contract creation has failed?
			// If the contract throws during construction, then I don't get that address
			// If this has failed, then I can get the

			// Handle contract creations that error out
			if traces.Result[i].Action.To == "" {
				if traces.Result[i].Result.Address == "" {
					if traces.Result[i].Error != "" {
						bytes, err := getTransactionReceipt(traces.Result[i].TransactionHash)
						if err != nil {
							panic(err)
						}
						var receipt TransReceipt
						err = json.Unmarshal(bytes, &receipt)
						if err != nil {
							panic(err)
						}
						addr := receipt.Result.ContractAddress
						if isGood(addr) {
							addresses[addr+blockAndIdx] = true
						}
					}
				}
			}

		} else {
			fmt.Println("New trace type:", traces.Result[i].Type)
		}

		// Parse output of trace
		if len(traces.Result[i].Result.Output) > 2 {
			outputData := traces.Result[i].Result.Output[2:]
			//fmt.Println("Input data:", inputData, len(inputData))
			for i := 0; i < len(outputData)/64; i++ {
				addr := string(outputData[i*64 : (i+1)*64])
				if isPotentialAddress(addr) {
					addr = "0x" + string(addr[24:])
					if isGood(addr) {
						addresses[addr+blockAndIdx] = true
					}
				}
			}
		}
	}
}

func getLogAddresses(addresses map[string]bool, logs *BlockLogs, blockNum string) {

	for i := 0; i < len(logs.Result); i++ {
		idxInt, err := strconv.ParseInt(logs.Result[i].TransactionIndex, 0, 32)
		if err != nil {
			fmt.Println("Error:", err)
		}
		idx := leftZero(strconv.FormatInt(idxInt, 10), 5)

		blockAndIdx := "\t" + blockNum + "\t" + idx

		for j := 0; j < len(logs.Result[i].Topics); j++ {
			addr := string(logs.Result[i].Topics[j][2:])
			if isPotentialAddress(addr) {
				addr = "0x" + string(addr[24:])
				if isGood(addr) {
					addresses[addr+blockAndIdx] = true
				}
			}
		}

		if len(logs.Result[i].Data) > 2 {
			inputData := logs.Result[i].Data[2:]
			for i := 0; i < len(inputData)/64; i++ {
				addr := string(inputData[i*64 : (i+1)*64])
				if isPotentialAddress(addr) {
					addr = "0x" + string(addr[24:])
					if isGood(addr) {
						addresses[addr+blockAndIdx] = true
					}
				}
			}
		}
	}
}

func writeAddresses(blockNum string, addresses map[string]bool) {

	addressArray := make([]string, len(addresses))
	idx := 0
	for address := range addresses {
		addressArray[idx] = address
		idx++
	}
	sort.Strings(addressArray)
	toWrite := []byte(strings.Join(addressArray[:], "\n") + "\n")

	folderPath := "blocks" //+ string(blockNum[:3]) + "/" + string(blockNum[3:6])

	if _, err := os.Stat(folderPath); os.IsNotExist(err) {
		os.MkdirAll(folderPath, os.ModePerm)
	}

	fileName := folderPath + "/" + blockNum + ".txt"
	err := ioutil.WriteFile(fileName, toWrite, 0777)
	if err != nil {
		fmt.Println("Error writing file:", err)
	}
	fmt.Print("Finished Block Processing:", blockNum, "\n")
}

func getAddress(traceAndLogs chan TraceAndLogs, addresswg *sync.WaitGroup) {
	for blockTraceAndLog := range traceAndLogs {
		//fmt.Println("Beginning Block Processing...")
		// Set of 'address \t block \t txIdx'
		addresses := make(map[string]bool)

		// Parse the traces
		var traces BlockTraces
		err := json.Unmarshal(blockTraceAndLog.Traces, &traces)
		if err != nil {
			panic(err)
		}
		blockNum := leftZero(strconv.Itoa(traces.Result[0].BlockNumber), 9)
		getTraceAddresses(addresses, &traces, blockNum)

		// Now, parse log data
		var logs BlockLogs
		err = json.Unmarshal(blockTraceAndLog.Logs, &logs)
		if err != nil {
			panic(err)
		}
		getLogAddresses(addresses, &logs, blockNum)

		// Write all of these addresses out to a file
		writeAddresses(blockNum, addresses)
	}
	addresswg.Done()
}

// Searching!

// AddrSighting - An appearance of an address
type AddrSighting struct {
	block int
	txIdx int
}

func searchForAddress(address string, fileNames chan string, sightings chan AddrSighting) {
	for fileName := range fileNames {
		data, err := ioutil.ReadFile("block/" + fileName)
		if err != nil {
			fmt.Println("Error:", err)
		}
		fmt.Print(string(data))
		sightings <- AddrSighting{0, 0}
	}
}

func testSearch() {
	fileNames := make(chan string)
	sightings := make(chan AddrSighting)

	for i := 0; i < 10; i++ {
		go searchForAddress("0xe3e1d84`f4d369faa89b01393b34a8193da6dead", fileNames, sightings)
	}

	for i := 6000000; i < 6000000+10000; i++ {
		fileName := leftZero(strconv.Itoa(i), 9) + ".txt"
		fileNames <- fileName
	}

	done := make(chan int)
	<-done
}

func readLines(path string) ([]string, error) {
	file, err := os.Open(path)
	if err != nil {
		return nil, err
	}
	defer file.Close()

	var lines []string
	// TODO: get file size, then make the slice that number of lines

	scanner := bufio.NewScanner(file)
	for scanner.Scan() {
		text := scanner.Text()
		lines = append(lines, text)
	}
	return lines, scanner.Err()
}

func writeLines(lines []string, path string) error {
	file, err := os.Create(path)
	if err != nil {
		return err
	}
	defer file.Close()

	w := bufio.NewWriter(file)
	for _, line := range lines {
		fmt.Fprintln(w, line)
	}
	return w.Flush()
}

/*
type BlockRecSize struct {
	blockNum int
	numRecords int
}

type MaxContigious struct {
	done []int
	contigiousTo int
	contigiousSum int
}

func newContiguous(size int) MaxContigious {
	done := make([]int, size)
	for i := 0; i < size; i++ {
		done[i] = -1
	}
	return MaxContigious{done, 0, 0}
}

func (c *MaxContigious) Finish(idx int, val int)  {
	if idx < 0 || idx >= len(c.done) {
		fmt.Println("Out of range")
		// todo: actually return error
		return
	}

	c.done[idx] = val
	if c.contigiousTo == idx - 1 {
		newContig := idx
		addSum := val
		for i := idx; i < len(c.done); i++ {
			newContig = i
			addSum += c.done[i] // TODO: maybe an off by one here
			if c.done[i] == -1 {
				break
			}
		}
		c.contigiousTo = newContig
		c.contigiousSum += addSum
	}
}

func contigious(size int) {}

func consolidator(startBlock int, numBlocks int, numRecords int, finishedBlocks chan BlockRecSize) {
	maxContigious := newContiguous(numBlocks)

	for blockRecSize := range finishedBlocks {
		maxContigious.Finish(blockRecSize.blockNum - startBlock, blockRecSize.numRecords)
		if maxContigious.contigiousSum > numRecords {
			//TODO
		}
		contigiousTo = maxContigious.contigiousTo


	}

} */

func consolidate(startBlock int, endBlock int, numRecords int) {
	currRecords := make([]string, 0)
	numCurrRecords := 0
	currStartBlock := startBlock
	for i := startBlock; i < endBlock; i++ {
		// read in file, and figure out number of records
		addressSightings, err := readLines("blocks/" + leftZero(strconv.Itoa(i), 9) + ".txt")
		if err != nil {
			fmt.Println("ERROR:", err)
		}
		currRecords = append(currRecords, addressSightings...)
		numCurrRecords += len(addressSightings)
		if numCurrRecords > numRecords {
			// sort, and then write file
			sort.Strings(currRecords)
			// TODO: can sort in linear time, as compared to log linear (b/c already sorted)
			writeLines(currRecords, "blocks/"+leftZero("a"+strconv.Itoa(currStartBlock), 9)+"-"+leftZero(strconv.Itoa(i), 9)+".txt")

			currRecords = make([]string, 0)
			numCurrRecords = 0
			currStartBlock = i + 1
		}
	}
	sort.Strings(currRecords)
	writeLines(currRecords, "blocks/"+leftZero("a"+strconv.Itoa(currStartBlock), 9)+"-"+leftZero(strconv.Itoa(endBlock), 9)+".txt")
}

func main() {
	startBlock := 2000000
	numBlocks := 2000

	consolidate(startBlock, startBlock+numBlocks, 20000)

	/*
		numTraceLogGetters := 20
		numGetAddresses := 100

		blocks := make(chan int)
		traceAndLogs := make(chan TraceAndLogs)

		// make a bunch of block trace getters
		var tracewg sync.WaitGroup
		tracewg.Add(numTraceLogGetters)
		for i := 0; i < numTraceLogGetters; i++ {
			go getTraceAndLogs(blocks, traceAndLogs, &tracewg)
		}

		var addresswg sync.WaitGroup
		addresswg.Add(numGetAddresses)
		for i := 0; i < numGetAddresses; i++ {
			go getAddress(traceAndLogs, &addresswg)
		}

		for block := startBlock; block < startBlock+numBlocks; block++ {
			blocks <- block
		}
		// close the channel here
		close(blocks)
		tracewg.Wait()

		// once all blocks have been processed
		close(traceAndLogs)
		addresswg.Wait() */

	// TODO: we can have a "finished blocks channel"
	// then, we can maintain a "finished" int array for when blocks are finished
	// the array stores the number of records in a block
	// then, we can maintain the "finished upto" variable
	// and the number of records that that contains
	// then, once we can process that, we do process that
}
