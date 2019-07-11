package cmd

import (
	"bytes"
//	"encoding/csv"
	"encoding/json"
	"fmt"
	"io/ioutil"
	"net/http"
	"os"
//	"path/filepath"
	"sort"
	"strconv"
	"strings"
	"sync"

	"github.com/spf13/cobra"
	"github.com/spf13/viper"
)

// Params Parameters used during calls to the RPC.
type Params []interface{}

// RPCPayload Data structure used during calls to the RPC.
type RPCPayload struct {
	Jsonrpc string `json:"jsonrpc"`
	Method  string `json:"method"`
	Params  `json:"params"`
	ID      int `json:"id"`
}

// Filter Used by the getLogs RPC call to identify the block range.
type Filter struct {
	Fromblock string `json:"fromBlock"`
	Toblock   string `json:"toBlock"`
}

// BlockTraces Returned value from the RPC containing all the traces for a given block.
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

// BlockLogs Returned value from the RPC containing all the logs for a given block.
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

// getTracesFromBlock Returns all traces for a given block.
func getTracesFromBlock(blockNum int) ([]byte, error) {
	payloadBytes, err := json.Marshal(RPCPayload{"2.0", "trace_block", Params{fmt.Sprintf("0x%x", blockNum)}, 2})
	if err != nil {
		return nil, err
	}

	body := bytes.NewReader(payloadBytes)
	req, err := http.NewRequest("POST", viper.GetString("settings.rpcProvider"), body)
	if err != nil {
		return nil, err
	}

	req.Header.Set("Content-Type", "application/json")
	resp, err := http.DefaultClient.Do(req)
	if err != nil {
		return nil, err
	}

	tracesBody, err := ioutil.ReadAll(resp.Body)
	if err != nil {
		return nil, err
	}

	defer resp.Body.Close()
	return tracesBody, nil
}

// getLogsFromBlock Returns all logs for a given block.
func getLogsFromBlock(blockNum int) ([]byte, error) {
	payloadBytes, err := json.Marshal(RPCPayload{"2.0", "eth_getLogs", Params{Filter{fmt.Sprintf("0x%x", blockNum), fmt.Sprintf("0x%x", blockNum)}}, 2})
	if err != nil {
		return nil, err
	}

	body := bytes.NewReader(payloadBytes)
	req, err := http.NewRequest("POST", viper.GetString("settings.rpcProvider"), body)
	if err != nil {
		return nil, err
	}

	req.Header.Set("Content-Type", "application/json")
	resp, err := http.DefaultClient.Do(req)
	if err != nil {
		return nil, err
	}

	logsBody, err := ioutil.ReadAll(resp.Body)
	if err != nil {
		return nil, err
	}

	defer resp.Body.Close()
	return logsBody, nil
}

// getTransactionReceipt Returns recipt for a given transaction -- only used in errored contract creations
func getTransactionReceipt(hash string) ([]byte, error) {
	payloadBytes, err := json.Marshal(RPCPayload{"2.0", "eth_getTransactionReceipt", Params{hash}, 2})
	if err != nil {
		return nil, err
	}

	body := bytes.NewReader(payloadBytes)
	req, err := http.NewRequest("POST", viper.GetString("settings.rpcProvider"), body)
	if err != nil {
		return nil, err
	}

	req.Header.Set("Content-Type", "application/json")
	resp, err := http.DefaultClient.Do(req)
	if err != nil {
		return nil, err
	}

	receiptBody, err := ioutil.ReadAll(resp.Body)
	if err != nil {
		return nil, err
	}

	defer resp.Body.Close()
	return receiptBody, nil
}

// getTracesAndLogs Process the block channel and for each block query the node for both traces and logs. Send results to addressChannel
func getTracesAndLogs(blockChannel chan int, addressChannel chan BlockInternals, blockWG *sync.WaitGroup) {

	for blockNum := range blockChannel {
		traces, err := getTracesFromBlock(blockNum)
		if err != nil {
			panic(err)
		}
		logs, err := getLogsFromBlock(blockNum)
		if err != nil {
			panic(err)
		}
		addressChannel <- BlockInternals{traces, logs}
	}
	blockWG.Done()
}

func extractAddresses(addressChannel chan BlockInternals, addressWG *sync.WaitGroup) {

	for blockTraceAndLog := range addressChannel {
		addressMap := make(map[string]bool)

		// Parse the traces
		var traces BlockTraces
		err := json.Unmarshal(blockTraceAndLog.Traces, &traces)
		if err != nil {
			panic(err)
		}
		blockNum := ""
		if traces.Result != nil && len(traces.Result) > 0 {
			blockNum = padLeft(strconv.Itoa(traces.Result[0].BlockNumber), 9)
			extractAddressesFromTraces(addressMap, &traces, blockNum)
		}

		// Now, parse log data
		var logs BlockLogs
		err = json.Unmarshal(blockTraceAndLog.Logs, &logs)
		if err != nil {
			panic(err)
		}
		if blockNum == "" && len(logs.Result) > 0 {
			blockNum = padLeft(logs.Result[0].BlockNumber, 9)
		}
		if blockNum != "" {
			extractAddressesFromLogs(addressMap, &logs, blockNum)
			writeAddresses(blockNum, addressMap)
		}
	}
	addressWG.Done()
}

func extractAddressesFromTraces(addressMap map[string]bool, traces *BlockTraces, blockNum string) {

	for i := 0; i < len(traces.Result); i++ {

		idx := padLeft(strconv.Itoa(traces.Result[i].TransactionPosition), 5)
		blockAndIdx := "\t" + blockNum + "\t" + idx

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
				if goodAddr(author) {
					addressMap[author+"\t"+blockNum+"\t"+"99999"] = true
				}

			} else if traces.Result[i].Action.RewardType == "uncle" {
				author := traces.Result[i].Action.Author
				if goodAddr(author) {
					addressMap[author+"\t"+blockNum+"\t"+"99998"] = true
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
			// so to match with quickblocks, we just parse init
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
						if goodAddr(addr) {
							addressMap[addr+blockAndIdx] = true
						}
					}
				}
			}

		} else {
			fmt.Println("New trace type:", traces.Result[i].Type)
			err := ""
			panic(err)
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

// extractAddressesFromLogs Extracts addresses from any part of the log data.
func extractAddressesFromLogs(addressMap map[string]bool, logs *BlockLogs, blockNum string) {

	for i := 0; i < len(logs.Result); i++ {
		idxInt, err := strconv.ParseInt(logs.Result[i].TransactionIndex, 0, 32)
		if err != nil {
			fmt.Println("Error:", err)
		}
		idx := padLeft(strconv.FormatInt(idxInt, 10), 5)

		blockAndIdx := "\t" + blockNum + "\t" + idx

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

var counter = 0

func writeAddresses(blockNum string, addressMap map[string]bool) {

	addressArray := make([]string, len(addressMap))
	idx := 0
	for address := range addressMap {
		addressArray[idx] = address
		idx++
	}
	sort.Strings(addressArray)
	toWrite := []byte(strings.Join(addressArray[:], "\n") + "\n")

	folderPath := viper.GetString("settings.cachePath") + "addr_index/raw/"
	if _, err := os.Stat(folderPath); os.IsNotExist(err) {
		os.MkdirAll(folderPath, os.ModePerm)
	}

	fileName := folderPath + blockNum + ".txt"
	err := ioutil.WriteFile(fileName, toWrite, 0777)
	if err != nil {
		fmt.Println("Error writing file:", err)
	}
	fmt.Fprint(os.Stderr, "\t\t\t\t", blockNum, " ", len(addressMap), " - ", counter, "    \r") // \r
	counter = counter + 1
}

func processBlocks(startBlock int, numBlocks int, skip int, nBlockProcesses int, nAddressProcesses int) {

	blockChannel := make(chan int)
	addressChannel := make(chan BlockInternals)

	var blockWG sync.WaitGroup
	blockWG.Add(nBlockProcesses)
	for i := 0; i < nBlockProcesses; i++ {
		go getTracesAndLogs(blockChannel, addressChannel, &blockWG)
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

/*
func getAllSightings(sightings chan AddrSighting) {
	for sighting := range sightings {
		fmt.Println(sighting)
	}
}

func searchForAddress(address string, fileNames chan string, sightings chan AddrSighting) {
	for fileName := range fileNames {
		//fmt.Println(fileName)

		f, err := os.Open(fileName)
		if err != nil {
			fmt.Println("ERROR:", err)
		}

		lines, err := csv.NewReader(f).ReadAll()
		if err != nil {
			fmt.Println("ERROR:", err)
		}
		f.Close()

		// Binary search for addresses
		i := sort.Search(len(lines), func(i int) bool { return lines[i][0] >= address })
		if i < len(lines) && lines[i][0] == address {
			// found the address
			block, err := strconv.Atoi(lines[i][1])
			txIdx, err := strconv.Atoi(lines[i][2])
			if err != nil {
				fmt.Println("ERROR:", err)
			}

			// TODO: note that we didn't find every block it was included in
			sightings <- AddrSighting{block, txIdx}
		}
	}

	for i := 0; i < 10; i++ {
		go searchForAddress("0xc8883059be00EC5F708398369B857A7487130317", fileNames, sightings)
	}

	go getAllSightings(sightings)

	root := "/home/jrush/goblocks/blocks"
	err := filepath.Walk(root, func(path string, info os.FileInfo, err error) error {
		if strings.HasSuffix(path, ".txt") {
			fileNames <- path
		}
		return nil
	})
	if err != nil {
		panic(err)
	}
}
*/

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

// goodAddr Returns true if the address is not a precompile and not zero
func goodAddr(addr string) bool {
	// As per EIP 1352, all addresses less than the following value are reserved
	// for pre-compiles. We don't index precompiles.
	if addr < "0x000000000000000000000000000000000000ffff" {
		return false
	}
	return true
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

var scrapeCmd = &cobra.Command{
	Use:   "scrape",
	Short: "Freshen the index to the front of the chain",
	Long: `
Description:

  The 'scrape' subcommand freshens the TrueBlocks index, picking up where it last
  left off. 'Scrape' visits every block, queries that block's traces and logs
  looking for addresses, and writes an index of those addresses per transaction.`,
	Run: func(cmd *cobra.Command, args []string) {
		start := viper.GetInt("startBlock")
		if start < 0 {
			start = 1
		}
		n := viper.GetInt("nBlocks")
		skip := 1
		nBlockProcesses := viper.GetInt("nBlockProcesses")
		nAddrProcesses := viper.GetInt("nAddrProcesses")
		//		fmt.Println("rpcProvider: ", viper.GetString("settings.rpcProvider"))
		//		fmt.Println("cachePath: ", viper.GetString("settings.cachePath"))
		//		fmt.Println("startBlock: ", start)
		//		fmt.Println("nBlocks: ", n)
		//      fmt.Println("nBlockProcesses: ", nBlockProcesses)
		//      fmt.Println("nAddrProcesses: ", nAddrProcesses)
		processBlocks(start, n, skip, nBlockProcesses, nAddrProcesses)
	},
}

var maxBlocks int

func init() {
	rootCmd.AddCommand(scrapeCmd)
	scrapeCmd.PersistentFlags().IntVarP(&maxBlocks, "maxBlocks", "m", 0, "The maximum number of blocks to scrape (default is to catch up).")
}
