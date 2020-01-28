package cmd

import (
	"encoding/json"
	"fmt"
	"os"
	"sync"

	"github.com/spf13/cobra"
)

type scanOptionsT struct {
	columns string
}

func processBlock(blockChannel chan int, blockWG *sync.WaitGroup) {

	for blockNum := range blockChannel {
		fmt.Printf("%d\r", blockNum)
		blockBytes, err := getBlockHeader(blockNum)
		if err != nil {
			fmt.Println(err)
			os.Exit(1)
		}
		var block BlockHeader
		err = json.Unmarshal(blockBytes, &block)
		if err != nil {
			fmt.Println(err)
			os.Exit(1)
		}
		fmt.Printf("%d-%s\r", blockNum, block.Result.Number)
	}
	blockWG.Done()
}

func scanBlocks() {

	blockChannel := make(chan int)

	var blockWG sync.WaitGroup
	blockWG.Add(Options.nBlockProcs)
	for i := 0; i < Options.nBlockProcs; i++ {
		go processBlock(blockChannel, &blockWG)
	}

	for block := Options.startBlock; block < Options.startBlock+Options.nBlocks; block++ {
		blockChannel <- block
	}

	close(blockChannel)
	blockWG.Wait()
}

// scanOptions carries local command line options related to the scan command
var scanOptions scanOptionsT

var scanCmd = &cobra.Command{
	Use:   "scan",
	Short: "Scan the chain for various parts of the data and export",
	Long: `
Description:
  The 'scan' subcommand traverses the Ethereum data structures extracting whatever
  portion you want. This code requires you to modify it to extract your exact
  data, so you have to edit this file to customize it.`,
	Run: func(cmd *cobra.Command, args []string) {
		Options.rpcProvider = "http://localhost:8545"
		fmt.Printf("\t  options:\t %d/%d/%d/%d\n", Options.startBlock, Options.nBlocks, Options.ripeBlock, (Options.startBlock + Options.nBlocks))
		fmt.Printf("\t  processes:\t %d/%d\n", Options.nBlockProcs, Options.nAddrProcs)
		fmt.Printf("\t  rpcProvider:\t %s\n", Options.rpcProvider)
		fmt.Printf("\t  indexPath:\t %s\n", Options.indexPath)
		fmt.Printf("\t  ripePath:\t %s\n", Options.ripePath)
		fmt.Printf("\t  unripePath:\t %s\n", Options.unripePath)
		fmt.Printf("\t  columns:\t %s\n", scanOptions.columns)
		fmt.Printf("\t  scraping:\n")
		scanBlocks()
		fmt.Println("")
	},
}

func init() {
	scanCmd.PersistentFlags().StringVarP(&scanOptions.columns, "columns", "l", "block.timestamp", "retrieve the timestamp of the block's data")
	rootCmd.AddCommand(scanCmd)
}

//https://medium.com/@matryer/golang-advent-calendar-day-eleven-persisting-go-objects-to-disk-7caf1ee3d11d
