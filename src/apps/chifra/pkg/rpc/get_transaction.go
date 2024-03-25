package rpc

import (
	"context"
	"errors"
	"fmt"
	"math/big"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/prefunds"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc/query"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/ethereum/go-ethereum/common"
)

func (conn *Connection) GetTransactionByNumberAndId(bn base.Blknum, txid uint64) (tx *types.SimpleTransaction, err error) {
	if conn.StoreReadable() {
		tx = &types.SimpleTransaction{
			BlockNumber:      bn,
			TransactionIndex: txid,
		}

		if err := conn.Store.Read(tx, nil); err == nil {
			// success
			return tx, nil
		}
	}

	rawTx, err := conn.getTransactionRaw(notAHash, notAHash, bn, txid)
	if err != nil {
		return
	}

	blockTs := conn.GetBlockTimestamp(bn)
	receipt, err := conn.GetReceipt(bn, txid, blockTs)
	if err != nil {
		return
	}

	tx = types.NewSimpleTransaction(rawTx, &receipt, blockTs)
	if conn.StoreWritable() && conn.EnabledMap["transactions"] && base.IsFinal(conn.LatestBlockTimestamp, blockTs) {
		_ = conn.Store.Write(tx, nil)
	}

	return
}

// TODO: See #3361

func (conn *Connection) GetTransactionByAppearance(app *types.SimpleAppearance, fetchTraces bool) (tx *types.SimpleTransaction, err error) {
	raw := types.RawAppearance{
		BlockNumber:      app.BlockNumber,
		TransactionIndex: app.TransactionIndex,
	}
	if !app.Address.IsZero() {
		raw.Address = app.Address.Hex()
	}

	bn := uint64(raw.BlockNumber)
	txid := uint64(raw.TransactionIndex)

	if conn.StoreReadable() {
		tx = &types.SimpleTransaction{
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

	tx = nil
	if bn == 0 {
		if tx, err = conn.GetTransactionPrefundByApp(&raw); err != nil {
			return nil, err
		}
	} else if txid == types.BlockReward || txid == types.MisconfigReward || txid == types.ExternalReward {
		if tx, err = conn.GetTransactionRewardByTypeAndApp(types.BlockReward, &raw); err != nil {
			return nil, err
		}
	} else if txid == types.UncleReward {
		if tx, err = conn.GetTransactionRewardByTypeAndApp(types.UncleReward, &raw); err != nil {
			return nil, err
		}
	} else if txid == types.Withdrawal {
		if tx, err = conn.GetTransactionRewardByTypeAndApp(types.Withdrawal, &raw); err != nil {
			return nil, err
		}
	}

	blockTs := conn.GetBlockTimestamp(bn)
	if tx != nil {
		tx.Timestamp = blockTs
		if conn.StoreWritable() && conn.EnabledMap["transactions"] && base.IsFinal(conn.LatestBlockTimestamp, blockTs) {
			_ = conn.Store.Write(tx, nil)
		}
		return tx, nil
	}

	receipt, err := conn.GetReceipt(bn, txid, blockTs)
	if err != nil {
		return
	}

	rawTx, err := conn.getTransactionRaw(notAHash, notAHash, bn, txid)
	if err != nil {
		return
	}

	tx = types.NewSimpleTransaction(rawTx, &receipt, blockTs)

	if conn.StoreWritable() && conn.EnabledMap["transactions"] && base.IsFinal(conn.LatestBlockTimestamp, blockTs) {
		_ = conn.Store.Write(tx, nil)
	}

	if fetchTraces {
		traces, err := conn.GetTracesByTransactionHash(tx.Hash.Hex(), tx)
		if err != nil {
			return nil, err
		}
		tx.Traces = traces
	}

	return
}

func (conn *Connection) GetTransactionAppByHash(hash string) (types.RawAppearance, error) {
	var ret types.RawAppearance
	if rawTx, err := conn.getTransactionRaw(notAHash, base.HexToHash(hash), notAnInt, notAnInt); err != nil {
		return ret, err
	} else {
		ret.BlockNumber = uint32(utils.MustParseUint(rawTx.BlockNumber))
		ret.TransactionIndex = uint32(utils.MustParseUint(rawTx.TransactionIndex))
		return ret, nil
	}
}

// GetTransactionHashByNumberAndID returns a transaction's hash if it's a valid transaction
func (conn *Connection) GetTransactionHashByNumberAndID(bn, txId uint64) (base.Hash, error) {
	if ec, err := conn.getClient(); err != nil {
		return base.Hash{}, err
	} else {
		defer ec.Close()

		block, err := ec.BlockByNumber(context.Background(), base.BiFromUint64(bn))
		if err != nil {
			return base.Hash{}, err
		}

		tx, err := ec.TransactionInBlock(context.Background(), block.Hash(), uint(txId))
		if err != nil {
			return base.Hash{}, err
		}

		return base.HexToHash(tx.Hash().Hex()), nil
	}
}

// GetTransactionHashByHash returns a transaction's hash if it's a valid transaction, an empty string otherwise
func (conn *Connection) GetTransactionHashByHash(hash string) (string, error) {
	if ec, err := conn.getClient(); err != nil {
		return "", err
	} else {
		defer ec.Close()

		tx, _, err := ec.TransactionByHash(context.Background(), common.HexToHash(hash))
		if err != nil {
			return "", err
		}

		return tx.Hash().Hex(), nil
	}
}

// GetTransactionHashByHashAndID returns a transaction's hash if it's a valid transaction
func (conn *Connection) GetTransactionHashByHashAndID(hash string, txId uint64) (string, error) {
	if ec, err := conn.getClient(); err != nil {
		return "", err
	} else {
		defer ec.Close()

		tx, err := ec.TransactionInBlock(context.Background(), common.HexToHash(hash), uint(txId))
		if err != nil {
			return "", err
		}

		return tx.Hash().Hex(), nil
	}
}

func (conn *Connection) GetTransactionPrefundByApp(raw *types.RawAppearance) (tx *types.SimpleTransaction, err error) {
	// TODO: performance - This loads and then drops the file every time it's called. Quite slow.
	// TODO: performance - in the old C++ we stored these values in a pre fundAddrMap so that given a txid in block zero
	// TODO: performance - we knew which address was granted allocation at that transaction.
	prefundPath := prefunds.GetPrefundPath(conn.Chain)
	if prefundMap, err := prefunds.LoadPrefundMap(conn.Chain, prefundPath); err != nil {
		return nil, err
	} else {
		var blockHash base.Hash
		var ts int64
		if block, err := conn.GetBlockHeaderByNumber(uint64(0)); err != nil {
			return nil, err
		} else {
			blockHash = block.Hash
			ts = block.Timestamp
		}

		entry := (*prefundMap)[base.HexToAddress(raw.Address)]
		if entry.Address.Hex() == raw.Address {
			ret := types.SimpleTransaction{
				BlockHash:        blockHash,
				BlockNumber:      uint64(raw.BlockNumber),
				TransactionIndex: uint64(raw.TransactionIndex),
				Timestamp:        ts,
				From:             base.PrefundSender,
				To:               base.HexToAddress(raw.Address),
				Value:            entry.Prefund,
			}
			return &ret, nil
		}
	}
	return nil, errors.New("not found")
}

// TODO: This is not cross-chain correct nor does it work properly for post-merge

func (conn *Connection) GetTransactionRewardByTypeAndApp(rt base.Txnum, raw *types.RawAppearance) (*types.SimpleTransaction, error) {
	if block, err := conn.GetBlockBodyByNumber(uint64(raw.BlockNumber)); err != nil {
		return nil, err
	} else {
		if rt == types.Withdrawal {
			tx := &types.SimpleTransaction{
				BlockNumber:      uint64(raw.BlockNumber),
				TransactionIndex: uint64(raw.TransactionIndex),
				Timestamp:        block.Timestamp,
				From:             base.WithdrawalSender,
				To:               base.HexToAddress(raw.Address),
			}
			return tx, nil
		}

		if uncles, err := conn.GetUncleBodiesByNumber(uint64(raw.BlockNumber)); err != nil {
			return nil, err
		} else {
			var blockReward = base.NewMyWei(0)
			var nephewReward = base.NewMyWei(0)
			var feeReward = base.NewMyWei(0)
			var uncleReward = base.NewMyWei(0)

			sender := base.HexToAddress(raw.Address)
			bn := uint64(raw.BlockNumber)
			blockReward = conn.getBlockReward(bn)
			switch rt {
			case types.BlockReward:
				if block.Miner.Hex() == raw.Address {
					sender = base.BlockRewardSender
					nUncles := len(uncles)
					if nUncles > 0 {
						nephewReward = new(base.MyWei).Mul(blockReward, base.NewMyWei(int64(nUncles)))
						nephewReward.Div(nephewReward, base.NewMyWei(32))
					}
					for _, tx := range block.Transactions {
						gp := base.NewMyWei(int64(tx.GasPrice))
						gu := base.NewMyWei(int64(tx.Receipt.GasUsed))
						feeReward = feeReward.Add(feeReward, gp.Mul(gp, gu))
					}
				} else {
					blockReward = base.NewMyWei(0)
				}
			case types.UncleReward:
				for _, uncle := range uncles {
					if uncle.Miner.Hex() == raw.Address {
						sender = base.UncleRewardSender
						if bn < uncle.BlockNumber+6 {
							diff := (uncle.BlockNumber + 8 - bn) // positive since +6 < bn
							uncleReward = new(base.MyWei).Mul(blockReward, base.NewMyWei(int64(diff)))
							uncleReward.Div(uncleReward, base.NewMyWei(8))
						}
					}
				}
				if block.Miner.Hex() == raw.Address {
					sender = base.BlockRewardSender // if it's both, it's the block reward
					// The uncle miner may also have been the miner of the block
					if minerTx, err := conn.GetTransactionRewardByTypeAndApp(types.BlockReward, raw); err != nil {
						return nil, err
					} else {
						blockReward = &minerTx.Rewards.Block
						nephewReward = &minerTx.Rewards.Nephew
						feeReward = &minerTx.Rewards.TxFee
					}
				} else {
					blockReward = base.NewMyWei(0)
				}
			case types.NephewReward:
				fallthrough
			case types.TxFeeReward:
				fallthrough
			default:
				return nil, errors.New("invalid reward type")
			}

			rewards, total := types.NewReward(blockReward, nephewReward, feeReward, uncleReward)
			tx := &types.SimpleTransaction{
				BlockNumber:      uint64(raw.BlockNumber),
				TransactionIndex: uint64(raw.TransactionIndex),
				BlockHash:        block.Hash,
				Timestamp:        block.Timestamp,
				From:             sender,
				To:               base.HexToAddress(raw.Address),
				Value:            (big.Int)(total),
				Rewards:          &rewards,
			}
			return tx, nil
		}
	}
}

// GetTransactionCountInBlock returns the number of transactions in a block
func (conn *Connection) GetTransactionCountInBlock(bn uint64) (uint64, error) {
	if ec, err := conn.getClient(); err != nil {
		return 0, err
	} else {
		defer ec.Close()

		block, err := ec.BlockByNumber(context.Background(), base.BiFromUint64(bn))
		if err != nil {
			return 0, err
		}

		cnt, err := ec.TransactionCount(context.Background(), block.Hash())
		return uint64(cnt), err
	}
}

var (
	notAnInt = utils.NOPOS
	notAHash = base.Hash{}
)

func (conn *Connection) getTransactionRaw(blkHash base.Hash, txHash base.Hash, bn base.Blknum, txid uint64) (raw *types.RawTransaction, err error) {
	method := "eth_getTransactionByBlockNumberAndIndex"
	params := query.Params{fmt.Sprintf("0x%x", bn), fmt.Sprintf("0x%x", txid)}
	if txHash != notAHash {
		method = "eth_getTransactionByHash"
		params = query.Params{txHash.Hex()}
	} else if blkHash != notAHash {
		method = "eth_getTransactionByBlockHashAndIndex"
		params = query.Params{blkHash.Hex(), fmt.Sprintf("0x%x", txid)}
	}

	if trans, err := query.Query[types.RawTransaction](conn.Chain, method, params); err != nil {
		return &types.RawTransaction{}, err
	} else {
		if trans.AccessList == nil {
			trans.AccessList = make([]types.StorageSlot, 0)
		}
		return trans, nil
	}
}
