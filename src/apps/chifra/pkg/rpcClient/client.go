// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package rpcClient

import (
	"context"
	"log"
	"math/big"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/ethereum/go-ethereum/common"
	"github.com/ethereum/go-ethereum/common/hexutil"
	"github.com/ethereum/go-ethereum/ethclient"
)

// TODO: this looks weird, but before we were creating and deleting the client with every call which
// TODO: overran the number of TPC connection the OS would create (on a Mac). Since then, we now
// TODO: open the connection once and just use it allowing the operating system to clean it up
var perProviderClientMap = map[string]*ethclient.Client{}

func GetClient(provider string) *ethclient.Client {
	if perProviderClientMap[provider] == nil {
		// TODO: I don't like the fact that we Dail In every time we want to us this
		// TODO: If we make this a cached item, it needs to be cached per chain, see timestamps
		ec, err := ethclient.Dial(provider)
		if err != nil {
			log.Fatalln(err)
		}
		perProviderClientMap[provider] = ec
	}
	return perProviderClientMap[provider]
}

// BlockNumber returns the block number at the front of the chain (i.e. latest)
func BlockNumber(provider string) uint64 {
	ec := GetClient(provider)
	defer ec.Close()

	r, err := ec.BlockNumber(context.Background())
	if err != nil {
		logger.Log(logger.Error, "Could not connect to RPC client")
		return 0
	}

	return r
}

var noProvider string = `

  {R}Warning{O}: The RPC server ([{PROVIDER}]) was not found. Either start it, or edit the {T}rpcProvider{O}
  value in the file {T}[{FILE}]{O}. Quitting...
`

// CheckRpc will not return if the RPC is not available
func CheckRpc(provider string) {
	GetIDs(provider)
}

// GetIDs returns both chainId and networkId from the node
func GetIDs(provider string) (uint64, uint64) {
	ec := GetClient(provider)
	defer ec.Close()

	// We might need it, so create it
	msg := noProvider
	msg = strings.Replace(msg, "[{PROVIDER}]", provider, -1)
	msg = strings.Replace(msg, "[{FILE}]", config.GetPathToRootConfig()+"trueBlocks.toml", -1)
	msg = strings.Replace(msg, "{R}", colors.Red, -1)
	msg = strings.Replace(msg, "{T}", colors.Cyan, -1)
	msg = strings.Replace(msg, "{O}", colors.Off, -1)

	ch, err := ec.ChainID(context.Background())
	if err != nil {
		log.Fatalln(msg)
	}

	n, err := ec.NetworkID(context.Background())
	if err != nil {
		log.Fatalln(msg)
	}

	return ch.Uint64(), n.Uint64()
}

// TxHashFromHash returns a transaction's hash if it's a valid transaction
func TxHashFromHash(provider, hash string) (string, error) {
	ec := GetClient(provider)
	defer ec.Close()

	tx, _, err := ec.TransactionByHash(context.Background(), common.HexToHash(hash))
	if err != nil {
		return "", err
	}

	return tx.Hash().Hex(), nil
}

// TxHashFromHashAndId returns a transaction's hash if it's a valid transaction
func TxHashFromHashAndId(provider, hash string, txId uint64) (string, error) {
	ec := GetClient(provider)
	defer ec.Close()

	tx, err := ec.TransactionInBlock(context.Background(), common.HexToHash(hash), uint(txId))
	if err != nil {
		return "", err
	}

	return tx.Hash().Hex(), nil
}

// TxHashFromNumberAndId returns a transaction's hash if it's a valid transaction
func TxHashFromNumberAndId(provider string, blkNum, txId uint64) (string, error) {
	ec := GetClient(provider)
	defer ec.Close()

	block, err := ec.BlockByNumber(context.Background(), new(big.Int).SetUint64(blkNum))
	if err != nil {
		return "", err
	}

	tx, err := ec.TransactionInBlock(context.Background(), block.Hash(), uint(txId))
	if err != nil {
		return "", err
	}

	return tx.Hash().Hex(), nil
}

// BlockHashFromHash returns a block's hash if it's a valid block
func BlockHashFromHash(provider, hash string) (string, error) {
	ec := GetClient(provider)
	defer ec.Close()

	block, err := ec.BlockByHash(context.Background(), common.HexToHash(hash))
	if err != nil {
		return "", err
	}

	return block.Hash().Hex(), nil
}

// BlockHashFromNumber returns a block's hash if it's a valid block
func BlockHashFromNumber(provider string, blkNum uint64) (string, error) {
	ec := GetClient(provider)
	defer ec.Close()

	block, err := ec.BlockByNumber(context.Background(), new(big.Int).SetUint64(blkNum))
	if err != nil {
		return "", err
	}

	return block.Hash().Hex(), nil
}

/*
// Functions available in the client
func NewClient(c *rpc.Client) *Client
func (ec *Client) BalanceAt(ctx context.Context, account common.Address, blockNumber *big.Int) (*big.Int, error)
func (ec *Client) CallContract(ctx context.Context, msg ethereum.CallMsg, blockNumber *big.Int) ([]byte, error)
func (ec *Client) CodeAt(ctx context.Context, account common.Address, blockNumber *big.Int) ([]byte, error)
func (ec *Client) EstimateGas(ctx context.Context, msg ethereum.CallMsg) (uint64, error)
func (ec *Client) FilterLogs(ctx context.Context, q ethereum.FilterQuery) ([]types.Log, error)
func (ec *Client) HeaderByHash(ctx context.Context, hash common.Hash) (*types.Header, error)
func (ec *Client) HeaderByNumber(ctx context.Context, number *big.Int) (*types.Header, error)
func (ec *Client) NonceAt(ctx context.Context, account common.Address, blockNumber *big.Int) (uint64, error)
func (ec *Client) PendingBalanceAt(ctx context.Context, account common.Address) (*big.Int, error)
func (ec *Client) PendingCallContract(ctx context.Context, msg ethereum.CallMsg) ([]byte, error)
func (ec *Client) PendingCodeAt(ctx context.Context, account common.Address) ([]byte, error)
func (ec *Client) PendingNonceAt(ctx context.Context, account common.Address) (uint64, error)
func (ec *Client) PendingStorageAt(ctx context.Context, account common.Address, key common.Hash) ([]byte, error)
func (ec *Client) PendingTransactionCount(ctx context.Context) (uint, error)
func (ec *Client) SendTransaction(ctx context.Context, tx *types.Transaction) error
func (ec *Client) StorageAt(ctx context.Context, account common.Address, key common.Hash, ...) ([]byte, error)
func (ec *Client) SubscribeFilterLogs(ctx context.Context, q ethereum.FilterQuery, ch chan<- types.Log) (ethereum.Subscription, error)
func (ec *Client) SubscribeNewHead(ctx context.Context, ch chan<- *types.Header) (ethereum.Subscription, error)
func (ec *Client) SuggestGasPrice(ctx context.Context) (*big.Int, error)
func (ec *Client) SuggestGasTipCap(ctx context.Context) (*big.Int, error)
func (ec *Client) SyncProgress(ctx context.Context) (*ethereum.SyncProgress, error)
func (ec *Client) TransactionCount(ctx context.Context, blockHash common.Hash) (uint, error)
func (ec *Client) TransactionReceipt(ctx context.Context, txHash common.Hash) (*types.Receipt, error)
func (ec *Client) TransactionSender(ctx context.Context, tx *types.Transaction, block common.Hash, index uint) (common.Address, error)
*/

func GetBlockTimestamp(provider string, bn uint64) uint64 {
	ec := GetClient(provider)
	r, err := ec.BlockByNumber(context.Background(), big.NewInt(int64(bn)))
	defer ec.Close()
	if err != nil {
		logger.Log(logger.Error, "Could not connect to RPC client")
		return 0
	}
	return r.Time()
}

// HexToAddress converts a string with hex to go-ethereum's common.Address
func HexToAddress(hex string) common.Address {
	return common.HexToAddress(hex)
}

// DecodeHex decodes a string with hex into a slice of bytes
func DecodeHex(hex string) []byte {
	return hexutil.MustDecode(hex)
}
