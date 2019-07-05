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
	"sync"
	"time"
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

func extractInternals(blockChannel chan int, addressChannel chan BlockInternals, blockWG *sync.WaitGroup) {
	for blockNum := range blockChannel {
		traces, err := getTracesForBlock(blockNum)
		if err != nil {
			panic(err)
		}
		logs, err := getLogsForBlock(blockNum)
		if err != nil {
			panic(err)
		}
		addressChannel <- BlockInternals{traces, logs}
	}
	blockWG.Done()
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

// BlockInternals - carries both the traces and the logs for a block
type BlockInternals struct {
	Traces []byte
	Logs   []byte
}

func getTraceAddresses(addresses map[string]bool, traces *BlockTraces, blockNum string) {

	for i := 0; i < len(traces.Result); i++ {

		idx := PadLeft(strconv.Itoa(traces.Result[i].TransactionPosition), 5)

		blockAndIdx := "\t" + blockNum + "\t" + idx
		// Try to get addresses from the input data
		if len(traces.Result[i].Action.Input) > 10 {
			inputData := traces.Result[i].Action.Input[10:]
			//fmt.Println("Input data:", inputData, len(inputData))
			for i := 0; i < len(inputData)/64; i++ {
				addr := string(inputData[i*64 : (i+1)*64])
				if PotentialAddress(addr) {
					addr = "0x" + string(addr[24:])
					if GoodAddr(addr) {
						addresses[addr+blockAndIdx] = true
					}
				}
			}
		}

		if traces.Result[i].Type == "call" {
			// If it's a call, get the to and from
			from := traces.Result[i].Action.From
			if GoodAddr(from) {
				addresses[from+blockAndIdx] = true
			}
			to := traces.Result[i].Action.To
			if GoodAddr(to) {
				addresses[to+blockAndIdx] = true
			}

		} else if traces.Result[i].Type == "reward" {
			if traces.Result[i].Action.RewardType == "block" {
				author := traces.Result[i].Action.Author
				if GoodAddr(author) {
					addresses[author+"\t"+blockNum+"\t"+"99999"] = true
				}
			} else if traces.Result[i].Action.RewardType == "uncle" {

				//author := traces.Result[i].Action.Author
				//if GoodAddr(author) {
				//  addresses[author + "\t" + blockNum + "\t" + "99998"] = true
				//}
			} else {
				fmt.Println("New type of reward", traces.Result[i].Action.RewardType)
			}
		} else if traces.Result[i].Type == "suicide" {
			// add the contract that died, and where it sent it's money
			address := traces.Result[i].Action.Address
			if GoodAddr(address) {
				addresses[address+blockAndIdx] = true
			}
			refundAddress := traces.Result[i].Action.RefundAddress
			if GoodAddr(refundAddress) {
				addresses[refundAddress+blockAndIdx] = true
			}

		} else if traces.Result[i].Type == "create" {
			// add the creator, and the new address name
			from := traces.Result[i].Action.From
			if GoodAddr(from) {
				addresses[from+blockAndIdx] = true
			}
			address := traces.Result[i].Result.Address
			if GoodAddr(address) {
				addresses[address+blockAndIdx] = true
			}

			// If it's a top level trace, then the call data is the init,
			// so to match with quickblocks, we just parse init
			if len(traces.Result[i].TraceAddress) == 0 {
				if len(traces.Result[i].Action.Init) > 10 {
					initData := traces.Result[i].Action.Init[10:]
					for i := 0; i < len(initData)/64; i++ {
						addr := string(initData[i*64 : (i+1)*64])
						if PotentialAddress(addr) {
							addr = "0x" + string(addr[24:])
							if GoodAddr(addr) {
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
						if GoodAddr(addr) {
							addresses[addr+blockAndIdx] = true
						}
					}
				}
			}

		} else {
			fmt.Println("New trace type:", traces.Result[i].Type)
			err := ""
			panic(err)
		}

		// Parse output of trace
		if len(traces.Result[i].Result.Output) > 2 {
			outputData := traces.Result[i].Result.Output[2:]
			for i := 0; i < len(outputData)/64; i++ {
				addr := string(outputData[i*64 : (i+1)*64])
				if PotentialAddress(addr) {
					addr = "0x" + string(addr[24:])
					if GoodAddr(addr) {
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
		idx := PadLeft(strconv.FormatInt(idxInt, 10), 5)

		blockAndIdx := "\t" + blockNum + "\t" + idx

		for j := 0; j < len(logs.Result[i].Topics); j++ {
			addr := string(logs.Result[i].Topics[j][2:])
			if PotentialAddress(addr) {
				addr = "0x" + string(addr[24:])
				if GoodAddr(addr) {
					addresses[addr+blockAndIdx] = true
				}
			}
		}

		if len(logs.Result[i].Data) > 2 {
			inputData := logs.Result[i].Data[2:]
			for i := 0; i < len(inputData)/64; i++ {
				addr := string(inputData[i*64 : (i+1)*64])
				if PotentialAddress(addr) {
					addr = "0x" + string(addr[24:])
					if GoodAddr(addr) {
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

	folderPath := "data" //+ string(blockNum[:3]) + "/" + string(blockNum[3:6])
	if _, err := os.Stat(folderPath); os.IsNotExist(err) {
		os.MkdirAll(folderPath, os.ModePerm)
	}

	fileName := folderPath + "/" + blockNum + ".txt"
	err := ioutil.WriteFile(fileName, toWrite, 0777)
	if err != nil {
		fmt.Println("Error writing file:", err)
	}
//	fmt.Print(blockNum, "\t", len(addresses), "     \r")
}

func extractAddresses(addressChannel chan BlockInternals, addressWG *sync.WaitGroup) {
	for blockTraceAndLog := range addressChannel {
		//fmt.Println("Beginning Block Processing...")
		// Set of 'address \t block \t txIdx'
		addresses := make(map[string]bool)

		// Parse the traces
		var traces BlockTraces
		err := json.Unmarshal(blockTraceAndLog.Traces, &traces)
		if err != nil {
			panic(err)
		}

		blockNum := ""
		if traces.Result != nil && len(traces.Result) > 0 {
			blockNum = PadLeft(strconv.Itoa(traces.Result[0].BlockNumber), 9)
			getTraceAddresses(addresses, &traces, blockNum)
		}

		// Now, parse log data
		var logs BlockLogs
		err = json.Unmarshal(blockTraceAndLog.Logs, &logs)
		if err != nil {
			panic(err)
		}
		if blockNum == "" && len(logs.Result) > 0 {
			blockNum = PadLeft(logs.Result[0].BlockNumber, 9)
		}
		if blockNum != "" {
			getLogAddresses(addresses, &logs, blockNum)
			writeAddresses(blockNum, addresses)
		}
	}
	addressWG.Done()
}

func processBlocks(startBlock int, numBlocks int, skip int, nBlockProcesses int, nAddressProcesses int) {

	blockChannel := make(chan int)
	addressChannel := make(chan BlockInternals)

	var blockWG sync.WaitGroup
	blockWG.Add(nBlockProcesses)
	for i := 0; i < nBlockProcesses; i++ {
		go extractInternals(blockChannel, addressChannel, &blockWG)
	}

	var addressWG sync.WaitGroup
	addressWG.Add(nAddressProcesses)
	for i := 0; i < nAddressProcesses; i++ {
		go extractAddresses(addressChannel, &addressWG)
	}

	for block := startBlock; block < startBlock+numBlocks; block = block + skip {
		blockChannel <- block
	}

	close(blockChannel)
	blockWG.Wait()

	close(addressChannel)
	addressWG.Wait()
}

func main() {
	start := 0
	n := 8000000
	skip := 20000
	bPs := 0
	aPs := 0
    fmt.Print("\t");
    for aPs = 1 ; aPs < 100 ; aPs = aPs + 2 {
        fmt.Print(aPs, "\t")
    }
    fmt.Print("\n");
	for bPs = 1; bPs < 50; bPs = bPs + 2 {
        fmt.Print(bPs, "\t")
		for aPs = 1; aPs < 100; aPs = aPs + 2 {
			startTime := time.Now()
			processBlocks(start, n, skip, bPs, aPs)
//			fmt.Println(start, " ", n, " ", skip, " ", ((n - start) / skip), " ", bPs, " ", aPs, " ", time.Since(startTime))
			fmt.Print(time.Since(startTime).Nanoseconds() / 1000., "\t")
		}
        fmt.Print("\n")
	}
	os.Exit(1)
}
