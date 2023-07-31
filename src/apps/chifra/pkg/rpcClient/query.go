package rpcClient

import (
	"context"
	"fmt"
	"math/big"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
)

// GetTransactionHashByNumberAndID returns a transaction's hash if it's a valid transaction
func GetTransactionHashByNumberAndID(chain string, blkNum, txId uint64) (string, error) {
	provider, _ := config.GetRpcProvider(chain)
	ec, _ := GetClient(provider)
	defer ec.Close()

	block, err := ec.BlockByNumber(context.Background(), new(big.Int).SetUint64(blkNum))
	if err != nil {
		return "", fmt.Errorf("error at block %s: %w", fmt.Sprintf("%d", blkNum), err)
	}

	tx, err := ec.TransactionInBlock(context.Background(), block.Hash(), uint(txId))
	if err != nil {
		return "", fmt.Errorf("transaction at %s returned an error: %w", fmt.Sprintf("%s.%d", block.Hash(), txId), err)
	}

	return tx.Hash().Hex(), nil
}
