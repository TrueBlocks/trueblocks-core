package rpcClient

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
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

// func GetPrefundByAppearance(chain string, appearance *types.RawAppearance) (tx *types.SimpleTransaction, err error) {
// 	alloc, err := prefunds.GetPrefundAt(chain, appearance.Address, appearance.TransactionIndex)
// 	if err != nil {
// 		return
// 	}

// 	return &types.SimpleTransaction{
// 		BlockNumber:      uint64(appearance.BlockNumber),
// 		TransactionIndex: uint64(appearance.TransactionIndex),
// 		From:             base.HexToAddress("0xfedcba9876543210123456789abcdefdeadbeef"),
// 		To:               base.HexToAddress(appearance.Address),
// 		Value:            alloc.Amount,
// 	}, nil
// }

// var FakeBlockRewardSender = base.HexToAddress("0xdeadbeefdeadbeefdeadbeefdeadbeefdeadbeef")
// var FakeUncleRewardSender = base.HexToAddress("0xdeadbeeedeadbeeedeadbeeedeadbeeedeadbeee")

// func GetMinerRewardByAppearance(chain string, appearance *types.RawAppearance) (tx *types.SimpleTransaction, err error) {
// 	block, err := GetBlockByNumber(chain, uint64(appearance.BlockNumber))
// 	if err != nil {
// 		return
// 	}

// 	reward := big.Int{}
// 	reward.SetString("5000000000000000000", 10)
// 	return &types.SimpleTransaction{
// 		BlockNumber:      uint64(appearance.BlockNumber),
// 		TransactionIndex: uint64(appearance.TransactionIndex),
// 		BlockHash:        block.Hash,
// 		From:             FakeBlockRewardSender,
// 		To:               base.HexToAddress(appearance.Address),
// 		Value:            reward,
// 	}, nil
// }

// func GetUncleRewardByAppearance(chain string, appearance *types.RawAppearance) (tx *types.SimpleTransaction, err error) {
// 	reward := big.Int{}
// 	reward.SetString("3500000000000000000", 10)
// 	return &types.SimpleTransaction{
// 		BlockNumber:      uint64(appearance.BlockNumber),
// 		TransactionIndex: uint64(appearance.TransactionIndex),
// 		From:             FakeUncleRewardSender,
// 		To:               base.HexToAddress(appearance.Address),
// 		Value:            reward,
// 	}, nil
// }

/*
if (blk == 0) {
	address_t addr = opt->prefundAddrMap[trav->app->txid];
	trav->trans.loadTransAsPrefund(trav->app->blk, trav->app->txid, addr, prefundAt(addr));
    blockNumber = bn;
    transactionIndex = txid;
    from = "0xPrefund";
    to = addr;
    value = amount;
    return true;
} else if (trav->app->txid == 99996 || trav->app->txid == 99997 || trav->app->txid == 99999) {
	trav->trans.loadTransAsBlockReward(trav->app->blk, trav->app->txid, opt->blkRewardMap[trav->app->blk]);
    blockNumber = bn;
    transactionIndex = txid;
    from = "0xBlockReward";
    to = addr;
    value = getBlock Reward2(bn);
    extraValue1 = getNephew Reward(bn);
    extraValue2 = getTrans Fees(bn);  // weird temp value for reconciliation only
    receipt = CReceipt();
    receipt.pTransaction = this;
    return true;
} else if (trav->app->txid == 99998) {
	uint64_t nUncles = getUncleCount(trav->app->blk);
	for (size_t u = 0; u < nUncles; u++) {
		CBlock uncle;
		getUncle(uncle, trav->app->blk, u);
		if (uncle.miner == opt->blkRewardMap[trav->app->blk]) {
			trav->trans.loadTransAsUncleReward(trav->app->blk, uncle.blockNumber, uncle.miner);
			from = "0xUncleReward";
			to = addr;
			value += getUncle Reward(bn, uncleBn);  // we use += here because you can win more than one uncle block per block
			receipt = CReceipt();
			receipt.pTransaction = this;
			return true;
		}
	}
} else {
	getTransaction(trav->trans, trav->app->blk, trav->app->txid);
	getFullReceipt(&trav->trans, true);
}
*/

func GetTransactionByAppearance(chain string, appearance *types.RawAppearance, fetchTraces bool) (tx *types.SimpleTransaction, err error) {
	bn := uint64(appearance.BlockNumber)
	txid := uint64(appearance.TransactionIndex)

	// if bn == 0 {
	// 	return GetPrefundByAppearance(chain, appearance)
	// } else if txid == 99999 || txid == 99997 || txid == 99996 {
	// 	return GetMinerRewardByAppearance(chain, appearance)
	// } else if txid == 99998 {
	// 	return GetUncleRewardByAppearance(chain, appearance)
	// }

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
