package rpc

import (
	"encoding/json"
	"errors"
	"fmt"
	"net/http"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/debug"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

func (conn *Connection) getTxsByAddressEs(chain, addr, requestType string, paginator *Paginator) ([]types.SimpleSlurp, int, error) {
	url, err := getEtherscanUrl(chain, addr, requestType, paginator)
	if err != nil {
		return []types.SimpleSlurp{}, 0, err
	}

	debug.DebugCurlStr(url)
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
		if fromEs.Message == "NOTOK" {
			return []types.SimpleSlurp{}, 0, fmt.Errorf("provider responded with: %s %s", url, fromEs.Message)
		}
		return []types.SimpleSlurp{}, 0, fmt.Errorf("decoder failed: %w", err)
	}

	if fromEs.Message == "NOTOK" {
		// Etherscan sends 200 OK responses even if there's an error. We want to cache the error
		// response so we don't keep asking Etherscan for the same address. The user may later
		// remove empty ABIs with chifra abis --decache.
		logger.Warn("provider responded with:", url, fromEs.Message, strings.Repeat(" ", 40))
		return []types.SimpleSlurp{}, 0, nil
		// } else if fromEs.Message != "OK" {
		// 	logger.Warn("URL:", url)
		// 	logger.Warn("provider responded with:", url, fromEs.Message)
	}

	var ret []types.SimpleSlurp
	for _, rawTx := range fromEs.Result {
		if transaction, err := conn.rawSlurpToSimple(addr, requestType, &rawTx); err != nil {
			return nil, 0, err
		} else {
			ret = append(ret, transaction)
		}
	}

	return ret, len(ret), nil
}

// rawSlurpToSimple converts one RawEtherscan to SimpleSlurp.
func (conn *Connection) rawSlurpToSimple(addr, requestType string, rawTx *types.RawSlurp) (types.SimpleSlurp, error) {
	s := types.SimpleSlurp{
		Hash:             base.HexToHash(rawTx.Hash),
		BlockHash:        base.HexToHash(rawTx.BlockHash),
		BlockNumber:      utils.MustParseUint(rawTx.BlockNumber),
		TransactionIndex: utils.MustParseUint(rawTx.TransactionIndex),
		Timestamp:        utils.MustParseInt(rawTx.Timestamp),
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
		// We use a weird marker here since Etherscan doesn't send the transaction id for internal txs and we don't
		// want to make another RPC call. We tried (see commented code), but EtherScan balks with a weird message
		app, _ := conn.GetTransactionAppByHash(s.Hash.Hex())
		s.TransactionIndex = uint64(app.TransactionIndex)
	} else if requestType == "miner" {
		s.BlockHash = base.HexToHash("0xdeadbeef")
		s.TransactionIndex = types.BlockReward
		s.From = base.BlockRewardSender
		// TODO: This is incorrect for mainnet
		s.Value.SetString("5000000000000000000", 0)
		s.To = base.HexToAddress(addr)
	} else if requestType == "uncles" {
		s.BlockHash = base.HexToHash("0xdeadbeef")
		s.TransactionIndex = types.UncleReward
		s.From = base.UncleRewardSender
		// TODO: This is incorrect for mainnet
		s.Value.SetString("3750000000000000000", 0)
		s.To = base.HexToAddress(addr)
	} else if requestType == "withdrawals" {
		s.BlockHash = base.HexToHash("0xdeadbeef")
		s.TransactionIndex = types.Withdrawal
		s.From = base.WithdrawalSender
		s.ValidatorIndex = utils.MustParseUint(rawTx.ValidatorIndex)
		s.WithdrawalIndex = utils.MustParseUint(rawTx.WithdrawalIndex)
		s.Value.SetString(rawTx.Amount, 0)
		s.To = base.HexToAddress(addr)
		if s.To != base.HexToAddress(rawTx.Address) {
			logger.Fatal("should not happen ==> in rawSlurpToSimple", s.To, rawTx.Address)
		}
	}

	s.SetRaw(rawTx)
	return s, nil
}

func getEtherscanUrl(chain, value string, requestType string, paginator *Paginator) (string, error) {
	var actions = map[string]string{
		"ext":         "txlist",
		"int":         "txlistinternal",
		"token":       "tokentx",
		"nfts":        "tokennfttx",
		"1155":        "token1155tx",
		"miner":       "getminedblocks&blocktype=blocks",
		"uncles":      "getminedblocks&blocktype=uncles",
		"byHash":      "eth_getTransactionByHash",
		"withdrawals": "txsBeaconWithdrawal&startblock=0&endblock=999999999",
	}

	if actions[requestType] == "" {
		return "", fmt.Errorf("should not happen (%s) ==> in getEtherscanUrl", requestType)
	}

	key := config.GetKey("etherscan").ApiKey
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

	return ret, nil
}

func (conn *Connection) getTxCountByAddressEs(chain, addr, requestType string, paginator *Paginator) (int, error) {
	_, cnt, err := conn.getTxsByAddressEs(chain, addr, requestType, paginator)
	return cnt, err
}
