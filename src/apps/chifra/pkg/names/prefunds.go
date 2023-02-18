package names

import (
	"math/big"
	"sort"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/ethereum/go-ethereum/common"
	"github.com/ethereum/go-ethereum/common/hexutil"
)

type Allocation struct {
	Address types.Address `json:"address"`
	Balance big.Int
}

func LoadPrefunds(chain string) ([]Allocation, error) {

	prefundPath := config.GetPathToChainConfig(chain) + "allocs.csv"
	lines := file.AsciiFileToLines(prefundPath)

	prefunds := make([]Allocation, 0, len(lines))
	for _, line := range lines {
		record := strings.Split(line, ",")

		// silently skip errored rows...
		if len(record) != 2 {
			continue
		}

		// silently skip malformed lines
		if common.IsHexAddress(record[0]) {
			prefunds = append(prefunds, Allocation{
				Address: types.HexToAddress(record[0]),
				Balance: utils.Str_2_BigInt(record[1]),
			})
		}
	}

	if len(prefunds) == 0 {
		// We want at least one...
		prefunds = append(prefunds, Allocation{
			Address: types.HexToAddress("0x0"),
			Balance: utils.Str_2_BigInt("0"),
		})
	}

	// sort the results by address
	sort.Slice(prefunds, func(i, j int) bool {
		item := hexutil.Encode(prefunds[i].Address.Bytes())
		jtem := hexutil.Encode(prefunds[j].Address.Bytes())
		return item < jtem
	})

	return prefunds, nil
}
