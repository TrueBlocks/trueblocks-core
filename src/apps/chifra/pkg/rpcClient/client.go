// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package rpcClient

import (
	"context"
	"fmt"
	"log"
	"math/big"
	"strconv"
	"strings"
	"sync"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/ethereum/go-ethereum/common"
	"github.com/ethereum/go-ethereum/common/hexutil"
	"github.com/ethereum/go-ethereum/ethclient"
)

// TODO: this looks weird, but before we were creating and deleting the client with every call which
// TODO: overran the number of TPC connection the OS would create (on a Mac). Since then, we now
// TODO: open the connection once and just use it allowing the operating system to clean it up
var perProviderClientMap = map[string]*ethclient.Client{}
var clientMutex sync.Mutex

func GetClient(provider string) *ethclient.Client {
	clientMutex.Lock()
	if perProviderClientMap[provider] == nil {
		// TODO: I don't like the fact that we Dail In every time we want to us this
		// TODO: If we make this a cached item, it needs to be cached per chain, see timestamps
		ec, err := ethclient.Dial(provider)
		if err != nil || ec == nil {
			log.Println("Missdial(" + provider + "):")
			log.Fatalln(err)
		}
		perProviderClientMap[provider] = ec
	}
	clientMutex.Unlock()
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
func GetIDs(provider string) (uint64, uint64, error) {
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

	ec := GetClient(provider)
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

// TxNumberAndIdFromHash returns a transaction's blockNum and tx_id given its hash
func TxNumberAndIdFromHash(provider string, hash string) (uint64, uint64, error) {
	// RPCPayload is used during to make calls to the RPC.
	var trans Transaction
	transPayload := RPCPayload{
		Jsonrpc:   "2.0",
		Method:    "eth_getTransactionByHash",
		RPCParams: RPCParams{fmt.Sprintf("%s", hash)},
		ID:        1002,
	}
	err := FromRpc(provider, &transPayload, &trans)
	if err != nil {
		fmt.Println("FromRpc(traces) returned error")
		log.Fatal(err)
	}
	bn, err := strconv.ParseUint(trans.Result.BlockNumber[2:], 16, 32)
	if err != nil {
		return 0, 0, err
	}
	txid, err := strconv.ParseUint(trans.Result.TransactionIndex[2:], 16, 32)
	if err != nil {
		return 0, 0, err
	}
	return bn, txid, nil
}

// TxCountByBlockNumber returns the number of transactions in a block
func TxCountByBlockNumber(provider string, blkNum uint64) (uint64, error) {
	ec := GetClient(provider)
	defer ec.Close()

	block, err := ec.BlockByNumber(context.Background(), new(big.Int).SetUint64(blkNum))
	if err != nil {
		return 0, err
	}

	cnt, err := ec.TransactionCount(context.Background(), block.Hash())
	return uint64(cnt), nil
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

// BlockNumberFromHash returns a block's hash if it's a valid block
func BlockNumberFromHash(provider, hash string) (uint64, error) {
	ec := GetClient(provider)
	defer ec.Close()

	block, err := ec.BlockByHash(context.Background(), common.HexToHash(hash))
	if err != nil {
		return 0, err
	}

	return block.NumberU64(), nil
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

// TODO: This is okay since Ropsten is dead as of the merge. We use it for testing
// TODO: but we need this to actually work (for Geth for instance)
func IsTracingNode(testMode bool, chain string) bool {
	if testMode && chain == "ropsten" {
		return false
	}
	return true
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
	defer ec.Close()

	r, err := ec.BlockByNumber(context.Background(), big.NewInt(int64(bn)))
	if err != nil {
		logger.Log(logger.Error, "Could not connect to RPC client")
		return 0
	}

	return r.Time()
}

// DecodeHex decodes a string with hex into a slice of bytes
func DecodeHex(hex string) []byte {
	return hexutil.MustDecode(hex)
}

func GetBlockByNumber(chain string, bn uint64) (types.NamedBlock, error) {
	var block BlockHeader
	var payload = RPCPayload{
		Jsonrpc:   "2.0",
		Method:    "eth_getBlockByNumber",
		RPCParams: RPCParams{fmt.Sprintf("0x%x", bn), false},
		ID:        1005,
	}
	rpcProvider := config.GetRpcProvider(chain)
	err := FromRpc(rpcProvider, &payload, &block)
	if err != nil {
		return types.NamedBlock{}, err
	}
	if len(block.Result.Number) == 0 || len(block.Result.Timestamp) == 0 {
		msg := fmt.Sprintf("block number or timestamp for %d not found", bn)
		return types.NamedBlock{}, fmt.Errorf(msg)
	}
	n, _ := strconv.ParseUint(block.Result.Number[2:], 16, 64)
	ts, _ := strconv.ParseUint(block.Result.Timestamp[2:], 16, 64)
	if n == 0 {
		ts, err = GetBlockZeroTs(chain)
		if err != nil {
			return types.NamedBlock{}, err
		}
	}
	return types.NamedBlock{
		BlockNumber: n,
		TimeStamp:   ts,
	}, nil
}

// GetBlockZeroTs for some reason block zero does not return a timestamp, so we assign block one's ts minus 14 seconds
func GetBlockZeroTs(chain string) (uint64, error) {
	blockOne, err := GetBlockByNumber(chain, 1)
	if err != nil {
		return utils.EarliestEvmTs, err
	}
	return blockOne.TimeStamp - 14, nil
}

// TODO: use block number by converting it
func GetCodeAt(chain, addr string, bn uint64) ([]byte, error) {
	// return IsValidAddress(addr)
	provider := config.GetRpcProvider(chain)
	ec := GetClient(provider)
	address := common.HexToAddress(addr)
	return ec.CodeAt(context.Background(), address, nil) // nil is latest block
}
