package main

import (
	"fmt"
	"log"
	"math/big"
	"strconv"
	"sync"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/TrueBlocks/trueblocks-core/v0/sdk"
	"github.com/bykof/gostradamus"
	"gonum.org/v1/plot"
	"gonum.org/v1/plot/plotter"
	"gonum.org/v1/plot/vg"
)

func createChart(data []types.SimpleState, filename string) {
	points := make(plotter.XYs, len(data))
	for i, d := range data {
		points[i].X = float64(d.BlockNumber)
		points[i].Y, _ = strconv.ParseFloat(utils.FormattedValue(d.Balance, true, 18), 64)
	}

	// Create a new plot
	p, err := plot.New()
	if err != nil {
		log.Fatalf("Error creating plot: %v", err)
	}

	p.Title.Text = "Block Number vs. Balance"
	p.X.Label.Text = "Block Number"
	p.Y.Label.Text = "Balance"

	// Add the line plot to the plot
	line, err := plotter.NewLine(points)
	if err != nil {
		log.Fatalf("Error creating line plot: %v", err)
	}
	p.Add(line)

	// Save the plot to an SVG file
	if err := p.Save(8*vg.Inch, 8*vg.Inch, filename); err != nil {
		log.Fatalf("Error saving plot: %v", err)
	}
}

var dateFmt = "YYYY-MM-DDTHH:mm:ss"
var m sync.Mutex

func getBalances(addrs []string, start, end gostradamus.DateTime, freq string) (data []types.SimpleState) {
	whenOpts := sdk.WhenOptions{
		BlockIds: []string{start.Format(dateFmt), end.Format(dateFmt)},
		Globals:  sdk.Globals{Cache: true},
	}

	// First, we get the block numbers from the dates
	var err error
	var blocks []types.SimpleNamedBlock
	blocks, _, err = whenOpts.Query()
	if err != nil {
		logger.Fatal(err)
	}

	namesOpts := sdk.NamesOptions{
		Terms:  addrs,
		Custom: true,
	}

	if names, _, _ := namesOpts.Query(); len(names) == 0 {
		logger.Fatal(fmt.Errorf("no names found"))
	} else {
		ch := make(chan sdk.StateSdk)
		var wg sync.WaitGroup
		wg.Add(len(names))
		for _, name := range names {
			go func(n types.SimpleName) {
				defer wg.Done()
				stateOpts := sdk.StateOptions{
					Addrs:    []string{name.Address.Hex()},
					BlockIds: []string{fmt.Sprintf("%d-%d:%s", blocks[0].BlockNumber, blocks[1].BlockNumber, freq)},
					Parts:    sdk.SPBalance, // | sdk.SPNonce | sdk.SPCode,
					Globals: sdk.Globals{
						Ether: true,
						Cache: true,
					},
				}
				m.Lock()
				if state, _, err := stateOpts.Query(); err != nil {
					logger.Fatal(err)
				} else if len(state) == 0 {
					logger.Fatal(fmt.Errorf("weird outcome"))
				} else {
					m.Unlock()
					zero := new(big.Int)
					for _, st := range state {
						if st.Balance.Int.Cmp(zero) != 0 {
							st.Address.Address = name.Address
							ch <- st
						}
					}
				}
			}(name)
		}

		go func() {
			wg.Wait()
			close(ch)
		}()

		for state := range ch {
			ethStr := utils.FormattedValue(state.Balance.Int, true, 18)
			data = append(data, types.SimpleState{
				BlockNumber: state.BlockNumber,
				Timestamp:   state.Timestamp,
				Balance:     state.Balance.Int,
			})
			fmt.Println(state.Address.Address.Hex(), state.BlockNumber, ethStr)
		}
	}
	return
}

func main() {
	addrs := []string{"trueblocks.eth"}
	startDate := gostradamus.NewDateTime(2015, 8, 1, 0, 0, 0, 0, gostradamus.UTC)
	endDate := gostradamus.NewDateTime(2023, 12, 31, 23, 59, 59, 0, gostradamus.UTC)

	data := getBalances(addrs, startDate, endDate, "hourly")
	createChart(data, "plot.svg")
	utils.System("open plot.svg")
}
