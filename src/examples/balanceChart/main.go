package main

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/TrueBlocks/trueblocks-core/v0/sdk"
	"github.com/bykof/gostradamus"

	"github.com/TrueBlocks/balanceChart/charts"
)

var dateFmt = "YYYY-MM-DDTHH:mm:ss"

func getBalances(addrs []string, start, end gostradamus.DateTime, freq string) []types.SimpleState {
	// First, we get the block numbers from the dates
	whenOpts := sdk.WhenOptions{
		BlockIds: []string{start.Format(dateFmt), end.Format(dateFmt)},
		Globals:  sdk.Globals{Cache: true},
	}

	if blocks, _, err := whenOpts.Query(); err != nil {
		logger.Error(err)
		return []types.SimpleState{}
	} else {
		fmt.Println("Getting balances for", len(addrs), "addresses at", len(blocks), "blocks...")

		blockRange := fmt.Sprintf("%d-%d:%s", blocks[0].BlockNumber, blocks[1].BlockNumber, freq)
		stateOpts := sdk.StateOptions{
			Addrs:    addrs,
			BlockIds: []string{blockRange},
			Parts:    sdk.SPBalance, // | sdk.SPNonce | sdk.SPCode,
			NoZero:   true,
			Globals: sdk.Globals{
				Ether: true,
				Cache: true,
			},
		}

		if state, _, err := stateOpts.Query(); err != nil {
			logger.Error(err)
			return []types.SimpleState{}
		} else {
			return state
		}
	}
}

func main() {
	addrs := []string{"meriam.eth", "trueblocks.eth", "giveth.eth", "rotki.eth"}
	startDate := gostradamus.NewDateTime(2015, 8, 2, 0, 0, 0, 0, gostradamus.UTC)
	endDate := gostradamus.NewDateTime(2023, 12, 31, 23, 59, 59, 0, gostradamus.UTC)

	fmt.Println("Getting balances...")
	data := getBalances(addrs, startDate, endDate, "monthly")

	// charts.LineChart(data, "Account Balance over Time", "line.svg")
	// utils.System("open line.svg")

	charts.GroupedBarChart(data, "Account Balance over Time", "bar.svg")
	utils.System("open bar.svg")
}
