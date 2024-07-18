package main

import (
	"sort"

	"github.com/TrueBlocks/trueblocks-core/sdk/v3"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// getPrefunds returns the first nAddrs Prefund addresses with balances
// less than or equal to maxAmt wei. It returns an error if the names
// database cannot be loaded.
//
// Equivalent command line: chifra names --prefund --expand
func getPrefunds(nAddrs int, maxAmt float64) ([]types.Name, error) {
	// Prepare the query options
	namesOpts := sdk.NamesOptions{
		Terms:   []string{""},
		Prefund: true,
		Expand:  true,
	}

	// Retrieve all the prefund addresses with their balances
	names, _, err := namesOpts.Names()
	if err != nil {
		return nil, err
	}

	// Sort addresses by balance in descending order, and then by address
	sort.Slice(names, func(i, j int) bool {
		if names[i].Prefund.Cmp(&names[j].Prefund) == 0 {
			return names[i].Address.Hex() < names[j].Address.Hex()
		}
		return names[i].Prefund.Cmp(&names[j].Prefund) > 0
	})

	// Collect the first nAddrs addresses with balances <= maxAmt wei
	var ret []types.Name
	for _, name := range names {
		if len(ret) >= nAddrs {
			break
		}
		if base.ToEther(&name.Prefund).Cmp(base.NewEther(maxAmt)) <= 0 {
			name.Name = name.Address.Hex()
			ret = append(ret, name)
		}
	}

	return ret, nil
}
