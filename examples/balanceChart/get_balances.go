package main

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/sdk"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/bykof/gostradamus"
)

// getBalances retrieves the balances for a list of addresses within a specified time range and period.
// It converts the names to string addresses, constructs block identifiers from start and end times,
// determines relevant block numbers, creates a block range string for the period, fetches the balances
// for the addresses within this range, and returns the results or an error if any occur.
func getBalances(names []types.Name, start, end gostradamus.DateTime, period string) ([]types.State, error) {
	fmt.Println("Getting balances for", len(names), "addresses")

	// Convert names to string addresses
	addrs := make([]string, len(names))
	for i, name := range names {
		addrs[i] = name.Address.Hex()
	}

	// Construct block identifiers from start and end times
	whenOpts := sdk.WhenOptions{
		BlockIds: []string{
			start.Format("YYYY-MM-DDTHH:mm:ss"),
			end.Format("YYYY-MM-DDTHH:mm:ss"),
		},
		Globals: sdk.Globals{
			Cache: true,
		},
	}
	blocks, _, err := whenOpts.When()
	if err != nil {
		return nil, err
	}

	// Create a block range string for the period
	blockRange := fmt.Sprintf("%d-%d:%s", blocks[0].BlockNumber, blocks[1].BlockNumber, period)

	// Fetch the balances for the addresses within this range
	stateOpts := sdk.StateOptions{
		Addrs:    addrs,
		BlockIds: []string{blockRange},
		Parts:    sdk.SPBalance,
		NoZero:   true,
		Globals: sdk.Globals{
			Ether: true,
			Cache: true,
		},
	}
	state, _, err := stateOpts.State()
	if err != nil {
		return nil, err
	}

	fmt.Println("Got", len(state), "states")
	return state, nil
}
