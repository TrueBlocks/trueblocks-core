package ledger2

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
)

// AssetTransfer is a raw movement of an asset derived from logs, traces, or transaction details.
type AssetTransfer struct {
	BlockNumber      base.Blknum
	TransactionIndex base.Txnum
	AssetAddress     base.Address
	AssetName        string
	Amount           base.Wei
	Index            string
	GasCost          base.Wei
	MiningReward     base.Wei
	UncleReward      base.Wei
	Withdrawal       base.Wei
	SelfDestruct     base.Wei
	Prefund          base.Wei
	InternalTxFees   base.Wei
	FromAddress      base.Address
	ToAddress        base.Address
}

// NewAssetTransfer creates an AssetTransfer with the provided fields, returning by value.
func NewAssetTransfer(
	blockNumber base.Blknum,
	txIndex base.Txnum,
	assetAddress base.Address,
	assetName string,
	amount base.Wei,
	index string,
	fromAddress base.Address,
	toAddress base.Address,
) AssetTransfer {
	return AssetTransfer{
		BlockNumber:      blockNumber,
		TransactionIndex: txIndex,
		AssetAddress:     assetAddress,
		AssetName:        assetName,
		Amount:           amount,
		Index:            index,
		FromAddress:      fromAddress,
		ToAddress:        toAddress,
	}
}

// String returns a human-readable string representation of this AssetTransfer.
func (at *AssetTransfer) String() string {
	return fmt.Sprintf(
		"AssetTransfer(Block=%d Tx=%d Asset=%s Amount=%s Index=%s From=%s To=%s)",
		at.BlockNumber,
		at.TransactionIndex,
		at.AssetAddress,
		at.Amount.String(),
		at.Index,
		at.FromAddress.Hex(),
		at.ToAddress.Hex(),
	)
}
