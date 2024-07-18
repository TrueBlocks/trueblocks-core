package sdk

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func GetMetaData(chain string) (*types.MetaData, error) {
	unused := BlocksOptions{
		BlockIds: []string{"1"},
		Globals:  Globals{Chain: chain},
	}
	_, meta, err := unused.BlocksHashes()
	return meta, err
}

func GetLatestBlock(chain string) base.Blknum {
	meta, _ := GetMetaData(chain)
	return meta.Latest
}
