package articulate

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/abi"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (abiCache *AbiCache) ArticulateReceipt(chain string, receipt *types.SimpleReceipt) (err error) {
	for index := range receipt.Logs {
		address := receipt.Logs[index].Address
		if !abiCache.loadedMap.Get(address) && !abiCache.skipMap.Get(address) {
			if err := abi.LoadAbi(chain, address, &abiCache.abiMap); err != nil {
				abiCache.skipMap.Set(address, true)
				return err
			} else {
				abiCache.loadedMap.Set(address, true)
			}
		}

		if !abiCache.skipMap.Get(address) {
			if err = abiCache.ArticulateLog(chain, &receipt.Logs[index]); err != nil {
				return err
			}
		}
	}
	return nil
}
