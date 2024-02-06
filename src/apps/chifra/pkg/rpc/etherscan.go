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
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/debug"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc/query"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

type Paginator struct {
	Page    int
	PerPage int
}

func (conn *Connection) SlurpTxsByAddress(chain, source, addr, requestType string, paginator *Paginator) ([]types.SimpleSlurp, int, error) {
	switch source {
	case "key":
		return conn.getTxsByAddressKey(chain, addr, paginator)
	case "etherscan":
		return conn.getTxsByAddressEs(chain, addr, requestType, paginator)
	default:
		return []types.SimpleSlurp{}, 0, fmt.Errorf("unknown source: %s", source)
	}
}

func (conn *Connection) getTxsByAddressKey(chain, addr string, paginator *Paginator) ([]types.SimpleSlurp, int, error) {
	url := "https://1fc17zhbqd.execute-api.us-east-1.amazonaws.com/prod/rpc"

	key := config.GetKey("trueblocks").Jwt
	if key == "" {
		return []types.SimpleSlurp{}, 0, errors.New("cannot read API key")
	}

	type keyParam struct {
		Address  string `json:"Address"`
		Page     int    `json:"Page"`
		PageSize int    `json:"PageSize"`
	}

	method := "tb_getAppearances"
	params := query.Params{keyParam{
		Address:  addr,
		Page:     paginator.Page,
		PageSize: paginator.PerPage,
	}}
	headers := map[string]string{
		"x-quicknode-id": "test-trueblocks-1",
		"x-instance-id":  "foobar",
		"x-qn-chain":     "ethereum",
		"x-qn-network":   "mainnet",
		"Authorization":  "Basic " + key,
	}

	if apps, err := query.QueryWithHeaders[[]types.SimpleSlurp](url, headers, method, params); err != nil {
		return []types.SimpleSlurp{}, 0, err
	} else {
		return *apps, len(*apps), nil
	}

	// url, err := conn.getKeyUrl(chain, addr, paginator)
	// if err != nil {
	// 	return []types.SimpleSlurp{}, 0, err
	// }

	// url := fmt.Sprintf(listPins, status, first, cnt)

	// if req, err := http.NewRequest("GET", url, nil); err != nil {
	// 	return 0, []Pin{}
	// } else {
	// 	s, _ := NewService(chain, Pinata)
	// 	if s.HeaderFunc != nil {
	// 		headers := s.HeaderFunc(&s, "application/json")
	// 		for key, value := range headers {
	// 			req.Header.Add(key, value)
	// 		}
	// 	}
	// 	if res, err := http.DefaultClient.Do(req); err != nil {
	// 		return 0, []Pin{}
	// 	} else {
	// 		defer res.Body.Close()
	// 		data := PinSet{}
	// 		decoder := json.NewDecoder(res.Body)
	// 		if err = decoder.Decode(&data); err != nil {
	// 			return 0, []Pin{}
	// 		}
	// 		return data.Count, data.Rows
	// 	}
	// }

	// resp, err := http.Get(url)
	// if err != nil {
	// 	return []types.SimpleSlurp{}, 0, err
	// }
	// defer resp.Body.Close()
	// if resp.StatusCode != http.StatusOK {
	// 	return []types.SimpleSlurp{}, 0, fmt.Errorf("API error: %s", resp.Status)
	// }

	// decoder := json.NewDecoder(resp.Body)
	// fromEs := struct {
	// 	Result []types.RawSlurp `json:"result"`
	// }{}
	// if err = decoder.Decode(&fromEs); err != nil {
	// 	return []types.SimpleSlurp{}, 0, err
	// }

	// ret := make([]types.SimpleSlurp, len(fromEs.Result))
	// for _, raw := range fromEs.Result {
	// 	if s, err := conn.rawToSimple(addr, "ext", &raw); err != nil {
	// 		return []types.SimpleSlurp{}, 0, err
	// 	} else {
	// 		// s.Address
	// 		ret = append(ret, s)
	// 	}
	// }
	// return ret, len(ret), nil
}

func (conn *Connection) getTxsByAddressEs(chain, addr string, requestType string, paginator *Paginator) ([]types.SimpleSlurp, int, error) {
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
		return []types.SimpleSlurp{}, 0, err
	}

	if fromEs.Message == "NOTOK" {
		// Etherscan sends 200 OK responses even if there's an error. We want to cache the error
		// response so we don't keep asking Etherscan for the same address. The user may later
		// remove empty ABIs with chifra abis --decache.
		logger.Warn("provider responded with:", url, fromEs.Message, ss)
		return []types.SimpleSlurp{}, 0, nil
		// } else if fromEs.Message != "OK" {
		// 	logger.Warn("URL:", url)
		// 	logger.Warn("provider responded with:", url, fromEs.Message)
	}

	return conn.responseToTransactions(addr, requestType, fromEs.Result)
}

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
			logger.Fatal("should not happen ==> in rawToSimple", s.To, rawTx.Address)
		}
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
		logger.Fatal("should not happen ==> in getEtherscanUrl", requestType)
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

	paginator.Page++

	return ret, nil
}

func mustParseInt(input any) (result int64) {
	result, _ = strconv.ParseInt(fmt.Sprint(input), 0, 64)
	return
}

var ss = strings.Repeat(" ", 40)
