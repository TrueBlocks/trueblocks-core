package rpcClient

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/ethereum/go-ethereum/common"
	"github.com/ethereum/go-ethereum/common/hexutil"
	ethTypes "github.com/ethereum/go-ethereum/core/types"
)

// GetTransactionReceipt fetches receipt from the RPC. If txGasPrice is provided, it will be used for
// receipts in blocks before London
func GetTransactionReceipt(chain string, bn uint64, txid uint64, txHash *common.Hash, txGasPrice uint64) (receipt types.SimpleReceipt, err error) {
	// First, get raw receipt directly from RPC
	rawReceipt, _, err := getRawTransactionReceipt(chain, bn, txid, txHash)
	if err != nil {
		return
	}

	// Prepare logs of type []SimpleLog
	logs := []types.SimpleLog{}
	for _, log := range rawReceipt.Logs {
		logAddress := common.HexToAddress(log.Address)
		logIndex, parseErr := hexutil.DecodeUint64(log.LogIndex)
		if parseErr != nil {
			err = parseErr
			return
		}

		logBlockNumber, parseErr := hexutil.DecodeUint64(log.BlockNumber)
		if parseErr != nil {
			err = parseErr
			return
		}

		logTxIndex, parseErr := hexutil.DecodeUint64(log.TransactionIndex)
		if parseErr != nil {
			err = parseErr
			return
		}

		logTopics := make([]common.Hash, 0, len(log.Topics))
		for _, topic := range log.Topics {
			logTopics = append(logTopics, common.HexToHash(topic))
		}

		logs = append(logs, types.SimpleLog{
			Address:          logAddress,
			LogIndex:         logIndex,
			BlockNumber:      logBlockNumber,
			TransactionIndex: uint32(logTxIndex),
			Timestamp:        0, // TODO: FIXME
			Topics:           logTopics,
			Data:             string(log.Data),
			CompressedLog:    "", // TODO: FIXME
		})
	}

	// Type-cast values that are not strings
	blockNumber, err := hexutil.DecodeUint64(rawReceipt.BlockNumber)
	if err != nil {
		return
	}
	cumulativeGasUsed, err := hexutil.DecodeUint64(rawReceipt.CumulativeGasUsed)
	if err != nil {
		return
	}
	gasUsed, err := hexutil.DecodeUint64(rawReceipt.GasUsed)
	if err != nil {
		return
	}
	status, err := hexutil.DecodeUint64(rawReceipt.Status)
	if err != nil {
		return
	}
	transactionIndex, err := hexutil.DecodeUint64(rawReceipt.TransactionIndex)
	if err != nil {
		return
	}

	receipt = types.SimpleReceipt{
		BlockHash:         common.HexToHash(rawReceipt.BlockHash),
		BlockNumber:       blockNumber,
		ContractAddress:   common.HexToAddress(rawReceipt.ContractAddress),
		CumulativeGasUsed: fmt.Sprint(cumulativeGasUsed),
		GasUsed:           gasUsed,
		Logs:              logs,
		Status:            uint32(status),
		IsError:           status == 0,
		TransactionHash:   common.HexToHash(rawReceipt.TransactionHash),
		TransactionIndex:  transactionIndex,
		// From:
		// To:
		// EffectiveGasPrice
	}
	receipt.SetRaw(*rawReceipt)

	return
}

// getRawTransactionReceipt fetches raw transaction. If txHash is provided, the function
// will not fetch the transaction (we may have already loaded it)
func getRawTransactionReceipt(chain string, bn uint64, txid uint64, txHash *common.Hash) (receipt *types.RawReceipt, tx ethTypes.Transaction, err error) {
	var txHashString string
	if txHash != nil {
		txHashString = txHash.Hex()
	} else {
		tx, err = TxFromNumberAndId(chain, bn, txid)
		if err != nil {
			return
		}
		txHashString = tx.Hash().Hex()
	}

	var response struct {
		Result types.RawReceipt `json:"result"`
	}
	err = FromRpc(
		config.GetRpcProvider(chain),
		&RPCPayload{
			Method:    "eth_getTransactionReceipt",
			RPCParams: RPCParams{txHashString},
		},
		&response,
	)
	if err != nil {
		return
	}
	receipt = &response.Result

	return
}
