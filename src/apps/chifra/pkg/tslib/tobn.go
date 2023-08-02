package tslib

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/ethereum/go-ethereum"
)

// FromDateToBn returns a chain-specific block number given a date string (date strings are valid JSON dates).
func FromDateToBn(chain, dateStr string) (uint64, error) {
	ts, err := FromDateToTs(dateStr)
	if err != nil {
		return 0, err
	}
	ret, err := FromTs(chain, ts)
	if err != nil {
		return 0, err
	}
	return uint64(ret.Bn), err
}

// FromNameToBn returns the chain-specific block number (if found) given the name of a special block. The list of special blocks is per-chain.
func FromNameToBn(chain, name string) (uint64, error) {
	if name == "latest" {
		conn := rpcClient.NewConnection(chain)
		meta, err := conn.GetMetaData(chain, false)
		if err != nil {
			return 0, err
		}
		return meta.Latest, nil
	}

	specials, _ := GetSpecials(chain) // it's okay if it's empty
	for _, value := range specials {
		if value.Name == name {
			return value.BlockNumber, nil
		}
	}

	return uint64(utils.NOPOS), fmt.Errorf("block at %s returned an error: %w", name, ethereum.NotFound)
}

// FromTsToBn returns a chain-specific block number given a Linux timestamp.
func FromTsToBn(chain string, ts base.Timestamp) (uint64, error) {
	ret, err := FromTs(chain, ts)
	if err != nil {
		return 0, err
	}
	return uint64(ret.Bn), err
}
