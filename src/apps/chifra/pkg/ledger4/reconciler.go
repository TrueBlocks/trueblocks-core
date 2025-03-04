package ledger4

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/filter"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/walk"
)

type Reconcilerer interface {
	GetStatements(pos *types.AppPosition, filter *filter.AppearanceFilter, trans *types.Transaction) ([]types.Statement, error)
	GetTransfers(pos *types.AppPosition, filter *filter.AppearanceFilter, trans *types.Transaction) ([]types.Transfer, error)
}

// Reconciler4 tracks asset movements for a specific holder
type Reconciler4 struct {
	holder    base.Address
	conn      *rpc.Connection
	balances  map[base.Address]*base.Wei
	transfers []AssetTransferNew
}

func NewReconciler4(chain string, holder base.Address, enabled bool, enabledMap map[walk.CacheType]bool) *Reconciler4 {
	return &Reconciler4{
		holder:    holder,
		conn:      rpc.NewConnection(chain, enabled, enabledMap),
		balances:  make(map[base.Address]*base.Wei),
		transfers: []AssetTransferNew{},
	}
}

// func (r *Reconciler4) ProcessAppearance(blockNum base.Blknum, txIdx base.Txnum) error {
// 	tx, err := r.conn.GetTransactionByNumberAndId(blockNum, txIdx)
// 	if err != nil {
// 		return fmt.Errorf("failed to get transaction at block %d, index %d: %v", blockNum, txIdx, err)
// 	}

// 	r.processETHTransfer(tx)
// 	r.processGasFee(tx)
// 	r.processTokenTransfers(receipt, blockNum, timestamp)
// 	traces, err := r.conn.GetTracesByBlockNumberAndIndex(blockNum, txIdx)
// 	if err != nil {
// 		return fmt.Errorf("failed to get traces at block %d, index %d: %v", blockNum, txIdx, err)
// 	}
// 	r.processInternalTransfers(traces, blockNum, timestamp)

// 	return nil
// }

// func newEthTransfer(holder base.Address, tx *types.Transaction) AssetTransferNew {
// 	return AssetTransferNew{
// 		Amount:           base.ZeroWei, /* the caller will fill this in */
// 		AssetAddress:     base.FAKE_ETH_ADDRESS,
// 		BlockNumber:      tx.BlockNumber,
// 		TransactionIndex: tx.TransactionIndex,
// 		LogIndex:         base.Lognum(base.NOPOS),
// 		TraceIndex:       "0",
// 		TransactionHash:  tx.Hash,
// 		GasPrice:         tx.GasPrice,
// 		GasUsed:          base.Gas(0),
// 		Holder:           holder,
// 		Recipient:        tx.To,
// 		Reason:           "eth_transfer",
// 		Sender:           tx.From,
// 		Timestamp:        tx.Timestamp,
// 	}
// }

// func (r *Reconciler4) processETHTransfer(tx *types.Transaction) {
// 	transfer := newEthTransfer(r.holder, tx)
// 	if transfer.Recipient == r.holder {
// 		transfer.Amount = &tx.Value
// 	} else if tx.From == r.holder {
// 		transfer.Amount = tx.Value.Neg()
// 	}
// 	r.transfers = append(r.transfers, transfer)
// }

// func (r *Reconciler4) processGasFee(tx *types.Transaction) {
// 	gasUsed := base.Gas(receipt.GasUsed)
// 	gasPrice := tx.GasPrice
// 	fee := new(base.Wei).Mul(new(base.Wei).SetUint64(uint64(gasUsed)), gasPrice)

// 	r.addTransfer(AssetTransferNew{
// 		Amount:           new(base.Wei).Neg(fee),
// 		AssetAddress:     base.FAKE_ETH_ADDRESS,
// 		BlockNumber:      blockNum,
// 		GasPrice:         gasPrice,
// 		GasUsed:          gasUsed,
// 		Holder:           r.holder,
// 		LogIndex:         base.Lognum(0),
// 		Recipient:        base.HexToAddress("0xMiner"), // Simplified recipient for gas fees
// 		Reason:           "gas_fee",
// 		Sender:           r.holder,
// 		Timestamp:        timestamp,
// 		TraceIndex:       "",
// 		TransactionHash:  tx.Hash,
// 		TransactionIndex: tx.TransactionIndex,
// 	})
// }

// // processTokenTransfers handles ERC-20 token transfers from receipt logs
// func (r *Reconciler4) processTokenTransfers(receipt *types.Receipt, blockNum base.Blknum, timestamp base.Timestamp) {
// 	for _, log := range receipt.Logs {
// 		if len(log.Topics) == 3 && log.Topics[0] == base.HexToHash("0xddf252ad1be2c89b69c2b068fc378daa952ba7f163c4a11628f55a4df523b3ef") {
// 			from := base.HexToAddress(log.Topics[1].Hex())
// 			to := base.HexToAddress(log.Topics[2].Hex())
// 			amount := new(base.Wei).SetBytes(log.Data)
// 			tokenAddr := log.Address

// 			gasPrice := new(base.Wei).SetUint64(0)
// 			gasUsed := base.Gas(0)

// 			if from == r.holder {
// 				r.addTransfer(AssetTransferNew{
// 					Amount:           new(base.Wei).Neg(amount),
// 					AssetAddress:     tokenAddr,
// 					BlockNumber:      blockNum,
// 					GasPrice:         gasPrice,
// 					GasUsed:          gasUsed,
// 					Holder:           r.holder,
// 					LogIndex:         log.LogIndex,
// 					Recipient:        to,
// 					Reason:           "token_transfer",
// 					Sender:           from,
// 					Timestamp:        timestamp,
// 					TraceIndex:       "",
// 					TransactionHash:  receipt.TxHash,
// 					TransactionIndex: receipt.TransactionIndex,
// 				})
// 			} else if to == r.holder {
// 				r.addTransfer(AssetTransferNew{
// 					Amount:           amount,
// 					AssetAddress:     tokenAddr,
// 					BlockNumber:      blockNum,
// 					GasPrice:         gasPrice,
// 					GasUsed:          gasUsed,
// 					Holder:           r.holder,
// 					LogIndex:         log.LogIndex,
// 					Recipient:        to,
// 					Reason:           "token_transfer",
// 					Sender:           from,
// 					Timestamp:        timestamp,
// 					TraceIndex:       "",
// 					TransactionHash:  receipt.TxHash,
// 					TransactionIndex: receipt.TransactionIndex,
// 				})
// 			}
// 		}
// 	}
// }

// // processInternalTransfers handles internal ETH transfers from traces
// func (r *Reconciler4) processInternalTransfers(traces []types.Trace) {
// 	for _, trace := range traces {
// 		if trace.Action.CallType == "call" && !trace.Action.Value.Equal(base.ZeroWei) {
// 			from := trace.Action.From
// 			to := trace.Action.To
// 			value := trace.Action.Value

// 			gasPrice := new(base.Wei).SetUint64(0)
// 			gasUsed := base.Gas(0)
// 			logIndex := base.Lognum(0)

// 			if from == r.holder {
// 				r.addTransfer(AssetTransferNew{
// 					Amount:           new(base.Wei).Neg(value),
// 					AssetAddress:     base.FAKE_ETH_ADDRESS,
// 					BlockNumber:      blockNum,
// 					GasPrice:         gasPrice,
// 					GasUsed:          gasUsed,
// 					Holder:           r.holder,
// 					LogIndex:         logIndex,
// 					Recipient:        to,
// 					Reason:           "internal_call",
// 					Sender:           from,
// 					Timestamp:        tx.Timestamp,
// 					TraceIndex:       trace.TraceIndex,
// 					TransactionHash:  trace.TxHash,
// 					TransactionIndex: trace.TransactionIndex,
// 				})
// 			} else if to == r.holder {
// 				r.addTransfer(AssetTransferNew{
// 					Amount:           value,
// 					AssetAddress:     base.FAKE_ETH_ADDRESS,
// 					BlockNumber:      blockNum,
// 					GasPrice:         gasPrice,
// 					GasUsed:          gasUsed,
// 					Holder:           r.holder,
// 					LogIndex:         logIndex,
// 					Recipient:        to,
// 					Reason:           "internal_call",
// 					Sender:           from,
// 					Timestamp:        timestamp,
// 					TraceIndex:       trace.TraceIndex,
// 					TransactionHash:  trace.TxHash,
// 					TransactionIndex: trace.TransactionIndex,
// 				})
// 			}
// 		}
// 	}
// }

// func (r *Reconciler4) ReconcileBlock(blockNum base.Blknum) error {
// 	onChainETH, err := r.conn.GetBalanceAt(r.holder, blockNum)
// 	if err != nil {
// 		return err
// 	}
// 	ourETH := r.balances[zeroAddr]
// 	if ourETH == nil {
// 		ourETH = new(base.Wei).SetUint64(0)
// 	}
// 	if ourETH.Cmp(onChainETH) != 0 {
// 		adjustment := new(base.Wei).Sub(onChainETH, ourETH)
// 		r.addTransfer(AssetTransferNew{
// 			Amount:           adjustment,
// 			AssetAddress:     zeroAddr,
// 			BlockNumber:      blockNum,
// 			GasPrice:         new(base.Wei).SetUint64(0),
// 			GasUsed:          base.Gas(0),
// 			Holder:           r.holder,
// 			LogIndex:         base.Lognum(0),
// 			Recipient:        r.holder,
// 			Reason:           "adjustment",
// 			Sender:           base.HexToAddress("0xAdjustment"),
// 			Timestamp:        0, // Replace with actual block timestamp if needed
// 			TraceIndex:       "",
// 			TransactionHash:  base.Hash(""),
// 			TransactionIndex: base.Txnum(0),
// 		})
// 	}
// 	// Add similar logic for token balances
// 	return nil
// }
