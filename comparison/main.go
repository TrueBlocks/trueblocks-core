package main

import (
	"fmt"
	"os"
	"time"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	sdk "github.com/TrueBlocks/trueblocks-sdk/v3"
)

func main() {
	address := "trueBlocks.eth"
	if len(os.Args) > 1 {
		address = os.Args[1]
	}

	lOpts := sdk.ListOptions{
		Addrs: []string{address},
	}
	now := time.Now()
	// On the command line you would do: chifra list <address>
	if unchainedAppearances, _, err := lOpts.List(); err != nil {
		fmt.Println("Error from Unchained Index:", err)
		return
	} else {
		fmt.Println("Time to fetch unchained index:", time.Since(now))
		now = time.Now()
		sOpts := sdk.SlurpOptions{
			Addrs: []string{address},
			Parts: sdk.SLPAll,
			Globals: sdk.Globals{
				Cache: true,
			},
		}
		// On the command line you would do: chifra slurp --appearances <address>
		if etherscanAppearances, _, err := sOpts.SlurpAppearances(); err != nil {
			fmt.Println("From Etherscan:", err)
			return
		} else {
			fmt.Println("Time to fetch from etherscan:", time.Since(now))
			compareAppearances(unchainedAppearances, etherscanAppearances)
		}
	}
}

func compareAppearances(unchained, etherscan []types.Appearance) {
	unchainedMap := make(map[types.Appearance]bool)
	etherscanMap := make(map[types.Appearance]bool)

	// min := uint32(0)
	for _, u := range unchained {
		// min = base.Max(min, u.BlockNumber)
		if u.BlockNumber > 19000000 {
			unchainedMap[u] = true
		}
	}
	for _, e := range etherscan {
		// min = base.Min(min, e.BlockNumber)
		if e.BlockNumber > 19000000 {
			etherscanMap[e] = true
		}
	}
	fmt.Println("In Unchained but not in Etherscan:")
	for u := range unchainedMap {
		if !etherscanMap[u] {
			fmt.Println(u)
		}
	}
	fmt.Println("In Etherscan but not in Unchained:")
	for e := range etherscanMap {
		if !unchainedMap[e] {
			fmt.Println(e)
		}
	}
}
