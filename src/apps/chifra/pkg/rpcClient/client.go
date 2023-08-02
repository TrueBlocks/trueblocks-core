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
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/ethereum/go-ethereum/common"
	ethTypes "github.com/ethereum/go-ethereum/core/types"
	"github.com/ethereum/go-ethereum/ethclient"
)

// TODO: this looks weird, but before we were creating and deleting the client with every call which
// TODO: overran the number of TPC connection the OS would create (on a Mac). Since then, we now
// TODO: open the connection once and just use it allowing the operating system to clean it up
var perProviderClientMap = map[string]*ethclient.Client{}
var clientMutex sync.Mutex

func getClient(chain string) (*ethclient.Client, error) {
	provider, _ := config.GetRpcProvider(chain)
	if provider == "https://" {
		var noProvider = `

  Warning: The RPC server ([{PROVIDER}]) was not available. Either start it, or edit the rpcProvider
  value in the file [{FILE}]. Quitting...
`
		msg := noProvider
		msg = strings.Replace(msg, "[{PROVIDER}]", provider, -1)
		msg = strings.Replace(msg, "[{FILE}]", config.GetPathToRootConfig()+"trueBlocks.toml", -1)
		msg = strings.Replace(msg, "https://", "<empty>", -1)
		return nil, fmt.Errorf("%s", msg)
	}

	clientMutex.Lock()
	if perProviderClientMap[provider] == nil {
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
func (options *Options) GetLatestBlockNumber(chain string) uint64 {
	if ec, err := getClient(chain); err != nil {
		logger.Error("Could not connect to RPC client: %w", err)
		return 0
	} else {
		defer ec.Close()

		r, err := ec.BlockNumber(context.Background())
		if err != nil {
			logger.Error("Could not connect to RPC client: %w", err)
			return 0
		}

		return r
	}
}

// GetClientIDs returns both chainId and networkId from the node
func (options *Options) GetClientIDs(chain string) (uint64, uint64, error) {
	if ec, err := getClient(chain); err != nil {
		return 0, 0, err
	} else {
		defer ec.Close()

		ch, err := ec.ChainID(context.Background())
		if err != nil {
			return 0, 0, err
		}

		n, err := ec.NetworkID(context.Background())
		if err != nil {
			return ch.Uint64(), 0, err
		}

		return ch.Uint64(), n.Uint64(), nil
	}
}

// TODO: C++ code used to cache version info

// GetClientVersion returns the version of the client
func (options *Options) GetClientVersion(chain string) (version string, err error) {
	method := "web3_clientVersion"
	params := rpc.Params{}

	if version, err := rpc.Query[string](chain, method, params); err != nil {
		return "", err
	} else {
		return *version, nil
	}
}

// GetTransactionHashFromHashStr returns a transaction's hash if it's a valid transaction, an empty string otherwise
func (options *Options) GetTransactionHashFromHashStr(chain, hash string) (string, error) {
	if ec, err := getClient(chain); err != nil {
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

// GetBlockHashFromHashStr returns a block's hash if it's a valid block
func (options *Options) GetBlockHashFromHashStr(chain, hash string) (string, error) {
	if ec, err := getClient(chain); err != nil {
		return "", err
	} else {
		defer ec.Close()

		block, err := ec.BlockByHash(context.Background(), common.HexToHash(hash))
		if err != nil {
			return "", err
		}

		return block.Hash().Hex(), nil
	}
}

// GetTransactionHashByHashAndID returns a transaction's hash if it's a valid transaction
func (options *Options) GetTransactionHashByHashAndID(chain, hash string, txId uint64) (string, error) {
	if ec, err := getClient(chain); err != nil {
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

// GetTransactionByNumberAndID returns an actual transaction
func (options *Options) GetTransactionByNumberAndID(chain string, bn, txId uint64) (ethTypes.Transaction, error) {
	if ec, err := getClient(chain); err != nil {
		return ethTypes.Transaction{}, err
	} else {
		defer ec.Close()

		block, err := ec.BlockByNumber(context.Background(), new(big.Int).SetUint64(bn))
		if err != nil {
			return ethTypes.Transaction{}, err
		}

		tx, err := ec.TransactionInBlock(context.Background(), block.Hash(), uint(txId))
		if err != nil {
			return ethTypes.Transaction{}, err
		}

		return *tx, nil
	}
}

// GetCountTransactionsInBlock returns the number of transactions in a block
func (options *Options) GetCountTransactionsInBlock(chain string, bn uint64) (uint64, error) {
	if ec, err := getClient(chain); err != nil {
		return 0, err
	} else {
		defer ec.Close()

		block, err := ec.BlockByNumber(context.Background(), new(big.Int).SetUint64(bn))
		if err != nil {
			return 0, err
		}

		cnt, err := ec.TransactionCount(context.Background(), block.Hash())
		return uint64(cnt), err
	}
}

// GetBlockNumberByHash returns a block's hash if it's a valid block
func (options *Options) GetBlockNumberByHash(chain, hash string) (base.Blknum, error) {
	if ec, err := getClient(chain); err != nil {
		return 0, err
	} else {
		defer ec.Close()

		block, err := ec.BlockByHash(context.Background(), common.HexToHash(hash))
		if err != nil {
			return 0, err
		}

		return block.NumberU64(), nil
	}
}

// GetBlockHashByNumber returns a block's hash if it's a valid block
func (options *Options) GetBlockHashByNumber(chain string, bn uint64) (string, error) {
	if ec, err := getClient(chain); err != nil {
		return "", err
	} else {
		defer ec.Close()

		block, err := ec.BlockByNumber(context.Background(), new(big.Int).SetUint64(bn))
		if err != nil {
			return "", err
		}

		return block.Hash().Hex(), nil
	}
}

// GetBalanceAt returns a balance for an address at a block
func (options *Options) GetBalanceAt(chain string, addr base.Address, bn uint64) (*big.Int, error) {
	if ec, err := getClient(chain); err != nil {
		var zero big.Int
		return &zero, err
	} else {
		defer ec.Close()
		return ec.BalanceAt(context.Background(), addr.ToCommon(), new(big.Int).SetUint64(bn))
	}
}

// GetCodeAt returns a code (if any) for an address at a block
func (options *Options) GetCodeAt(chain string, addr base.Address, bn uint64) ([]byte, error) {
	if ec, err := getClient(chain); err != nil {
		return []byte{}, err
	} else {
		defer ec.Close()
		return ec.CodeAt(context.Background(), addr.ToCommon(), new(big.Int).SetUint64(bn))
	}
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
