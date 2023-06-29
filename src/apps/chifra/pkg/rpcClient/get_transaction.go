package rpcClient

import (
	"errors"
	"fmt"
	"math/big"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
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

// These purposefully chosen baddresses are used to indicate that the transaction is a prefund
// and uncle reward, or a mining reward. They are not real addresses, but are used to indicate
// that the transaction is not a normal transaction. They are not (currently) indexed.
var (
	PrefundSender     = base.HexToAddress("0x0000000000000000000000000050726566756e64") // The word "Prefund" in hex
	BlockRewardSender = base.HexToAddress("0x0000000000000000000000000000004d696e6572") // The word "Miner" in hex
	UncleRewardSender = base.HexToAddress("0x000000000000000000000000000000556e636c65") // The word "Uncle" in hex
)

func GetPrefundTxByApp(chain string, appearance *types.RawAppearance) (tx *types.SimpleTransaction, err error) {
	prefundPath := prefunds.GetPrefundPath(chain)
	if prefundMap, err := prefunds.LoadPrefundMap(chain, prefundPath); err != nil {
		return nil, err
	} else {
		var blockHash base.Hash
		var ts int64
		if block, err := GetBlockByNumber(chain, uint64(0)); err != nil {
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
				From:             PrefundSender,
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

func getReward(chain string, rt RewardType, block *types.SimpleBlock[types.SimpleTransaction]) big.Int {
	byzantiumBlock := uint64(4370000)
	constantinopleBlock := uint64(7280000)
	bn := block.BlockNumber
	if bn == 0 {
		return *big.NewInt(0)
	}

	switch rt {
	case BLOCK_REWARD:
		if bn < byzantiumBlock {
			return *big.NewInt(5000000000000000000)
		} else if bn < constantinopleBlock {
			return *big.NewInt(3000000000000000000)
		} else {
			return *big.NewInt(2000000000000000000)
		}
	case NEPHEW_REWARD:
		//     blknum_t nUncles = getUncleCount(bn);
		//     if (nUncles)
		//         reward += ((getBlockReward(bn) * nUncles) / 32);
		//     return reward;
		// }
		return *big.NewInt(3000000000000000000)
	case UNCLE_REWARD:
		//     wei_t reward = getBlockReward(bn);
		//     if ((uncleBn + 6) < bn)
		//         return 0;
		//     blknum_t diff = (uncleBn + 8 - bn);
		//     return (reward / 8 * diff);
		// }
		return *big.NewInt(2000000000000000000)
	case TXFEE_REWARD:
		//     CBlock block;
		//     getBlock(block, bn);
		//     for (auto tx : block.transactions)
		//         fees += (tx.receipt.gasUsed * tx.gasPrice);
		//     return fees;
		// }
		return *big.NewInt(0)
	default:
		return *big.NewInt(0)
	}
}

func GetBlockRewardTxByApp(chain string, appearance *types.RawAppearance) (*types.SimpleTransaction, error) {
	if block, err := GetBlockByNumberWithTxs(chain, uint64(appearance.BlockNumber)); err != nil {
		return nil, err
	} else {
		tx := &types.SimpleTransaction{
			BlockNumber:      uint64(appearance.BlockNumber),
			BlockHash:        block.Hash,
			TransactionIndex: uint64(appearance.TransactionIndex),
			Timestamp:        block.Timestamp,
			From:             BlockRewardSender,
			To:               base.HexToAddress(appearance.Address),
			Value:            getReward(chain, BLOCK_REWARD, &block),
			ExtraValue1:      getReward(chain, NEPHEW_REWARD, &block),
			ExtraValue2:      getReward(chain, TXFEE_REWARD, &block),
		}
		return tx, nil
	}
}

func GetUncleRewardTxByApp(chain string, appearance *types.RawAppearance) (tx *types.SimpleTransaction, err error) {
	if uncles, err := GetUnclesByNumber(chain, uint64(appearance.BlockNumber)); err != nil {
		return nil, err
	} else {
		tx = &types.SimpleTransaction{
			BlockNumber:      uint64(appearance.BlockNumber),
			TransactionIndex: uint64(appearance.TransactionIndex),
			From:             UncleRewardSender,
			To:               base.HexToAddress(appearance.Address),
		}
		for _, uncle := range uncles {
			if uncle.Miner.Hex() == appearance.Address {
				r := getReward(chain, UNCLE_REWARD, &uncle)
				tx.Value.Add(&tx.Value, &r)
			}
		}
		return tx, nil
	}
}

func GetTransactionByAppearance(chain string, appearance *types.RawAppearance, fetchTraces bool) (tx *types.SimpleTransaction, err error) {
	bn := uint64(appearance.BlockNumber)
	txid := uint64(appearance.TransactionIndex)

	if bn == 0 {
		return GetPrefundTxByApp(chain, appearance)
	} else if txid == 99999 || txid == 99997 || txid == 99996 {
		return GetBlockRewardTxByApp(chain, appearance)
	} else if txid == 99998 {
		return GetUncleRewardTxByApp(chain, appearance)
	}

	blockTs := rpc.GetBlockTimestamp(chain, bn)
	receipt, err := GetTransactionReceipt(chain, ReceiptQuery{
		Bn:      bn,
		Txid:    txid,
		NeedsTs: true,
		Ts:      blockTs,
	})
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

	if fetchTraces {
		traces, err := GetTracesByTransactionHash(chain, tx.Hash.Hex(), tx)
		if err != nil {
			return nil, err
		}
		tx.Traces = traces
	}

	return
}
