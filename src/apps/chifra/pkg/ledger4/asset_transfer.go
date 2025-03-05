package ledger4

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/filter"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

type Reconcilerer interface {
	GetStatements(pos *types.AppPosition, filter *filter.AppearanceFilter, trans *types.Transaction) ([]types.Statement, error)
	GetTransfers(pos *types.AppPosition, filter *filter.AppearanceFilter, trans *types.Transaction) ([]types.Transfer, error)
}

// AssetTransfer is a raw movement of an asset derived from logs, traces, or transaction details.
type AssetTransfer = types.Statement

/*
type A ssetTransferNew struct {
	Amount           *base.Wei
	AssetAddress     base.Address
	BlockNumber      base.Blknum
	GasPrice         base.Gas
	GasUsed          base.Gas
	Holder           base.Address
	LogIndex         base.Lognum
	Recipient        base.Address
	Reason           string
	Sender           base.Address
	Timestamp        base.Timestamp
	TraceIndex       string
	TransactionHash  base.Hash
	TransactionIndex base.Txnum
}
*/
