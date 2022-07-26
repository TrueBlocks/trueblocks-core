package names

import (
	"math/big"
	"sort"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/ethereum/go-ethereum/common"
	"github.com/ethereum/go-ethereum/common/hexutil"
)

type Allocation struct {
	Address common.Address `json:"address"`
	Balance big.Int
}

// TODO: BOGUS - SHOULD CACHE IN BINARY CACHE SINCE IT'S IMMUTABLE
func LoadPrefunds(chain string) ([]Allocation, error) {

	allocsPath := config.GetPathToChainConfig(chain) + "allocs.csv"
	lines := file.AsciiFileToLines(allocsPath)

	allocs := make([]Allocation, 0, len(lines))
	for _, line := range lines {
		record := strings.Split(line, ",")

		// silently skip errored rows...
		if len(record) != 2 {
			continue
		}

		// silently skip malformed lines
		if common.IsHexAddress(record[0]) {
			allocs = append(allocs, Allocation{
				Address: common.HexToAddress(record[0]),
				Balance: utils.ToBigInt(record[1]),
			})
		}
	}

	if len(allocs) == 0 {
		// We want at least one...
		allocs = append(allocs, Allocation{
			Address: common.HexToAddress("0x0"),
			Balance: utils.ToBigInt("0"),
		})
	}

	// sort the results by address
	sort.Slice(allocs, func(i, j int) bool {
		iAlloc := hexutil.Encode(allocs[i].Address.Bytes())
		jAlloc := hexutil.Encode(allocs[j].Address.Bytes())
		return iAlloc < jAlloc
	})

	return allocs, nil
}
