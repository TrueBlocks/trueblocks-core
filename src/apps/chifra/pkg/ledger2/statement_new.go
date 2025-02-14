package ledger2

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
)

// StatementNew provides a summarized view of ledger activity for a specific asset and block range.
type StatementNew struct {
	AssetAddress string
	StartBlock   base.Blknum
	EndBlock     base.Blknum
	StartBalance base.Wei
	EndBalance   base.Wei
	TotalIn      base.Wei
	TotalOut     base.Wei
	GeneratedAt  base.Timestamp
}

// NewStatementNew constructs a StatementNew for a single asset across the specified block range.
func NewStatementNew(
	assetAddress string,
	startBlock base.Blknum,
	endBlock base.Blknum,
	startBalance base.Wei,
	endBalance base.Wei,
	totalIn base.Wei,
	totalOut base.Wei,
	generatedAt base.Timestamp,
) StatementNew {
	return StatementNew{
		AssetAddress: assetAddress,
		StartBlock:   startBlock,
		EndBlock:     endBlock,
		StartBalance: startBalance,
		EndBalance:   endBalance,
		TotalIn:      totalIn,
		TotalOut:     totalOut,
		GeneratedAt:  generatedAt,
	}
}

// String returns a human-readable description of StatementNew.
func (s StatementNew) String() string {
	return fmt.Sprintf(
		"StatementNew(Asset=%s StartBlock=%d EndBlock=%d StartBal=%s EndBal=%s In=%s Out=%s GeneratedAt=%d)",
		s.AssetAddress,
		s.StartBlock,
		s.EndBlock,
		s.StartBalance.String(),
		s.EndBalance.String(),
		s.TotalIn.String(),
		s.TotalOut.String(),
		s.GeneratedAt,
	)
}
