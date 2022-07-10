package names

import (
	"encoding/csv"
	"errors"
	"io"
	"math/big"
	"os"
	"sort"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/ethereum/go-ethereum/common"
	"github.com/ethereum/go-ethereum/common/hexutil"
)

type Allocation struct {
	Address common.Address `json:"address"`
	Balance big.Int
}

// TODO: BOGUS - SHOULD CACHE IN BINARY CACHE SINCE IT'S IMMUTABLE
func LoadPrefunds(chain string) ([]Allocation, error) {
	allocs := []Allocation{}

	allocsPath := config.GetPathToChainConfig(chain) + "allocs.csv"
	_, err := os.Stat(allocsPath)
	if err != nil {
		if chain == "mainnet" {
			logger.Log(logger.Info, "No allocas file found for chain", chain)
		}
		return nil, nil
	}
	file, err := os.Open(allocsPath)
	if err != nil {
		return nil, err
	}
	reader := csv.NewReader(file)
	reader.FieldsPerRecord = 2

	for {
		record, err := reader.Read()
		if err == io.EOF {
			break
		}
		if err != nil {
			return allocs, err
		}
		if len(record) == 2 {
			isAddr := common.IsHexAddress(record[0])
			if isAddr {
				addr := common.HexToAddress(record[0])
				i := big.Int{}
				if len(record[1]) > 2 && record[1][:2] == "0x" {
					i.SetString(record[1][2:], 16)
				} else {
					i.SetString(record[1], 10)
				}
				allocs = append(allocs, Allocation{
					Address: addr,
					Balance: i,
				})
			}
		}
	}

	if len(allocs) == 0 {
		return allocs, errors.New("found no allocs")
	}

	sort.Slice(allocs, func(i, j int) bool {
		iAlloc := hexutil.Encode(allocs[i].Address.Bytes())
		jAlloc := hexutil.Encode(allocs[j].Address.Bytes())
		return iAlloc < jAlloc
	})

	return allocs, nil
}
