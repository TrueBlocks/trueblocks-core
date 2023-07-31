// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package rpcClient

import (
	"context"
	"fmt"
	"math/big"
	"strings"
	"sync"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/ethereum/go-ethereum/common"
	"github.com/ethereum/go-ethereum/common/hexutil"
	"github.com/ethereum/go-ethereum/ethclient"
)

// TODO: this looks weird, but before we were creating and deleting the client with every call which
// TODO: overran the number of TPC connection the OS would create (on a Mac). Since then, we now
// TODO: open the connection once and just use it allowing the operating system to clean it up
var perProviderClientMap = map[string]*ethclient.Client{}
var clientMutex sync.Mutex

func GetClient(provider string) (*ethclient.Client, error) {
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
	return perProviderClientMap[provider], nil
}

// GetLatestBlockNumber returns the block number at the front of the chain (i.e. latest)
func GetLatestBlockNumber(chain string) uint64 {
	if provider, err := config.GetRpcProvider(chain); err != nil {
		logger.Info("Shit")
		return 0
	} else {
		ec, _ := GetClient(provider)
		defer ec.Close()

		r, err := ec.BlockNumber(context.Background())
		if err != nil {
			logger.Error("Could not connect to RPC client")
			return 0
		}

		return r
	}
}

var noProvider string = `

  {R}Warning{O}: The RPC server ([{PROVIDER}]) was not available. Either start it, or edit the {T}rpcProvider{O}
  value in the file {T}[{FILE}]{O}. Quitting...
`

// GetClientIDs returns both chainId and networkId from the node
func GetClientIDs(provider string) (uint64, uint64, error) {
	// We might need it, so create it
	msg := noProvider
	msg = strings.Replace(msg, "[{PROVIDER}]", provider, -1)
	msg = strings.Replace(msg, "[{FILE}]", config.GetPathToRootConfig()+"trueBlocks.toml", -1)
	msg = strings.Replace(msg, "{R}", colors.Red, -1)
	msg = strings.Replace(msg, "{T}", colors.Cyan, -1)
	msg = strings.Replace(msg, "{O}", colors.Off, -1)
	if provider == "https://" {
		msg = strings.Replace(msg, "https://", "<empty>", -1)
		return 0, 0, fmt.Errorf("%s", msg)
	}

	ec, _ := GetClient(provider)
	defer ec.Close()

	ch, err := ec.ChainID(context.Background())
	if err != nil {
		return 0, 0, err
	}

	n, err := ec.NetworkID(context.Background())
	if err != nil {
		return 0, 0, err
	}

	return ch.Uint64(), n.Uint64(), nil
}

// TODO: C++ code used to cache version info

func GetClientVersion(chain string) (version string, err error) {
	method := "web3_clientVersion"
	params := rpc.Params{}

	if version, err := rpc.Query[string](chain, method, params); err != nil {
		return "", err
	} else {
		return *version, nil
	}
}

// TxHashFromHash returns a transaction's hash if it's a valid transaction
func TxHashFromHash(provider, hash string) (string, error) {
	ec, _ := GetClient(provider)
	defer ec.Close()

	tx, _, err := ec.TransactionByHash(context.Background(), common.HexToHash(hash))
	if err != nil {
		return "", err
	}

	return tx.Hash().Hex(), nil
}

// TxHashFromHashAndId returns a transaction's hash if it's a valid transaction
func TxHashFromHashAndId(provider, hash string, txId uint64) (string, error) {
	ec, _ := GetClient(provider)
	defer ec.Close()

	tx, err := ec.TransactionInBlock(context.Background(), common.HexToHash(hash), uint(txId))
	if err != nil {
		return "", err
	}

	return tx.Hash().Hex(), nil
}

// GetCountTransactionsInBlock returns the number of transactions in a block
func GetCountTransactionsInBlock(chain string, blkNum uint64) (uint64, error) {
	provider, _ := config.GetRpcProvider(chain)
	ec, _ := GetClient(provider)
	defer ec.Close()

	block, err := ec.BlockByNumber(context.Background(), new(big.Int).SetUint64(blkNum))
	if err != nil {
		return 0, err
	}

	cnt, err := ec.TransactionCount(context.Background(), block.Hash())
	return uint64(cnt), err
}

// BlockHashFromHash returns a block's hash if it's a valid block
func BlockHashFromHash(provider, hash string) (string, error) {
	ec, _ := GetClient(provider)
	defer ec.Close()

	block, err := ec.BlockByHash(context.Background(), common.HexToHash(hash))
	if err != nil {
		return "", err
	}

	return block.Hash().Hex(), nil
}

// GetBlockNumberByHash returns a block's hash if it's a valid block
func GetBlockNumberByHash(chain, hash string) (uint64, error) {
	if provider, err := config.GetRpcProvider(chain); err != nil {
		return 0, err
	} else {
		ec, _ := GetClient(provider)
		defer ec.Close()

		block, err := ec.BlockByHash(context.Background(), common.HexToHash(hash))
		if err != nil {
			return 0, err
		}

		return block.NumberU64(), nil
	}
}

// GetBlockHashByNumber returns a block's hash if it's a valid block
func GetBlockHashByNumber(chain string, blkNum uint64) (string, error) {
	if provider, err := config.GetRpcProvider(chain); err != nil {
		return "", err
	} else {
		ec, _ := GetClient(provider)
		defer ec.Close()

		block, err := ec.BlockByNumber(context.Background(), new(big.Int).SetUint64(blkNum))
		if err != nil {
			return "", err
		}

		return block.Hash().Hex(), nil
	}
}

// GetBalanceAt returns a balance for an address at a block
func GetBalanceAt(chain string, addr base.Address, blkNum uint64) (*big.Int, error) {
	provider, _ := config.GetRpcProvider(chain)
	ec, _ := GetClient(provider)
	defer ec.Close()

	bal, err := ec.BalanceAt(context.Background(), addr.ToCommon(), new(big.Int).SetUint64(blkNum))
	if err != nil {
		return bal, err
	}

	return bal, nil
}

/*
// Functions available in the client
func NewClient(c *rpc.Client) *Client
func (ec *Client) BalanceAt(ctx context.Context, account commo n.Address, blockNumber *big.Int) (*big.Int, error)
func (ec *Client) CallContract(ctx context.Context, msg ethereum.CallMsg, blockNumber *big.Int) ([]byte, error)
func (ec *Client) Code At(ctx context.Context, account commo n.Address, blockNumber *big.Int) ([]byte, error)
func (ec *Client) EstimateGas(ctx context.Context, msg ethereum.CallMsg) (uint64, error)
func (ec *Client) FilterLogs(ctx context.Context, q ethereum.FilterQuery) ([]types.Log, error)
func (ec *Client) HeaderByHash(ctx context.Context, hash commo n.Hash) (*types.Header, error)
func (ec *Client) HeaderByNumber(ctx context.Context, number *big.Int) (*types.Header, error)
func (ec *Client) NonceAt(ctx context.Context, account commo n.Address, blockNumber *big.Int) (uint64, error)
func (ec *Client) PendingBalanceAt(ctx context.Context, account commo n.Address) (*big.Int, error)
func (ec *Client) PendingCallContract(ctx context.Context, msg ethereum.CallMsg) ([]byte, error)
func (ec *Client) PendingCode At(ctx context.Context, account commo n.Address) ([]byte, error)
func (ec *Client) PendingNonceAt(ctx context.Context, account commo n.Address) (uint64, error)
func (ec *Client) PendingStorageAt(ctx context.Context, account commo n.Address, key commo n.Hash) ([]byte, error)
func (ec *Client) PendingTransactionCount(ctx context.Context) (uint, error)
func (ec *Client) SendTransaction(ctx context.Context, tx *types.Transaction) error
func (ec *Client) StorageAt(ctx context.Context, account commo n.Address, key commo n.Hash, ...) ([]byte, error)
func (ec *Client) SubscribeFilterLogs(ctx context.Context, q ethereum.FilterQuery, ch chan<- types.Log) (ethereum.Subscription, error)
func (ec *Client) SubscribeNewHead(ctx context.Context, ch chan<- *types.Header) (ethereum.Subscription, error)
func (ec *Client) SuggestGasPrice(ctx context.Context) (*big.Int, error)
func (ec *Client) SuggestGasTipCap(ctx context.Context) (*big.Int, error)
func (ec *Client) SyncProgress(ctx context.Context) (*ethereum.SyncProgress, error)
func (ec *Client) TransactionCount(ctx context.Context, blockHash commo n.Hash) (uint, error)
func (ec *Client) TransactionSender(ctx context.Context, tx *types.Transaction, block commo n.Hash, index uint) (commo n.Address, error)
*/

// DecodeHex decodes a string with hex into a slice of bytes
func DecodeHex(hex string) []byte {
	return hexutil.MustDecode(hex)
}

func GetCodeAt(chain string, addr base.Address, bn uint64) ([]byte, error) {
	// return IsValidAddress(addr)
	provider, _ := config.GetRpcProvider(chain)
	ec, _ := GetClient(provider)
	// TODO: we don't use block number, but we should - we need to convert it
	return ec.CodeAt(context.Background(), addr.ToCommon(), nil) // nil is latest block
}
