package rpc

import (
	"encoding/json"
	"errors"
	"fmt"
	"net/http"
	"strconv"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

type Paginator struct {
	Page    int
	PerPage int
}

func (conn *Connection) GetESTransactionByAddress(addr, requestType string, paginator *Paginator) ([]types.SimpleSlurp, int, error) {
	url, err := getEtherscanUrl(addr, requestType, paginator)
	if err != nil {
		return []types.SimpleSlurp{}, 0, err
	}

	resp, err := http.Get(url)
	if err != nil {
		return []types.SimpleSlurp{}, 0, err
	}
	defer resp.Body.Close()

	// Check server response
	if resp.StatusCode != http.StatusOK {
		return []types.SimpleSlurp{}, 0, fmt.Errorf("etherscan API error: %s", resp.Status)
	}

	decoder := json.NewDecoder(resp.Body)
	fromEs := struct {
		Message string           `json:"message"`
		Result  []types.RawSlurp `json:"result"`
		Status  string           `json:"status"`
	}{}
	if err = decoder.Decode(&fromEs); err != nil {
		return []types.SimpleSlurp{}, 0, err
	}

	if fromEs.Message == "NOTOK" {
		// Etherscan sends 200 OK responses even if there's an error. We want to cache the error
		// response so we don't keep asking Etherscan for the same address. The user may later
		// remove empty ABIs with chifra abis --clean.
		logger.Warn("provider responded with:", url, fromEs.Message)
		return []types.SimpleSlurp{}, 0, nil
		// } else if fromEs.Message != "OK" {
		// 	logger.Warn("URL:", url)
		// 	logger.Warn("provider responded with:", url, fromEs.Message)
	}

	return conn.responseToTransactions(addr, requestType, fromEs.Result)
}

// func (conn *Connection) getESTransactionByHash(txHash base.Hash) (types.SimpleSlurp, error) {
// 	url, err := getEtherscanUrl(txHash.Hex(), "byHash", &Paginator{Page: 1, PerPage: 10})
// 	if err != nil {
// 		return types.SimpleSlurp{}, err
// 	}

// 	resp, err := http.Get(url)
// 	if err != nil {
// 		return types.SimpleSlurp{}, err
// 	}
// 	defer resp.Body.Close()

// 	// Check server response
// 	if resp.StatusCode != http.StatusOK {
// 		return types.SimpleSlurp{}, fmt.Errorf("etherscan API error: %s", resp.Status)
// 	}

// 	decoder := json.NewDecoder(resp.Body)
// 	fromEs := struct {
// 		JsonRpc string             `json:"jsonrpc"`
// 		Id      int                `json:"id"`
// 		Result  types.RawSlurp `json:"result"`
// 	}{}
// 	if err = decoder.Decode(&fromEs); err != nil {
// 		return types.SimpleSlurp{}, err
// 	}

// 	return conn.rawToSimple("", "byHash", &fromEs.Result)
// }

// responseToTransaction converts one RawEtherscan to SimpleSlurp.
func (conn *Connection) rawToSimple(addr, requestType string, rawTx *types.RawSlurp) (types.SimpleSlurp, error) {
	s := types.SimpleSlurp{
		Hash:             base.HexToHash(rawTx.Hash),
		BlockHash:        base.HexToHash(rawTx.BlockHash),
		BlockNumber:      utils.MustParseUint(rawTx.BlockNumber),
		TransactionIndex: utils.MustParseUint(rawTx.TransactionIndex),
		Timestamp:        mustParseInt(rawTx.Timestamp),
		From:             base.HexToAddress(rawTx.From),
		To:               base.HexToAddress(rawTx.To),
		Gas:              utils.MustParseUint(rawTx.Gas),
		GasPrice:         utils.MustParseUint(rawTx.GasPrice),
		GasUsed:          utils.MustParseUint(rawTx.GasUsed),
		Input:            rawTx.Input,
	}

	s.IsError = rawTx.TxReceiptStatus == "0"
	s.HasToken = requestType == "nfts" || requestType == "token" || requestType == "1155"
	s.Value.SetString(rawTx.Value, 0)
	s.ContractAddress = base.HexToAddress(rawTx.ContractAddress)

	if requestType == "int" {
		// We use a weird marker here since Etherscan doesn't send the transaction id for internal txs and we don't want to make another RPC call
		// We tried (see commented code), but EtherScan balks with a weird message
		s.TransactionIndex = 80809
		// s.BlockHash = base.HexToHash("0xdeadbeef")
		// got, err := conn.GetESTransactionByHash(s.Hash)
		// if err != nil {
		// 	logger.Warn("error getting transaction from etherscan:", err)
		// 	s.TransactionIndex = 80809
		// } else {
		// 	s.TransactionIndex = utils.MustParseUint(got.TransactionIndex)
		// }
	} else if requestType == "miner" {
		s.BlockHash = base.HexToHash("0xdeadbeef")
		s.TransactionIndex = 99999
		s.From = base.BlockRewardSender
		s.Value.SetString("5000000000000000000", 0)
		s.To = base.HexToAddress(addr)

	} else if requestType == "uncles" {
		s.BlockHash = base.HexToHash("0xdeadbeef")
		s.TransactionIndex = 99998
		s.From = base.UncleRewardSender
		s.Value.SetString("3750000000000000000", 0)
		s.To = base.HexToAddress(addr)
	}
	s.SetRaw(rawTx)
	return s, nil
}

// responseToTransactions converts RawEtherscans to SimpleSlurp. It also returns the number of results.
func (conn *Connection) responseToTransactions(addr, requestType string, rawTxs []types.RawSlurp) ([]types.SimpleSlurp, int, error) {
	var ret []types.SimpleSlurp
	for _, rawTx := range rawTxs {
		rawTx := rawTx
		if transaction, err := conn.rawToSimple(addr, requestType, &rawTx); err != nil {
			return nil, 0, err
		} else {
			ret = append(ret, transaction)
		}
	}
	return ret, len(ret), nil
}

func getEtherscanUrl(value string, requestType string, paginator *Paginator) (string, error) {
	var actions = map[string]string{
		"ext":    "txlist",
		"int":    "txlistinternal",
		"token":  "tokentx",
		"nfts":   "tokennfttx",
		"1155":   "token1155tx",
		"miner":  "getminedblocks&blocktype=blocks",
		"uncles": "getminedblocks&blocktype=uncles",
		"byHash": "eth_getTransactionByHash",
	}

	if actions[requestType] == "" {
		logger.Fatal("Should not happen in getEtherscanUrl", requestType)
	}

	key := config.GetRootConfig().Keys["etherscan"].ApiKey
	if key == "" {
		return "", errors.New("cannot read Etherscan API key")
	}

	module := "account"
	tt := "address"
	if requestType == "byHash" {
		module = "proxy"
		tt = "txhash"
	}

	const str = "https://api.etherscan.io/api?module=[{MODULE}]&sort=asc&action=[{ACTION}]&[{TT}]=[{VALUE}]&page=[{PAGE}]&offset=[{PER_PAGE}]"
	ret := strings.Replace(str, "[{MODULE}]", module, -1)
	ret = strings.Replace(ret, "[{TT}]", tt, -1)
	ret = strings.Replace(ret, "[{ACTION}]", actions[requestType], -1)
	ret = strings.Replace(ret, "[{VALUE}]", value, -1)
	ret = strings.Replace(ret, "[{PAGE}]", fmt.Sprintf("%d", paginator.Page), -1)
	ret = strings.Replace(ret, "[{PER_PAGE}]", fmt.Sprintf("%d", paginator.PerPage), -1)
	ret = ret + "&apikey=" + key

	paginator.Page++

	return ret, nil
}

func mustParseInt(input any) (result int64) {
	result, _ = strconv.ParseInt(fmt.Sprint(input), 0, 64)
	return
}
