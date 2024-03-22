package main

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/v0/sdk"
	"github.com/bykof/gostradamus"

	"github.com/TrueBlocks/balanceChart/charts"
)

func main() {
	names := []types.SimpleName{
		{Name: "bankless.eth"},
		{Name: "trueblocks.eth"},
		{Name: "giveth.eth"},
		{Name: "rotki.eth"},
	}

	chains := []string{
		"mainnet",
		// "optimism",
		// "gnosis",
		"sepolia",
	}

	for _, chain := range chains {
		fmt.Println("Getting balances for", chain)
		data := getBalances(names, chain, NewDate(2022, 1, 1), NewDate(2023, 12, 31), "monthly")
		charts.GroupedBarChart(data, "Monthly Balances on ["+chain+"]", chain+"bar.png")
	}
}
func getBalances(names []types.SimpleName, chain string, start, end gostradamus.DateTime, freq string) []types.SimpleState {
	whenOpts := sdk.WhenOptions{
		BlockIds: []string{start.Format("YYYY-MM-DDTHH:mm:ss"), end.Format("YYYY-MM-DDTHH:mm:ss")},
		Globals: sdk.Globals{
			Cache: true,
			Chain: chain,
		},
	}

	if blocks, _, err := whenOpts.Query(); err != nil {
		logger.Error(err)
		return []types.SimpleState{}
	} else {
		addrs := make([]string, len(names))
		for i, name := range names {
			conn := rpc.TempConnection("mainnet")
			addrs[i], _ = conn.GetEnsAddress(name.Name)
		}

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
				Chain: chain,
			},
		}

		if state, meta, err := stateOpts.Query(); err != nil {
			logger.Error(err)
			return []types.SimpleState{}
		} else {
			fmt.Println("Got", len(state), "states to block", meta.Latest)
			return state
		}
	}
}

func NewDate(year, month, day int) gostradamus.DateTime {
	return gostradamus.NewDateTime(year, month, day, 0, 0, 0, 0, "UTC")
}
