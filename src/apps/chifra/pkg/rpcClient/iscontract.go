package rpcClient

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

func IsSmartContract(chain, addr string) (bool, error) {
	bytes, err := GetCodeAt(chain, base.HexToAddress(addr), utils.NOPOS)
	return len(bytes) > 0, err
}
