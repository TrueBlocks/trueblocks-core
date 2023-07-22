package rpc

import (
	"context"
	"fmt"
	"math/big"
	"sync"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	ethTypes "github.com/ethereum/go-ethereum/core/types"
	"github.com/ethereum/go-ethereum/ethclient"
)

// import (
// 	"context"
// 	"log"
// 	"math/big"
// 	"sync"

// 	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
// 	"github.com/ethereum/go-ethereum/ethclient"
// )

// TODO: this looks weird, but before we were creating and deleting the client with every call which
// TODO: overran the number of TPC connection the OS would create (on a Mac). Since then, we now
// TODO: open the connection once and just use it allowing the operating system to clean it up
// TODO: DUPLICATED DUE TO CYCLICAL IMPORT
var perProviderClientMap = map[string]*ethclient.Client{}
var clientMutex sync.Mutex

func getClient(provider string) *ethclient.Client {
	clientMutex.Lock()
	if perProviderClientMap[provider] == nil {
		// TODO: I don't like the fact that we Dail In every time we want to us this
		// TODO: If we make this a cached item, it needs to be cached per chain, see timestamps
		ec, err := ethclient.Dial(provider)
		if err != nil || ec == nil {
			logger.Error("Missdial("+provider+"):", err)
			logger.Fatal("")
		}
		perProviderClientMap[provider] = ec
	}
	clientMutex.Unlock()
	return perProviderClientMap[provider]
}

// GetTxHashFromNumberAndId returns a transaction's hash if it's a valid transaction
func GetTxHashFromNumberAndId(chain string, blkNum, txId uint64) (string, error) {
	provider := config.GetRpcProvider(chain)
	ec := getClient(provider)
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

// TODO: DUPLICATED DUE TO CYCLICAL IMPORT

func GetTxFromNumberAndId(chain string, blkNum, txId uint64) (ethTypes.Transaction, error) {
	provider := config.GetRpcProvider(chain)
	ec := getClient(provider)
	defer ec.Close()

	block, err := ec.BlockByNumber(context.Background(), new(big.Int).SetUint64(blkNum))
	if err != nil {
		return ethTypes.Transaction{}, err
	}

	tx, err := ec.TransactionInBlock(context.Background(), block.Hash(), uint(txId))
	if err != nil {
		return ethTypes.Transaction{}, err
	}

	return *tx, nil
}

// GetBlockTimestamp returns the timestamp associated with a given block
func GetBlockTimestamp(chain string, bn *uint64) base.Timestamp {
	provider := config.GetRpcProvider(chain)
	ec := getClient(provider)
	defer ec.Close()

	var blockNumber *big.Int
	if bn != nil {
		blockNumber = big.NewInt(int64(*bn))
	}

	r, err := ec.HeaderByNumber(context.Background(), blockNumber)
	if err != nil {
		logger.Error("Could not connect to RPC client", err)
		return 0
	}

	ts := base.Timestamp(r.Time)
	if ts == 0 {
		// The RPC does not return a timestamp for block zero, so we simulate it with ts from block one less 13 seconds
		// TODO: Chain specific
		return GetBlockTimestamp(chain, utils.PointerOf(uint64(1))) - 13
	}

	return ts
}
