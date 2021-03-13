package cmd

//----------------------------------------------------------------------------
import (
	"encoding/json"
	"fmt"
	"io/ioutil"
	"os"

	"sort"
	"strconv"
	"strings"
	"sync"

	"github.com/spf13/cobra"
)

// tracesAndLogs combines Traces and Logs to make processing easier
type tracesAndLogs struct {
	Traces []byte
	Logs   []byte
}

// getTracesAndLogs Process the block channel and for each block query the node for both traces and logs. Send results to addressChannel
func getTracesAndLogs(blockChannel chan int, addressChannel chan tracesAndLogs, blockWG *sync.WaitGroup) {

	for blockNum := range blockChannel {
		traces, err := getTracesFromBlock(blockNum)
		if err != nil {
			fmt.Println(err)
			os.Exit(1) // caller will start over if this process exits with non-zero value
		}
		logs, err := getLogsFromBlock(blockNum)
		if err != nil {
			fmt.Println(err)
			os.Exit(1) // caller will start over if this process exits with non-zero value
		}
		addressChannel <- tracesAndLogs{traces, logs}
	}
	blockWG.Done()
}

func extractAddresses(addressChannel chan tracesAndLogs, addressWG *sync.WaitGroup) {

	for blockTraceAndLog := range addressChannel {
		addressMap := make(map[string]bool)

		// Parse the traces
		var traces Trace
		err := json.Unmarshal(blockTraceAndLog.Traces, &traces)
		if err != nil {
			fmt.Println(err)
			os.Exit(1) // caller will start over if this process exits with non-zero value
		}
		blockNum := ""
		if traces.Result != nil && len(traces.Result) > 0 {
			blockNum = padLeft(strconv.Itoa(traces.Result[0].BlockNumber), 9)
			extractAddressesFromTraces(addressMap, &traces, blockNum)
		}

		// Now, parse log data
		var logs Log
		err = json.Unmarshal(blockTraceAndLog.Logs, &logs)
		if err != nil {
			fmt.Println(err)
			os.Exit(1) // caller will start over if this process exits with non-zero value
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

func extractAddressesFromTraces(addressMap map[string]bool, traces *Trace, blockNum string) {

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
				if author == "0x0" || author == "0x0000000000000000000000000000000000000000" {
					// Early clients allowed misconfigured miner settings with address
					// 0x0 (reward got burned). We enter a false record with a false tx_id
					// to account for this.
					author = "0xdeaddeaddeaddeaddeaddeaddeaddeaddeaddead"
					addressMap[author+"\t"+blockNum+"\t"+"99997"] = true

				} else {
					if goodAddr(author) {
						addressMap[author+"\t"+blockNum+"\t"+"99999"] = true
					}
				}

			} else if traces.Result[i].Action.RewardType == "uncle" {
				author := traces.Result[i].Action.Author
				if author == "0x0" || author == "0x0000000000000000000000000000000000000000" {
					// Early clients allowed misconfigured miner settings with address
					// 0x0 (reward got burned). We enter a false record with a false tx_id
					// to account for this.
					author = "0xdeaddeaddeaddeaddeaddeaddeaddeaddeaddead"
					addressMap[author+"\t"+blockNum+"\t"+"99998"] = true

				} else {
					if goodAddr(author) {
						addressMap[author+"\t"+blockNum+"\t"+"99998"] = true
					}
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
							fmt.Println(err)
							os.Exit(1) // caller will start over if this process exits with non-zero value
						}
						var receipt Receipt
						err = json.Unmarshal(bytes, &receipt)
						if err != nil {
							fmt.Println(err)
							os.Exit(1) // caller will start over if this process exits with non-zero value
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
			fmt.Println(err)
			os.Exit(1) // caller will start over if this process exits with non-zero value
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
func extractAddressesFromLogs(addressMap map[string]bool, logs *Log, blockNum string) {

	for i := 0; i < len(logs.Result); i++ {
		idxInt, err := strconv.ParseInt(logs.Result[i].TransactionIndex, 0, 32)
		if err != nil {
			fmt.Println(err)
			os.Exit(1) // caller will start over if this process exits with non-zero value
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

	bn, _ := strconv.Atoi(blockNum)
	// This code (disabled) tried to extract timestamp while we're scraping. It didn't work, so
	// commented out. We were trying to attach timestamp to the ripe block's filename
	//blockHeaderBytes, err := getBlockHeader(bn)
	//if err != nil {
	//	fmt.Println(err)
	//	os.Exit(1) // caller will start over if this process exits with non-zero value
	//}
	//
	//var header BlockHeader
	//err = json.Unmarshal(blockHeaderBytes, &header)
	//if err != nil {
	//	fmt.Println(err)
	//	os.Exit(1) // caller will start over if this process exits with non-zero value
	//}
	//
	//fileName := Options.ripePath + blockNum + "_ts" + header.Result.Timestamp + ".txt"
	fileName := Options.ripePath + blockNum + ".txt"
	if bn > Options.ripeBlock {
		fileName = Options.unripePath + blockNum + ".txt"
	}

	err := ioutil.WriteFile(fileName, toWrite, 0744)
	if err != nil {
		fmt.Println(err)
		os.Exit(1) // caller will start over if this process exits with non-zero value
	}
	// Show fifty dots no matter how many blocks we're scraping
	skip := Options.nBlocks / 100
	if skip < 1 {
		skip = 1
	}
	counter++
	if counter%skip == 0 {
		fmt.Fprint(os.Stderr, ".")
	}
}

func scrapeBlocks() {

	blockChannel := make(chan int)
	addressChannel := make(chan tracesAndLogs)

	var blockWG sync.WaitGroup
	blockWG.Add(Options.nBlockProcs)
	for i := 0; i < Options.nBlockProcs; i++ {
		go getTracesAndLogs(blockChannel, addressChannel, &blockWG)
	}

	var addressWG sync.WaitGroup
	addressWG.Add(Options.nAddrProcs)
	for i := 0; i < Options.nAddrProcs; i++ {
		go extractAddresses(addressChannel, &addressWG)
	}

	for block := Options.startBlock; block < Options.startBlock+Options.nBlocks; block++ {
		blockChannel <- block
	}

	close(blockChannel)
	blockWG.Wait()

	close(addressChannel)
	addressWG.Wait()
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

type scrapeOptionsT struct {
	columns2 string
}

// scrapeOptions carries local command line options related to the scrape command
var scrapeOptions scrapeOptionsT

var scrapeCmd = &cobra.Command{
	Use:   "scrape",
	Short: "Freshen the index to the front of the chain",
	Long: `
Description:
  The 'scrape' subcommand freshens the TrueBlocks index, picking up where it last
  left off. 'Scrape' visits every block, queries that block's traces and logs
  looking for addresses, and writes an index of those addresses per transaction.`,
	Run: func(cmd *cobra.Command, args []string) {
		//fmt.Printf("\t  options:\t %d/%d/%d/%d\n", Options.startBlock, Options.nBlocks, Options.ripeBlock, (Options.startBlock + Options.nBlocks))
		//fmt.Printf("\t  processes:\t %d/%d\n", Options.nBlockProcs, Options.nAddrProcs)
		//fmt.Printf("\t  rpcProvider:\t %s\n", Options.rpcProvider)
		//fmt.Printf("\t  indexPath:\t %s\n", Options.indexPath)
		//fmt.Printf("\t  ripePath:\t %s\n", Options.ripePath)
		//fmt.Printf("\t  unripePath:\t %s\n", Options.unripePath)
		//fmt.Printf("\t  columns2:\t%s\n", scrapeOptions.columns2)
		//fmt.Printf("\t  scraping:")
		scrapeBlocks()
		//fmt.Println("")
	},
}

func init() {
	scrapeCmd.PersistentFlags().StringVarP(&scrapeOptions.columns2, "columns", "l", "block.timestamp", "retrieve the timestamp of the block's data")
	rootCmd.AddCommand(scrapeCmd)
}
