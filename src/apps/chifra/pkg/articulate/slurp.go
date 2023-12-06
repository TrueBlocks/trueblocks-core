package articulate

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (abiCache *AbiCache) ArticulateSlurp(slurp *types.SimpleSlurp) error {
	tx := types.SimpleTransaction{
		To: slurp.To,
	}

	if err := abiCache.ArticulateTransaction(&tx); err != nil {
		return err
	}

	slurp.ArticulatedTx = tx.ArticulatedTx
	// TODO: We could add `message` here, but we'd have to modify the `types.SimpleSlurp` struct
	return nil
}
