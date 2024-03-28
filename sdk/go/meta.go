package sdk

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
)

func GetMetaData() (*rpc.MetaData, error) {
	unused := BlocksOptions{
		BlockIds: []string{"1"},
		Hashes:   true,
	}
	_, meta, err := unused.Query()
	return meta, err
}
