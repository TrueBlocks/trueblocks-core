package articulate

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/abi"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (cache *AbiCache) ArticulateReceipt(chain string, receipt *types.SimpleReceipt) (err error) {
	for index := range receipt.Logs {
		address := receipt.Logs[index].Address
		if !cache.loadedMap[address] && !cache.skipMap[address] {
			if err := abi.LoadAbi(chain, address, cache.abiMap); err != nil {
				cache.skipMap[address] = true
				return err
			} else {
				cache.loadedMap[address] = true
			}
		}

		if !cache.skipMap[address] {
			if err = cache.ArticulateLog(chain, &receipt.Logs[index]); err != nil {
				return err
			}
		}
	}
	return nil
}
