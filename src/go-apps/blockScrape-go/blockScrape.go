package main

import (
	"bytes"
	"encoding/json"
	"fmt"
	"io/ioutil"
	"net/http"
	"os"
	"sort"
	"strconv"
	"strings"
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

func getTraceAndLogs(blocks chan int, traceAndLogs chan TraceAndLogs) {
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

	if _, err := os.Stat("/path/to/whatever"); os.IsNotExist(err) {
		// path/to/whatever does not exist
	}

	folderPath := "blocks/" + string(blockNum[:3]) + "/" + string(blockNum[3:6])

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

func getAddress(traceAndLogs chan TraceAndLogs) {
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
		go writeAddresses(blockNum, addresses)
	}
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
		go searchForAddress("0xe3e1d847f4d369faa89b01393b34a8193da6dead", fileNames, sightings)
	}

	for i := 6000000; i < 6000000+10000; i++ {
		fileName := leftZero(strconv.Itoa(i), 9) + ".txt"
		fileNames <- fileName
	}

	done := make(chan int)
	<-done
}

func main() {
    print("Hello world\n")
/*
	//testSearch()

	startBlock := 224793
	numBlocks := 1 //00

	blocks := make(chan int)
	traceAndLogs := make(chan TraceAndLogs)

	// make a bunch of block trace getters
	for i := 0; i < 20; i++ {
		go getTraceAndLogs(blocks, traceAndLogs)
	}

	for i := 0; i < 100; i++ {
		go getAddress(traceAndLogs)
	}

	for block := startBlock; block < startBlock+numBlocks; block++ {
		blocks <- block
	}

	// blah, just wait around for ever (have to manuall terminate the process...)
	done := make(chan int)
	<-done
*/
}
