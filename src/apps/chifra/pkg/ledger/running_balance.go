package ledger

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/types"
)

// -----------------------------------------------------------------
// runningBalance represents the current balance and associated statement for an asset holder.
type runningBalance struct {
	amt  base.Wei
	stmt *types.Statement
}

// -----------------------------------------------------------------
func (r *runningBalance) String() string {
	return fmt.Sprintf("running blkid: %d amount: %s", r.Block(), r.Amount().Text(10))
}

// -----------------------------------------------------------------
func (r *runningBalance) Block() base.Blknum {
	if r.stmt == nil {
		return 0
	}
	return r.stmt.BlockNumber
}

// -----------------------------------------------------------------
func (r *runningBalance) Amount() *base.Wei {
	return &r.amt
}
