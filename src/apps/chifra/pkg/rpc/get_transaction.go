package rpc

import (
	"context"
	"errors"
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/prefunds"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc/query"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/ethereum/go-ethereum"
)

func (conn *Connection) GetTransactionByNumberAndId(bn base.Blknum, txid base.Txnum) (*types.Transaction, error) {
	if conn.StoreReadable() {
		tx := &types.Transaction{
			BlockNumber:      bn,
			TransactionIndex: txid,
		}

		if err := conn.Store.Read(tx, nil); err == nil {
			// success
			return tx, nil
		}
	}

	trans, err := conn.getTransactionFromRpc(notAHash, notAHash, bn, txid)
	if err != nil {
		return nil, err
	}

	blockTs := conn.GetBlockTimestamp(bn)
	receipt, err := conn.GetReceipt(bn, txid, blockTs)
	if err != nil {
		return nil, err
	}

	trans.Timestamp = blockTs
	trans.HasToken = types.IsTokenFunction(trans.Input)
	trans.GasUsed = receipt.GasUsed
	trans.IsError = receipt.IsError
	trans.Receipt = &receipt

	if conn.StoreWritable() && conn.EnabledMap["transactions"] && base.IsFinal(conn.LatestBlockTimestamp, blockTs) {
		_ = conn.Store.Write(trans, nil)
	}

	return trans, nil
}

func (conn *Connection) GetTransactionByAppearance(app *types.Appearance, fetchTraces bool) (*types.Transaction, error) {
	theApp := types.Appearance{
		BlockNumber:      app.BlockNumber,
		TransactionIndex: app.TransactionIndex,
	}
	if !app.Address.IsZero() {
		theApp.Address = app.Address
	}

	bn := base.Blknum(theApp.BlockNumber)
	txid := base.Txnum(theApp.TransactionIndex)

	if conn.StoreReadable() {
		tx := &types.Transaction{
			BlockNumber:      bn,
			TransactionIndex: txid,
		}

		if err := conn.Store.Read(tx, nil); err == nil {
			// success
			if fetchTraces {
				traces, err := conn.GetTracesByTransactionHash(tx.Hash.Hex(), tx)
				if err != nil {
					return nil, err
				}
				tx.Traces = traces
			}
			return tx, nil
		}
	}

	blockTs := conn.GetBlockTimestamp(bn)
	if bn == 0 {
		if tx, err := conn.GetTransactionPrefundByApp(&theApp); err != nil {
			return nil, err
		} else {
			tx.Timestamp = blockTs
			if conn.StoreWritable() && conn.EnabledMap["transactions"] && base.IsFinal(conn.LatestBlockTimestamp, blockTs) {
				_ = conn.Store.Write(tx, nil)
			}
			return tx, nil
		}
	} else if txid == types.BlockReward || txid == types.MisconfigReward || txid == types.ExternalReward {
		if tx, err := conn.GetTransactionRewardByTypeAndApp(types.BlockReward, &theApp); err != nil {
			return nil, err
		} else {
			tx.Timestamp = blockTs
			if conn.StoreWritable() && conn.EnabledMap["transactions"] && base.IsFinal(conn.LatestBlockTimestamp, blockTs) {
				_ = conn.Store.Write(tx, nil)
			}
			return tx, nil
		}
	} else if txid == types.UncleReward {
		if tx, err := conn.GetTransactionRewardByTypeAndApp(types.UncleReward, &theApp); err != nil {
			return nil, err
		} else {
			tx.Timestamp = blockTs
			if conn.StoreWritable() && conn.EnabledMap["transactions"] && base.IsFinal(conn.LatestBlockTimestamp, blockTs) {
				_ = conn.Store.Write(tx, nil)
			}
			return tx, nil
		}
	} else if txid == types.WithdrawalAmt {
		if tx, err := conn.GetTransactionRewardByTypeAndApp(types.WithdrawalAmt, &theApp); err != nil {
			return nil, err
		} else {
			tx.Timestamp = blockTs
			if conn.StoreWritable() && conn.EnabledMap["transactions"] && base.IsFinal(conn.LatestBlockTimestamp, blockTs) {
				_ = conn.Store.Write(tx, nil)
			}
			return tx, nil
		}
	}

	receipt, err := conn.GetReceipt(bn, txid, blockTs)
	if err != nil {
		return nil, err
	}

	trans, err := conn.getTransactionFromRpc(notAHash, notAHash, bn, txid)
	if err != nil {
		return nil, err
	}

	trans.Timestamp = blockTs
	trans.HasToken = types.IsTokenFunction(trans.Input)
	trans.GasUsed = receipt.GasUsed
	trans.IsError = receipt.IsError
	trans.Receipt = &receipt

	if conn.StoreWritable() && conn.EnabledMap["transactions"] && base.IsFinal(conn.LatestBlockTimestamp, blockTs) {
		_ = conn.Store.Write(trans, nil)
	}

	if fetchTraces {
		traces, err := conn.GetTracesByTransactionHash(trans.Hash.Hex(), trans)
		if err != nil {
			return nil, err
		}
		trans.Traces = traces
	}

	return trans, err
}

// GetTransactionAppByHash returns a transaction's appearance if it's a valid transaction
func (conn *Connection) GetTransactionAppByHash(hash string) (types.Appearance, error) {
	var ret types.Appearance
	if trans, err := conn.getTransactionFromRpc(notAHash, base.HexToHash(hash), base.NOPOSN, base.NOPOSN); err != nil {
		return ret, err
	} else {
		ret.BlockNumber = uint32(trans.BlockNumber)
		ret.TransactionIndex = uint32(trans.TransactionIndex)
		return ret, nil
	}
}

// GetTransactionHashByNumberAndID returns a transaction's hash if it's a valid transaction
func (conn *Connection) GetTransactionHashByNumberAndID(bn base.Blknum, txId base.Txnum) (base.Hash, error) {
	if trans, err := conn.getTransactionFromRpc(notAHash, notAHash, bn, txId); err != nil {
		return base.Hash{}, err
	} else {
		return trans.Hash, nil
	}
}

// GetTransactionHashByHash returns a transaction's hash if it's a valid transaction, an empty string otherwise
func (conn *Connection) GetTransactionHashByHash(hash string) (string, error) {
	if trans, err := conn.getTransactionFromRpc(notAHash, base.HexToHash(hash), base.NOPOSN, base.NOPOSN); err != nil {
		return "", err
	} else {
		return trans.Hash.Hex(), nil
	}
}

// GetTransactionHashByHashAndID returns a transaction's hash if it's a valid transaction
func (conn *Connection) GetTransactionHashByHashAndID(hash string, txId base.Txnum) (string, error) {
	if trans, err := conn.getTransactionFromRpc(base.HexToHash(hash), notAHash, base.NOPOSN, txId); err != nil {
		return "", err
	} else {
		return trans.Hash.Hex(), nil
	}
}

func (conn *Connection) GetTransactionPrefundByApp(theApp *types.Appearance) (tx *types.Transaction, err error) {
	// TODO: performance - This loads and then drops the file every time it's called. Quite slow.
	// TODO: performance - in the old C++ we stored these values in a pre fundAddrMap so that given a txid in block zero
	// TODO: performance - we knew which address was granted allocation at that transaction.
	prefundPath := prefunds.GetPrefundPath(conn.Chain)
	if prefundMap, err := prefunds.LoadPrefundMap(conn.Chain, prefundPath); err != nil {
		return nil, err
	} else {
		var blockHash base.Hash
		var ts base.Timestamp
		if block, err := conn.GetBlockHeaderByNumber(0); err != nil {
			return nil, err
		} else {
			blockHash = block.Hash
			ts = block.Timestamp
		}

		entry := (*prefundMap)[theApp.Address]
		if entry.Address == theApp.Address {
			ret := types.Transaction{
				BlockHash:        blockHash,
				BlockNumber:      base.Blknum(theApp.BlockNumber),
				TransactionIndex: base.Txnum(theApp.TransactionIndex),
				Timestamp:        ts,
				From:             base.PrefundSender,
				To:               theApp.Address,
				Value:            entry.Prefund,
			}
			return &ret, nil
		}
	}
	return nil, errors.New("not found")
}

// TODO: This is not cross-chain correct nor does it work properly for post-merge

func (conn *Connection) GetTransactionRewardByTypeAndApp(rt base.Txnum, theApp *types.Appearance) (*types.Transaction, error) {
	if block, err := conn.GetBlockBodyByNumber(base.Blknum(theApp.BlockNumber)); err != nil {
		return nil, err
	} else {
		if rt == types.WithdrawalAmt {
			tx := &types.Transaction{
				BlockNumber:      base.Blknum(theApp.BlockNumber),
				TransactionIndex: base.Txnum(theApp.TransactionIndex),
				Timestamp:        block.Timestamp,
				From:             base.WithdrawalSender,
				To:               theApp.Address,
			}
			return tx, nil
		}

		if uncles, err := conn.GetUncleBodiesByNumber(base.Blknum(theApp.BlockNumber)); err != nil {
			return nil, err
		} else {
			var blockReward = base.NewWei(0)
			var nephewReward = base.NewWei(0)
			var feeReward = base.NewWei(0)
			var uncleReward = base.NewWei(0)

			sender := theApp.Address
			bn := base.Blknum(theApp.BlockNumber)
			blockReward = conn.getBlockReward(bn)
			switch rt {
			case types.BlockReward:
				if block.Miner == theApp.Address {
					sender = base.BlockRewardSender
					nUncles := len(uncles)
					if nUncles > 0 {
						nephewReward = new(base.Wei).Mul(blockReward, base.NewWei(int64(nUncles)))
						nephewReward.Div(nephewReward, base.NewWei(32))
					}
					for _, tx := range block.Transactions {
						gp := base.NewWei(int64(tx.GasPrice))
						gu := base.NewWei(int64(tx.Receipt.GasUsed))
						feeReward = feeReward.Add(feeReward, gp.Mul(gp, gu))
					}
				} else {
					blockReward = base.NewWei(0)
				}
			case types.UncleReward:
				for _, uncle := range uncles {
					if uncle.Miner == theApp.Address {
						sender = base.UncleRewardSender
						if bn < uncle.BlockNumber+6 {
							diff := (uncle.BlockNumber + 8 - bn) // positive since +6 < bn
							uncleReward = new(base.Wei).Mul(blockReward, base.NewWei(int64(diff)))
							uncleReward.Div(uncleReward, base.NewWei(8))
						}
					}
				}
				if block.Miner == theApp.Address {
					sender = base.BlockRewardSender // if it's both, it's the block reward
					// The uncle miner may also have been the miner of the block
					if minerTx, err := conn.GetTransactionRewardByTypeAndApp(types.BlockReward, theApp); err != nil {
						return nil, err
					} else {
						blockReward = &minerTx.Rewards.Block
						nephewReward = &minerTx.Rewards.Nephew
						feeReward = &minerTx.Rewards.TxFee
					}
				} else {
					blockReward = base.NewWei(0)
				}
			case types.NephewReward:
				fallthrough
			case types.TxFeeReward:
				fallthrough
			default:
				return nil, errors.New("invalid reward type")
			}

			rewards, total := types.NewReward(blockReward, nephewReward, feeReward, uncleReward)
			tx := &types.Transaction{
				BlockNumber:      base.Blknum(theApp.BlockNumber),
				TransactionIndex: base.Txnum(theApp.TransactionIndex),
				BlockHash:        block.Hash,
				Timestamp:        block.Timestamp,
				From:             sender,
				To:               theApp.Address,
				Value:            total,
				Rewards:          &rewards,
			}
			return tx, nil
		}
	}
}

// GetTransactionCountInBlock returns the number of transactions in a block
func (conn *Connection) GetTransactionCountInBlock(bn base.Blknum) (uint64, error) {
	// TODO: Can we use our Query here?
	if ec, err := conn.getClient(); err != nil {
		return 0, err
	} else {
		defer ec.Close()

		block, err := ec.BlockByNumber(context.Background(), base.BiFromBn(bn))
		if err != nil {
			return 0, err
		}

		cnt, err := ec.TransactionCount(context.Background(), block.Hash())
		return uint64(cnt), err
	}
}

var (
	notAHash = base.Hash{}
)

func (conn *Connection) getTransactionFromRpc(blkHash base.Hash, txHash base.Hash, bn base.Blknum, txid base.Txnum) (*types.Transaction, error) {
	method := "eth_getTransactionByBlockNumberAndIndex"
	params := query.Params{fmt.Sprintf("0x%x", bn), fmt.Sprintf("0x%x", txid)}
	if txHash != notAHash {
		method = "eth_getTransactionByHash"
		params = query.Params{txHash.Hex()}
	} else if blkHash != notAHash {
		method = "eth_getTransactionByBlockHashAndIndex"
		params = query.Params{blkHash.Hex(), fmt.Sprintf("0x%x", txid)}
	}

	if trans, err := query.Query[types.Transaction](conn.Chain, method, params); err != nil {
		return &types.Transaction{
			Receipt: &types.Receipt{},
		}, err
	} else if trans.Hash.IsZero() {
		return &types.Transaction{
			Receipt: &types.Receipt{},
		}, ethereum.NotFound
	} else {
		return trans, nil
	}
}
