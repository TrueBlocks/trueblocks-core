package articulate

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (abiCache *AbiCache) ArticulateReceipt(receipt *types.SimpleReceipt) (err error) {
	if receipt == nil {
		return nil
	}

	for index := range receipt.Logs {
		if err = abiCache.ArticulateLog(&receipt.Logs[index]); err != nil {
			return err
		}
	}

	return nil
}
