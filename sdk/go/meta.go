package sdk

import "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"

func GetMetaData() (*types.MetaData, error) {
	unused := BlocksOptions{
		BlockIds: []string{"1"},
		Hashes:   true,
	}
	_, meta, err := unused.Query()
	return meta, err
}
