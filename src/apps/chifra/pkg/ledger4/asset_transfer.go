package ledger4

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// AssetTransfer is a raw movement of an asset derived from logs, traces, or transaction details.
type AssetTransfer = types.Statement

type AssetTransferNew struct {
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
