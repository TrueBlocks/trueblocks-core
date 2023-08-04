package articulate

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/abi"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (abiCache *AbiCache) ArticulateReceipt(chain string, receipt *types.SimpleReceipt) (err error) {
	for index := range receipt.Logs {
		address := receipt.Logs[index].Address
		if !abiCache.loadedMap.GetValue(address) && !abiCache.skipMap.GetValue(address) {
			if err := abi.LoadAbi(chain, address, &abiCache.abiMap); err != nil {
				abiCache.skipMap.SetValue(address, true)
				return err
			} else {
				abiCache.loadedMap.SetValue(address, true)
			}
		}

		if !abiCache.skipMap.GetValue(address) {
			if err = abiCache.ArticulateLog(chain, &receipt.Logs[index]); err != nil {
				return err
			}
		}
	}
	return nil
}
