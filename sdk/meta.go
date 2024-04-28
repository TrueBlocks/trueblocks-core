package sdk

import "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"

func GetMetaData() (*types.MetaData, error) {
	unused := BlocksOptions{
		BlockIds: []string{"1"},
	}
	_, meta, err := unused.BlocksHashes()
	return meta, err
}
