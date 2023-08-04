package rpcClient

import (
	"errors"
	"fmt"
	"math/big"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/prefunds"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

var (
	notAnInt = utils.NOPOS
	notAHash = base.Hash{}
)

func (conn *Connection) getRawTransaction(blkHash base.Hash, txHash base.Hash, bn base.Blknum, txid uint64) (raw *types.RawTransaction, err error) {
	method := "eth_getTransactionByBlockNumberAndIndex"
	params := rpc.Params{fmt.Sprintf("0x%x", bn), fmt.Sprintf("0x%x", txid)}
	if txHash != notAHash {
		method = "eth_getTransactionByHash"
		params = rpc.Params{txHash.Hex()}
	} else if blkHash != notAHash {
		method = "eth_getTransactionByBlockHashAndIndex"
		params = rpc.Params{blkHash.Hex(), fmt.Sprintf("0x%x", txid)}
	}

	if trans, err := rpc.Query[types.RawTransaction](conn.Chain, method, params); err != nil {
		return &types.RawTransaction{}, err
	} else {
		if trans.AccessList == nil {
			trans.AccessList = make([]types.StorageSlot, 0)
		}
		return trans, nil
	}
}

func (conn *Connection) GetAppearanceFromHash(hash string) (types.RawAppearance, error) {
	var ret types.RawAppearance
	if rawTx, err := conn.getRawTransaction(notAHash, base.HexToHash(hash), notAnInt, notAnInt); err != nil {
		return ret, err
	} else {
		ret.BlockNumber = uint32(utils.MustParseUint(rawTx.BlockNumber))
		ret.TransactionIndex = uint32(utils.MustParseUint(rawTx.TransactionIndex))
		return ret, nil
	}
}

func (conn *Connection) GetPrefundTxByApp(appearance *types.RawAppearance) (tx *types.SimpleTransaction, err error) {
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

		entry := (*prefundMap)[base.HexToAddress(appearance.Address)]
		if entry.Address.Hex() == appearance.Address {
			ret := types.SimpleTransaction{
				BlockHash:        blockHash,
				BlockNumber:      uint64(appearance.BlockNumber),
				TransactionIndex: uint64(appearance.TransactionIndex),
				Timestamp:        ts,
				From:             base.PrefundSender,
				To:               base.HexToAddress(appearance.Address),
				Value:            entry.Prefund,
			}
			return &ret, nil
		}
	}
	return nil, errors.New("not found")
}

type RewardType int

const (
	BLOCK_REWARD RewardType = iota
	NEPHEW_REWARD
	UNCLE_REWARD
	TXFEE_REWARD
)

// // This data was taken from Geth ./params/config.go
// blknum_t hardForkBlock(const string_q& hf) {
//     if (hf == "byzantium") {
//         map<string_q, blknum_t> theMap = {
//             make_pair("mainnet", 4370000),
//         };
//         return theMap[getChain()];
//     } else if (hf == "constantinople") {
//         map<string_q, blknum_t> theMap = {
//             make_pair("mainnet", 7280000),
//         };
//         return theMap[getChain()];
//     } else if (hf == "london") {
//         map<string_q, blknum_t> theMap = {
//             make_pair("goerli", 5062605),
//             make_pair("mainnet", 12965000),
//         };
//         return theMap[getChain()];
//     }
//     return 0;
// }

const (
	byzantiumBlock      = uint64(4370000)
	constantinopleBlock = uint64(7280000)
)

func getBlockReward(bn uint64) *big.Int {
	if bn == 0 {
		return big.NewInt(0)
	} else if bn < byzantiumBlock {
		return big.NewInt(5000000000000000000)
	} else if bn < constantinopleBlock {
		return big.NewInt(3000000000000000000)
	} else {
		return big.NewInt(2000000000000000000)
	}
}

// TODO: This is not cross-chain correct

func (conn *Connection) GetRewardTxByTypeAndApp(rt RewardType, appearance *types.RawAppearance) (*types.SimpleTransaction, error) {
	if block, err := conn.GetBlockBodyByNumber(uint64(appearance.BlockNumber)); err != nil {
		return nil, err
	} else {
		if uncles, err := conn.GetUnclesByNumber(uint64(appearance.BlockNumber)); err != nil {
			return nil, err
		} else {
			var blockReward = big.NewInt(0)
			var nephewReward = big.NewInt(0)
			var feeReward = big.NewInt(0)
			var uncleReward = big.NewInt(0)

			sender := base.HexToAddress(appearance.Address)
			bn := uint64(appearance.BlockNumber)
			blockReward = getBlockReward(bn)
			switch rt {
			case BLOCK_REWARD:
				if block.Miner.Hex() == appearance.Address {
					sender = base.BlockRewardSender
					nUncles := len(uncles)
					if nUncles > 0 {
						nephewReward = new(big.Int).Mul(blockReward, big.NewInt(int64(nUncles)))
						nephewReward.Div(nephewReward, big.NewInt(32))
					}
					for _, tx := range block.Transactions {
						gp := big.NewInt(int64(tx.GasPrice))
						gu := big.NewInt(int64(tx.Receipt.GasUsed))
						feeReward.Add(feeReward, gp.Mul(gp, gu))
					}
				} else {
					blockReward = big.NewInt(0)
				}
			case UNCLE_REWARD:
				for _, uncle := range uncles {
					if uncle.Miner.Hex() == appearance.Address {
						sender = base.UncleRewardSender
						if bn < uncle.BlockNumber+6 {
							diff := (uncle.BlockNumber + 8 - bn) // positive since +6 < bn
							uncleReward = new(big.Int).Mul(blockReward, big.NewInt(int64(diff)))
							uncleReward.Div(uncleReward, big.NewInt(8))
						}
					}
				}
				if block.Miner.Hex() == appearance.Address {
					sender = base.BlockRewardSender // if it's both, it's the block reward
					// The uncle miner may also have been the miner of the block
					if minerTx, err := conn.GetRewardTxByTypeAndApp(BLOCK_REWARD, appearance); err != nil {
						return nil, err
					} else {
						blockReward = &minerTx.Rewards.Block
						nephewReward = &minerTx.Rewards.Nephew
						feeReward = &minerTx.Rewards.TxFee
					}
				} else {
					blockReward = big.NewInt(0)
				}
			case NEPHEW_REWARD:
				fallthrough
			case TXFEE_REWARD:
				fallthrough
			default:
				return nil, errors.New("invalid reward type")
			}

			rewards, total := types.NewReward(blockReward, nephewReward, feeReward, uncleReward)
			tx := &types.SimpleTransaction{
				BlockNumber:      uint64(appearance.BlockNumber),
				BlockHash:        block.Hash,
				TransactionIndex: uint64(appearance.TransactionIndex),
				Timestamp:        block.Timestamp,
				From:             sender,
				To:               base.HexToAddress(appearance.Address),
				Value:            total,
				Rewards:          &rewards,
			}
			return tx, nil
		}
	}
}

func (conn *Connection) GetTransactionByAppearance(appearance *types.RawAppearance, fetchTraces bool) (tx *types.SimpleTransaction, err error) {
	bn := uint64(appearance.BlockNumber)
	txid := uint64(appearance.TransactionIndex)

	if conn.HasStore() {
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

	var writeOptions *cache.WriteOptions
	var blockTs base.Timestamp
	if conn.HasStoreWritable() {
		blockTs = conn.GetBlockTimestamp(&bn)
		writeOptions = &cache.WriteOptions{
			// Check if the block is final
			Pending: (&types.SimpleBlock[string]{Timestamp: blockTs}).Pending(conn.LatestBlockTimestamp),
		}
	}

	tx = nil
	if bn == 0 {
		if tx, err = conn.GetPrefundTxByApp(appearance); err != nil {
			return nil, err
		}
	} else if txid == 99999 || txid == 99997 || txid == 99996 {
		if tx, err = conn.GetRewardTxByTypeAndApp(BLOCK_REWARD, appearance); err != nil {
			return nil, err
		}
	} else if txid == 99998 {
		if tx, err = conn.GetRewardTxByTypeAndApp(UNCLE_REWARD, appearance); err != nil {
			return nil, err
		}
	}
	if tx != nil {
		if conn.HasStore() && conn.enabledMap["txs"] {
			_ = conn.Store.Write(tx, writeOptions)
		}
		return tx, nil
	}

	blockTs = conn.GetBlockTimestamp(&bn)
	receipt, err := conn.GetReceipt(ReceiptQuery{
		Bn:      bn,
		Txid:    txid,
		NeedsTs: true,
		Ts:      blockTs,
	})
	if err != nil {
		return
	}

	rawTx, err := conn.getRawTransaction(notAHash, notAHash, bn, txid)
	if err != nil {
		return
	}

	tx = types.NewSimpleTransaction(rawTx, &receipt, blockTs)

	if conn.HasStore() && conn.enabledMap["txs"] {
		_ = conn.Store.Write(tx, writeOptions)
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

func (conn *Connection) GetTransactionByBlockAndId(bn base.Blknum, txid uint64) (tx *types.SimpleTransaction, err error) {
	if conn.HasStore() {
		tx = &types.SimpleTransaction{
			BlockNumber:      bn,
			TransactionIndex: txid,
		}

		if err := conn.Store.Read(tx, nil); err == nil {
			// success
			return tx, nil
		}
	}

	rawTx, err := conn.getRawTransaction(notAHash, notAHash, bn, txid)
	if err != nil {
		return
	}
	blockTs := conn.GetBlockTimestamp(&bn)

	var writeOptions *cache.WriteOptions
	if conn.HasStoreWritable() {
		writeOptions = &cache.WriteOptions{
			// Check if the block is final
			Pending: (&types.SimpleBlock[string]{Timestamp: blockTs}).Pending(conn.LatestBlockTimestamp),
		}
	}

	receipt, err := conn.GetReceipt(ReceiptQuery{
		Bn:      bn,
		Txid:    txid,
		NeedsTs: true,
		Ts:      blockTs,
	})
	if err != nil {
		return
	}

	tx = types.NewSimpleTransaction(rawTx, &receipt, blockTs)

	if conn.HasStore() && conn.enabledMap["txs"] {
		_ = conn.Store.Write(tx, writeOptions)
	}

	return
}
