package main

import (
	"fmt"
	"sort"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/examples/balanceChart/charts"
	"github.com/TrueBlocks/trueblocks-core/sdk"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/bykof/gostradamus"
)

func main() {
	// logger.SetLoggerWriter(os.Stderr)
	namesOpts := sdk.NamesOptions{
		Terms:   []string{""},
		Prefund: true,
		Expand:  true,
	}

	names, _, err := namesOpts.Names()
	if err != nil {
		logger.Fatal(err)
	}
	sort.Slice(names, func(i, j int) bool {
		f1 := names[i].Prefund
		f2 := names[j].Prefund
		if f1.Cmp(&f2) == 0 {
			return names[i].Name < names[j].Name
		}
		return f1.Cmp(&f2) > 0
	})

	filtered := make([]types.Name, 0, len(names))
	for i, name := range names {
		names[i].Name = name.Address.Hex()
		if len(filtered) < 25 && base.ToEther(&name.Prefund).Cmp(base.NewEther(30000)) < 0 {
			filtered = append(filtered, name)
		}
	}
	names = filtered

	// names := []types.Name{
	// 	{Name: "trueblocks.eth"},
	// 	{Name: "rotki.eth"},
	// 	{Name: "scopelift.eth"},
	// 	{Name: "giveth.eth"},
	// 	{Name: "griff.eth"},
	// 	{Name: "dannyryan.eth"},
	// }

	chains := []string{
		"mainnet",
		// "optimism",
		// "gnosis",
		// "sepolia",
	}

	for _, chain := range chains {
		fmt.Println("Getting balances for", chain)
		data, _ := getBalances(names, chain, NewDate(2015, 1, 1), NewDate(2023, 12, 31), "annually")
		charts.GroupedBarChart(data, names, "Monthly Balances on ["+chain+"]", chain+"bar.png")
	}
}

func getBalances(names []types.Name, chain string, start, end gostradamus.DateTime, freq string) ([]types.State, error) {
	clamp := sdk.WhenOptions{
		BlockIds: []string{"0", "latest"},
		Globals: sdk.Globals{
			Cache: true,
			Chain: chain,
		},
	}
	clamps, _, _ := clamp.When()
	if start.UnixTimestamp() < clamps[0].Timestamp {
		start = gostradamus.FromUnixTimestamp(clamps[0].Timestamp)
	}
	if end.UnixTimestamp() > clamps[1].Timestamp {
		end = gostradamus.FromUnixTimestamp(clamps[1].Timestamp - 1)
	}

	whenOpts := sdk.WhenOptions{
		BlockIds: []string{start.Format("YYYY-MM-DDTHH:mm:ss"), end.Format("YYYY-MM-DDTHH:mm:ss"), "0", "latest"},
		Globals: sdk.Globals{
			Cache: true,
			Chain: chain,
		},
	}

	if blocks, _, err := whenOpts.When(); err != nil {
		logger.Error(err)
		return []types.State{}, err
	} else {
		addrs := make([]string, 0, len(names))
		for i := 0; i < len(names); i++ {
			if strings.Contains(names[i].Name, ".eth") {
				conn := rpc.TempConnection("mainnet")
				val, _ := conn.GetEnsAddress(names[i].Name)
				names[i].Address = base.HexToAddress(val)
			} else if names[i].Address.IsZero() && strings.HasPrefix(names[i].Name, "0x") {
				names[i].Address = base.HexToAddress(names[i].Name)
			}
			v := names[i].Address.Hex()
			addrs = append(addrs, v)
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

		if state, meta, err := stateOpts.State(); err != nil {
			logger.Error(err)
			return []types.State{}, err
		} else {
			fmt.Println("Got", len(state), "states to block", meta.Latest)
			return state, nil
		}
	}
}

func NewDate(year, month, day int) gostradamus.DateTime {
	return gostradamus.NewDateTime(year, month, day, 0, 0, 0, 0, "UTC")
}
