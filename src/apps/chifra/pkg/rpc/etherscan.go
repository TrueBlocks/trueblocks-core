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
)

var m = map[string]string{
	"ext":    "txlist",
	"int":    "txlistinternal",
	"token":  "tokentx",
	"nfts":   "tokennfttx",
	"1155":   "token1155tx",
	"miner":  "getminedblocks&blocktype=blocks",
	"uncles": "getminedblocks&blocktype=uncles",
}
var ss = map[string]string{
	"ext":    "asc",
	"int":    "asc",
	"token":  "asc",
	"nfts":   "asc",
	"1155":   "asc",
	"miner":  "asc",
	"uncles": "asc",
}

func getEtherscanUrl(addr string, requestType string, paginator *Paginator) string {
	if ss[requestType] == "" || m[requestType] == "" {
		logger.Fatal("Should not happen in getEtherscanUrl", requestType)
	}

	const str = "https://api.etherscan.io/api?module=account&sort=[{SORT}]&action=[{CMD}]&address=[{ADDRESS}]&page=[{PAGE}]&offset=[{PER_PAGE}]"
	ret := strings.Replace(str, "[{SORT}]", ss[requestType], -1)
	ret = strings.Replace(ret, "[{CMD}]", m[requestType], -1)
	ret = strings.Replace(ret, "[{ADDRESS}]", addr, -1)
	ret = strings.Replace(ret, "[{PAGE}]", fmt.Sprintf("%d", paginator.Page), -1)
	ret = strings.Replace(ret, "[{PER_PAGE}]", fmt.Sprintf("%d", paginator.PerPage), -1)
	paginator.Page++
	return ret
}

type Paginator struct {
	Page    int
	PerPage int
}

func (conn *Connection) GetTransactionsFromEtherscan(addr, requestType string, paginator *Paginator) ([]types.SimpleEtherscan, int, error) {
	url := getEtherscanUrl(addr, requestType, paginator)

	var ret []types.SimpleEtherscan

	key := config.GetRootConfig().Keys["etherscan"].ApiKey
	if key == "" {
		return ret, 0, errors.New("cannot read Etherscan API key")
	}
	url += "&apikey=" + key

	resp, err := http.Get(url)
	if err != nil {
		return ret, 0, err
	}
	defer resp.Body.Close()

	// Check server response
	if resp.StatusCode != http.StatusOK {
		return ret, 0, fmt.Errorf("etherscan API error: %s", resp.Status)
	}

	decoder := json.NewDecoder(resp.Body)
	fromEs := etherscanResponse{}
	if err = decoder.Decode(&fromEs); err != nil {
		return ret, 0, err
	}
	resp.Body.Close()

	if fromEs.Message == "NOTOK" {
		// Etherscan sends 200 OK responses even if there's an error. We want to cache the error
		// response so we don't keep asking Etherscan for the same address. The user may later
		// remove empty ABIs with chifra abis --clean.
		logger.Warn("provider responded with:", url, fromEs.Message)
		return ret, 0, nil
	}

	for _, esTx := range fromEs.Result {
		rawTx := types.RawEtherscan{
			BlockHash:        esTx.BlockHash,
			BlockNumber:      esTx.BlockNumber,
			From:             esTx.From,
			Gas:              esTx.Gas,
			GasPrice:         esTx.GasPrice,
			Hash:             esTx.Hash,
			Input:            esTx.Input,
			To:               esTx.To,
			TransactionIndex: esTx.TransactionIndex,
			Value:            esTx.Value,
		}

		transaction := types.SimpleEtherscan{
			Hash:             base.HexToHash(rawTx.Hash),
			BlockHash:        base.HexToHash(rawTx.BlockHash),
			BlockNumber:      mustParseUint(rawTx.BlockNumber),
			TransactionIndex: mustParseUint(rawTx.TransactionIndex),
			Timestamp:        mustParseInt(esTx.Timestamp),
			From:             base.HexToAddress(rawTx.From),
			To:               base.HexToAddress(rawTx.To),
			Gas:              mustParseUint(rawTx.Gas),
			GasPrice:         mustParseUint(rawTx.GasPrice),
			GasUsed:          mustParseUint(esTx.GasUsed),
			Input:            rawTx.Input,
		}
		transaction.GasCost = transaction.GasPrice * transaction.GasUsed
		transaction.IsError = esTx.TxReceiptStatus == "0"
		transaction.HasToken = requestType == "nfts" || requestType == "token" || requestType == "1155"
		transaction.Value.SetString(rawTx.Value, 0)
		transaction.ContractAddress = base.HexToAddress(esTx.ContractAddress)
		if requestType == "int" {
			// Markers to help us remove these since Etherscan doesn't send them and we don't want to make another RPC call
			transaction.BlockHash = base.HexToHash("0xdeadbeef")
			transaction.TransactionIndex = 80809
		} else if requestType == "miner" {
			transaction.BlockHash = base.HexToHash("0xdeadbeef")
			transaction.TransactionIndex = 99999
			transaction.From = base.BlockRewardSender
			transaction.Value.SetString("5000000000000000000", 0)
			transaction.To = base.HexToAddress(addr)
		} else if requestType == "uncles" {
			transaction.BlockHash = base.HexToHash("0xdeadbeef")
			transaction.TransactionIndex = 99998
			transaction.From = base.UncleRewardSender
			transaction.Value.SetString("3750000000000000000", 0)
			transaction.To = base.HexToAddress(addr)
		}
		transaction.SetRaw(&rawTx)

		ret = append(ret, transaction)
	}

	return ret, len(ret), nil
}

func mustParseUint(input any) (result uint64) {
	result, _ = strconv.ParseUint(fmt.Sprint(input), 0, 64)
	return
}

func mustParseInt(input any) (result int64) {
	result, _ = strconv.ParseInt(fmt.Sprint(input), 0, 64)
	return
}

type etherscanResponse struct {
	Message string               `json:"message"`
	Result  []types.RawEtherscan `json:"result"`
	Status  string               `json:"status"`
}
