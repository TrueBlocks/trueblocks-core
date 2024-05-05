package main

import (
	"fmt"
	"os"
	"sync"
	"time"

	"github.com/TrueBlocks/trueblocks-core/sdk"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

func main() {
	// We test how fast the processing is for each of 1-10:1, 10-100-10, 200-1000:100 workers
	workers := []int{
		1, 2, 3, 4, 5, 6, 7, 8, 9,
		10, 20, 30, 40, 50, 60, 70, 80, 90,
		100, 200, // 300, 400, 500, 600, 700, 800, 900, 1000,
	}

	if len(os.Args) > 1 {
		workers = []int{}
		for _, arg := range os.Args[1:] {
			v := int(utils.MustParseInt(arg))
			if v > 0 {
				workers = append(workers, v)
			}
		}
	}

	for numWorkers := len(workers) - 1; numWorkers >= 0; numWorkers-- {
		oneTest(workers[numWorkers])
	}
}

var first int = 47500
var last int = 45000

func oneTest(numWorkers int) {
	start := time.Now()
	defer func() {
		elapsed := time.Since(start)
		hundredths := float64(elapsed.Microseconds()) / 10000.0
		s := "| %d | %d | %.2f | %.2f |\n"
		fmt.Printf(s, numWorkers, first-last, hundredths, float64(first-last)/hundredths*100)
	}()

	var wg sync.WaitGroup
	blknumChan := make(chan base.Blknum)
	minBlockNumber := base.NOPOSN2
	var mu sync.Mutex

	// Start multiple worker goroutines
	wg.Add(numWorkers)
	for i := 0; i < numWorkers; i++ {
		// The workers wait until we put things in the channel
		go worker(blknumChan, &wg, &mu, &minBlockNumber)
	}

	// Send blknums into the channel
	go func() {
		for i := first; i >= last; i-- {
			blknumChan <- base.Blknum(i)
			if i%10 == 0 {
				file.StringToAsciiFile(
					"/Users/jrush/Development/trueblocks-core/build/shit",
					fmt.Sprintf("Workers: %d, Bn: %d, Cnt: %d", numWorkers, i, minBlockNumber))
			}
		}
		close(blknumChan)
	}()

	// Wait until all the block numbers are processed
	wg.Wait()

	if minBlockNumber != base.NOPOSN2 {
		fmt.Printf("Smallest BlockNumber with TransactionCount > 0 is: %d\n", minBlockNumber)
	} else {
		fmt.Println("No blocks with TransactionCount > 0 found.")
	}
}

// worker listens on a channel for blknums and processes each block by looking for
// the smallest block with transactions.
func worker(blknumChan <-chan base.Blknum, wg *sync.WaitGroup, mu *sync.Mutex, minBlockNumber *base.Blknum) {
	defer wg.Done()
	for blknum := range blknumChan {
		opts := sdk.BlocksOptions{
			BlockIds: []string{fmt.Sprintf("%d", blknum)},
		}

		blocks, _, err := opts.BlocksCount()
		if err != nil {
			fmt.Println(err)
			return
		}

		for _, block := range blocks {
			if block.TransactionsCnt > 0 {
				mu.Lock()
				if block.BlockNumber < *minBlockNumber {
					*minBlockNumber = block.BlockNumber
				}
				mu.Unlock()
			}
		}
	}
}
