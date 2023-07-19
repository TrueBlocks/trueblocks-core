package rpcClient

import (
	"errors"
	"fmt"
	"math/big"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cacheNew"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/prefunds"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

var (
	notAnInt = utils.NOPOS
	notAHash = base.Hash{}
)

func getRawTransaction(chain string, blkHash base.Hash, txHash base.Hash, bn base.Blknum, txid uint64) (raw *types.RawTransaction, err error) {
	method := "eth_getTransactionByBlockNumberAndIndex"
	params := rpc.Params{fmt.Sprintf("0x%x", bn), fmt.Sprintf("0x%x", txid)}
	if txHash != notAHash {
		method = "eth_getTransactionByHash"
		params = rpc.Params{txHash.Hex()}
	} else if blkHash != notAHash {
		method = "eth_getTransactionByBlockHashAndIndex"
		params = rpc.Params{blkHash.Hex(), fmt.Sprintf("0x%x", txid)}
	}

	if trans, err := rpc.Query[types.RawTransaction](chain, method, params); err != nil {
		return &types.RawTransaction{}, err
	} else {
		if trans.AccessList == nil {
			trans.AccessList = make([]types.StorageSlot, 0)
		}
		return trans, nil
	}
}

func GetRawTransactionByHashAndId(chain string, blkHash base.Hash, txid uint64) (raw *types.RawTransaction, err error) {
	return getRawTransaction(chain, blkHash, notAHash, notAnInt, txid)
}

func GetRawTransactionByHash(chain string, txHash base.Hash) (raw *types.RawTransaction, err error) {
	return getRawTransaction(chain, notAHash, txHash, notAnInt, notAnInt)
}

func GetRawTransactionByBlockAndId(chain string, bn base.Blknum, txid uint64) (raw *types.RawTransaction, err error) {
	return getRawTransaction(chain, notAHash, notAHash, bn, txid)
}

func GetAppearanceFromHash(chain string, hash string) (uint64, uint64, error) {
	if rawTx, err := GetRawTransactionByHash(chain, base.HexToHash(hash)); err != nil {
		return 0, 0, err
	} else {
		return mustParseUint(rawTx.BlockNumber), mustParseUint(rawTx.TransactionIndex), nil
	}
}

func GetPrefundTxByApp(chain string, appearance *types.RawAppearance) (tx *types.SimpleTransaction, err error) {
	// TODO: performance - This loads and then drops the file every time it's called. Quite slow.
	// TODO: performance - in the old C++ we stored these values in a pre fundAddrMap so that given a txid in block zero
	// TODO: performance - we knew which address was granted allocation at that transaction.
	prefundPath := prefunds.GetPrefundPath(chain)
	if prefundMap, err := prefunds.LoadPrefundMap(chain, prefundPath); err != nil {
		return nil, err
	} else {
		var blockHash base.Hash
		var ts int64
		if block, err := GetBlockByNumber(chain, uint64(0), nil); err != nil {
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

func GetRewardTxByTypeAndApp(chain string, rt RewardType, appearance *types.RawAppearance) (*types.SimpleTransaction, error) {
	if block, err := GetBlockByNumberWithTxs(chain, uint64(appearance.BlockNumber), nil); err != nil {
		return nil, err
	} else {
		if uncles, err := GetUnclesByNumber(chain, uint64(appearance.BlockNumber)); err != nil {
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
					if minerTx, err := GetRewardTxByTypeAndApp(chain, BLOCK_REWARD, appearance); err != nil {
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

func GetTransactionByAppearance(chain string, appearance *types.RawAppearance, fetchTraces bool, store *cacheNew.Store) (tx *types.SimpleTransaction, err error) {
	bn := uint64(appearance.BlockNumber)
	txid := uint64(appearance.TransactionIndex)

	if store != nil {
		tx = &types.SimpleTransaction{
			BlockNumber:      bn,
			TransactionIndex: txid,
		}

		if err := store.Read(tx, nil); err == nil {
			// success
			if fetchTraces {
				traces, err := GetTracesByTransactionHash(chain, tx.Hash.Hex(), tx, store)
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
		if tx, err = GetPrefundTxByApp(chain, appearance); err != nil {
			return nil, err
		}
	} else if txid == 99999 || txid == 99997 || txid == 99996 {
		if tx, err = GetRewardTxByTypeAndApp(chain, BLOCK_REWARD, appearance); err != nil {
			return nil, err
		}
	} else if txid == 99998 {
		if tx, err = GetRewardTxByTypeAndApp(chain, UNCLE_REWARD, appearance); err != nil {
			return nil, err
		}
	}
	if tx != nil {
		if store != nil {
			store.Write(tx, nil)
		}
		return tx, nil
	}

	blockTs := rpc.GetBlockTimestamp(chain, bn)
	receipt, err := GetTransactionReceipt(chain, ReceiptQuery{
		Bn:      bn,
		Txid:    txid,
		NeedsTs: true,
		Ts:      blockTs,
	}, cacheNew.NoCache)
	if err != nil {
		return
	}

	rawTx, err := GetRawTransactionByBlockAndId(chain, bn, txid)
	if err != nil {
		return
	}

	tx = &types.SimpleTransaction{
		Hash:             base.HexToHash(rawTx.Hash),
		BlockHash:        base.HexToHash(rawTx.BlockHash),
		BlockNumber:      bn,
		TransactionIndex: txid,
		Nonce:            mustParseUint(rawTx.Nonce),
		Timestamp:        blockTs,
		From:             base.HexToAddress(rawTx.From),
		To:               base.HexToAddress(rawTx.To),
		Gas:              mustParseUint(rawTx.Gas),
		GasPrice:         mustParseUint(rawTx.GasPrice),
		GasUsed:          receipt.GasUsed,
		Input:            rawTx.Input,
		IsError:          receipt.IsError,
		HasToken:         IsTokenRelated(rawTx.Input),
		Receipt:          &receipt,
	}
	tx.Value.SetString(rawTx.Value, 0)
	tx.SetGasCost(&receipt)
	tx.SetRaw(rawTx)

	if store != nil {
		store.Write(tx, nil)
	}

	if fetchTraces {
		traces, err := GetTracesByTransactionHash(chain, tx.Hash.Hex(), tx, store)
		if err != nil {
			return nil, err
		}
		tx.Traces = traces
	}

	return
}

func GetTransactionByBlockAndId(chain string, bn base.Blknum, txid uint64, store *cacheNew.Store) (tx *types.SimpleTransaction, err error) {
	if store != nil {
		tx = &types.SimpleTransaction{
			BlockNumber:      bn,
			TransactionIndex: txid,
		}

		if err := store.Read(tx, nil); err == nil {
			// success
			return tx, nil
		}
	}

	rawTx, err := GetRawTransactionByBlockAndId(chain, bn, txid)
	if err != nil {
		return
	}
	blockTs := rpc.GetBlockTimestamp(chain, bn)
	receipt, err := GetTransactionReceipt(chain, ReceiptQuery{
		Bn:      bn,
		Txid:    txid,
		NeedsTs: true,
		Ts:      blockTs,
	}, cacheNew.NoCache)
	if err != nil {
		return
	}

	// TODO: this gets repeated
	tx = &types.SimpleTransaction{
		Hash:             base.HexToHash(rawTx.Hash),
		BlockHash:        base.HexToHash(rawTx.BlockHash),
		BlockNumber:      bn,
		TransactionIndex: txid,
		Nonce:            mustParseUint(rawTx.Nonce),
		Timestamp:        blockTs,
		From:             base.HexToAddress(rawTx.From),
		To:               base.HexToAddress(rawTx.To),
		Gas:              mustParseUint(rawTx.Gas),
		GasPrice:         mustParseUint(rawTx.GasPrice),
		GasUsed:          receipt.GasUsed,
		Input:            rawTx.Input,
		IsError:          receipt.IsError,
		HasToken:         IsTokenRelated(rawTx.Input),
		Receipt:          &receipt,
	}
	tx.Value.SetString(rawTx.Value, 0)
	tx.SetGasCost(&receipt)
	tx.SetRaw(rawTx)

	if store != nil {
		store.Write(tx, nil)
	}

	return
}
