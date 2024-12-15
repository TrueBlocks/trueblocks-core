package main

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/examples/balanceChart/pkg/charts"
	"github.com/TrueBlocks/trueblocks-core/examples/balanceChart/pkg/dates"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

// main retrieves a list of prefund addresses with balances at the start of
// the blockchain, gets their balances at the end of each specified period
// (month, quarter, year), and plots the results.
func main() {
	n, maxAmt, period := getOptions()

	addresses, err := getPrefunds(n, maxAmt)
	if err != nil {
		logger.Fatal(err)
	}

	startDate := dates.NewDate(2015, 8, 1, 15, 26, 16)
	endDate := dates.NewDate(2023, 12, 31, 23, 59, 59)
	balances, err := getBalances(addresses, startDate, endDate, period)
	if err != nil {
		logger.Fatal(err)
	}

	f := "Prefund Balances over Time\n(<= %d ETH, %s, %d addresses)"
	title := fmt.Sprintf(f, int(maxAmt), period, n)
	charts.BarChart(addresses, balances, charts.Labels{
		Title: title,
		X:     "BlockNo.",
		Y:     "Ether",
	}, "ether_balances.png")
}
