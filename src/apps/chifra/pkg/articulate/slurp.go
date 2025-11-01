package articulate

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/types"
)

func (abiCache *AbiCache) ArticulateSlurp(slurp *types.Slurp) error {
	tx := types.Transaction{
		To:    slurp.To,
		Input: slurp.Input,
	}

	if err := abiCache.ArticulateTransaction(&tx); err != nil {
		return err
	}

	slurp.ArticulatedTx = tx.ArticulatedTx
	// TODO: We could add `message` here, but we'd have to modify the `types.Slurp` struct
	return nil
}
