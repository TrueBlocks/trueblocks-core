package rpcClient

// // GetTransactionReceipt retrieves a SimpleReceipt from the node using the RPC's eth_getTransactionReceipt
// func GetTransactionReceipt(chain string, bn uint64, txid uint64) (receipt types.SimpleReceipt, err error) {
// 	// First, get raw receipt directly from RPC
// 	ethReceipt, err := getRawTransactionReceipt(chain, bn, txid)
// 	if err != nil {
// 		return
// 	}

// 	// Prepare logs of type []SimpleLog
// 	logs := []types.SimpleLog{}
// 	for _, log := range ethReceipt.Logs {
// 		logAddress := log.Address
// 		logIndex := log.Index
// 		logBlockNumber := log.BlockNumber
// 		logTxIndex := ethReceipt.TransactionIndex
// 		logTopics := make([]common.Hash, 0, len(log.Topics))
// 		logTopics = append(logTopics, log.Topics...)
// 		logs = append(logs, types.SimpleLog{
// 			Address:          logAddress,
// 			LogIndex:         uint64(logIndex),
// 			BlockNumber:      logBlockNumber,
// 			TransactionIndex: uint32(logTxIndex),
// 			Timestamp:        12, // FIXME -- causes import cycle - tslib.FromBnToTs(logBlockNumber),
// 			Topics:           logTopics,
// 			Data:             string(log.Data),
// 			CompressedLog:    "", // FIXME
// 		})
// 	}

// 	// Type-cast values that are not strings
// 	blockNumber := ethReceipt.BlockNumber
// 	cumulativeGasUsed := ethReceipt.CumulativeGasUsed
// 	gasUsed := ethReceipt.GasUsed
// 	status := ethReceipt.Status
// 	transactionIndex := ethReceipt.TransactionIndex
// 	receipt = types.SimpleReceipt{
// 		BlockHash:         ethReceipt.BlockHash,
// 		BlockNumber:       blockNumber.Uint64(),
// 		ContractAddress:   ethReceipt.ContractAddress,
// 		CumulativeGasUsed: fmt.Sprint(cumulativeGasUsed),
// 		GasUsed:           gasUsed,
// 		Logs:              logs,
// 		Status:            uint32(status),
// 		IsError:           status == 0,
// 		TransactionHash:   ethReceipt.TxHash,
// 		TransactionIndex:  uint64(transactionIndex),
// 		// From:
// 		// To:
// 		// EffectiveGasPrice
// 	}
// 	receipt.SetRaw(*ethReceipt)

// 	return
// }

// func getRawTransactionReceipt(chain string, bn uint64, txid uint64) (receipt *types.RawReceipt, err error) {
// 	tx, err := TxFromNumberAndId(chain, bn, txid)
// 	if err != nil {
// 		return
// 	}

// 	var response struct {
// 		Result types.RawReceipt `json:"result"`
// 	}
// 	err = FromRpc(
// 		config.GetRpcProvider(chain),
// 		&RPCPayload{
// 			Method:    "eth_getTransactionReceipt",
// 			RPCParams: RPCParams{tx.Hash().Hex()},
// 		},
// 		&response,
// 	)
// 	if err != nil {
// 		return
// 	}
// 	receipt = &response.Result

// 	return
// }
