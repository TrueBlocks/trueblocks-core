package provider

import (
	"context"
	"encoding/json"
	"errors"
	"fmt"
	"net/http"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/debug"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"golang.org/x/time/rate"
)

const etherscanFirstPage = 1
const etherscanRequestsPerSecond = 5
const etherscanMaxPerPage = 3000

var etherscanBaseUrl = "https://api.etherscan.io"

type EtherscanProvider struct {
	printProgress    bool
	perPage          int
	baseUrl          string
	conn             *rpc.Connection
	limiter          *rate.Limiter
	convertSlurpType func(address string, requestType string, rawTx *types.RawSlurp) (types.Slurp, error)
	apiKey           string
}

func NewEtherscanProvider(conn *rpc.Connection) (p *EtherscanProvider, err error) {
	apiKey := config.GetKey("etherscan").ApiKey
	if apiKey == "" {
		err = errors.New("missing Etherscan API key")
		return
	}

	p = &EtherscanProvider{
		conn:    conn,
		perPage: etherscanMaxPerPage,
		baseUrl: etherscanBaseUrl,
		apiKey:  apiKey,
	}
	p.printProgress = true
	p.limiter = rate.NewLimiter(etherscanRequestsPerSecond, etherscanRequestsPerSecond)
	p.convertSlurpType = p.defaultConvertSlurpType

	return
}

func (p *EtherscanProvider) PrintProgress() bool {
	return p.printProgress
}

func (p *EtherscanProvider) SetPrintProgress(print bool) {
	p.printProgress = print
}

func (p *EtherscanProvider) NewPaginator(query *Query) Paginator {
	pageNumber := query.StartPage
	if pageNumber == 0 {
		pageNumber = etherscanFirstPage
	}
	perPageValue := query.PerPage
	if perPageValue == 0 {
		perPageValue = etherscanMaxPerPage
	}
	return NewPageNumberPaginator(pageNumber, pageNumber, int(perPageValue))
}

func (p *EtherscanProvider) TransactionsByAddress(ctx context.Context, query *Query, errorChan chan error) (txChan chan types.Slurp) {
	txChan = make(chan types.Slurp, providerChannelBufferSize)

	slurpedChan := fetchAndFilterData(ctx, p, query, errorChan, p.fetchData)
	go func() {
		defer close(txChan)
		for {
			select {
			case <-ctx.Done():
				return
			case item, ok := <-slurpedChan:
				if !ok {
					return
				}
				txChan <- *item.Transaction
			}
		}
	}()

	return
}

func (p *EtherscanProvider) Appearances(ctx context.Context, query *Query, errorChan chan error) (appChan chan types.Appearance) {
	appChan = make(chan types.Appearance, providerChannelBufferSize)

	slurpedChan := fetchAndFilterData(ctx, p, query, errorChan, p.fetchData)
	go func() {
		defer close(appChan)
		for {
			select {
			case <-ctx.Done():
				return
			case item, ok := <-slurpedChan:
				if !ok {
					return
				}
				appChan <- *item.Appearance
			}
		}
	}()

	return
}

func (p *EtherscanProvider) Count(ctx context.Context, query *Query, errorChan chan error) (monitorChan chan types.Monitor) {
	slurpedChan := fetchAndFilterData(ctx, p, query, errorChan, p.fetchData)
	return countSlurped(ctx, query, slurpedChan)
}

type etherscanResponseBody struct {
	Message string           `json:"message"`
	Result  []types.RawSlurp `json:"result"`
	Status  string           `json:"status"`
}

func (p *EtherscanProvider) fetchData(ctx context.Context, address base.Address, paginator Paginator, requestType string) (data []SlurpedPageItem, count int, err error) {
	url, err := p.url(address.String(), paginator, requestType)
	if err != nil {
		return []SlurpedPageItem{}, 0, err
	}

	if err = p.limiter.Wait(ctx); err != nil {
		return
	}

	debug.DebugCurlStr(url)
	resp, err := http.Get(url)
	if err != nil {
		return []SlurpedPageItem{}, 0, err
	}
	defer resp.Body.Close()

	// Check server response
	if resp.StatusCode != http.StatusOK {
		return []SlurpedPageItem{}, 0, fmt.Errorf("etherscan API error: %s", resp.Status)
	}

	decoder := json.NewDecoder(resp.Body)
	fromEs := etherscanResponseBody{}
	if err = decoder.Decode(&fromEs); err != nil {
		if fromEs.Message == "NOTOK" {
			return []SlurpedPageItem{}, 0, fmt.Errorf("provider responded with: %s %s", url, fromEs.Message)
		}
		return []SlurpedPageItem{}, 0, fmt.Errorf("decoder failed: %w", err)
	}

	if fromEs.Message == "NOTOK" {
		// Etherscan sends 200 OK responses even if there's an error. We want to cache the error
		// response so we don't keep asking Etherscan for the same address. The user may later
		// remove empty ABIs with chifra abis --decache.
		if !utils.IsFuzzing() {
			logger.Warn("provider responded with:", url, fromEs.Message, strings.Repeat(" ", 40))
		}
		return []SlurpedPageItem{}, 0, nil
		// } else if fromEs.Message != "OK" {
		// 	logger.Warn("URL:", url)
		// 	logger.Warn("provider responded with:", url, fromEs.Message)
	}

	var ret []SlurpedPageItem
	for _, rawTx := range fromEs.Result {
		if transaction, err := p.rawSlurpTo(address.String(), requestType, &rawTx); err != nil {
			return nil, 0, err
		} else {
			ret = append(ret, SlurpedPageItem{
				Appearance: &types.Appearance{
					Address:          address,
					BlockNumber:      uint32(transaction.BlockNumber),
					TransactionIndex: uint32(transaction.TransactionIndex),
				},
				Transaction: &transaction,
			})
		}
	}

	fetchedCount := len(ret)
	paginator.SetDone(fetchedCount < paginator.PerPage())

	return ret, fetchedCount, nil
}

// rawSlurpTo translate RawSlurp to Slurp. By default it uses `defaultConvertSlurpType`, but this can be changed, e.g. in tests
func (p *EtherscanProvider) rawSlurpTo(address string, requestType string, rawTx *types.RawSlurp) (types.Slurp, error) {
	return p.convertSlurpType(address, requestType, rawTx)
}

func (p *EtherscanProvider) defaultConvertSlurpType(address string, requestType string, rawTx *types.RawSlurp) (types.Slurp, error) {
	s := types.Slurp{
		Hash:             base.HexToHash(rawTx.Hash),
		BlockHash:        base.HexToHash(rawTx.BlockHash),
		BlockNumber:      base.MustParseBlknum(rawTx.BlockNumber),
		TransactionIndex: base.MustParseNumeral(rawTx.TransactionIndex),
		Timestamp:        base.MustParseInt(rawTx.Timestamp),
		From:             base.HexToAddress(rawTx.From),
		To:               base.HexToAddress(rawTx.To),
		Gas:              base.MustParseNumeral(rawTx.Gas),
		GasPrice:         base.MustParseNumeral(rawTx.GasPrice),
		GasUsed:          base.MustParseNumeral(rawTx.GasUsed),
		Input:            rawTx.Input,
	}

	s.IsError = rawTx.TxReceiptStatus == "0"
	s.HasToken = requestType == "nfts" || requestType == "token" || requestType == "1155"
	s.Value.SetString(rawTx.Value, 0)
	s.ContractAddress = base.HexToAddress(rawTx.ContractAddress)

	if requestType == "int" {
		// We use a weird marker here since Etherscan doesn't send the transaction id for internal txs and we don't
		// want to make another RPC call. We tried (see commented code), but EtherScan balks with a weird message
		app, _ := p.conn.GetTransactionAppByHash(s.Hash.Hex())
		s.TransactionIndex = base.Txnum(app.TransactionIndex)
	} else if requestType == "miner" {
		s.BlockHash = base.HexToHash("0xdeadbeef")
		s.TransactionIndex = types.BlockReward
		s.From = base.BlockRewardSender
		// TODO: This is incorrect for mainnet
		s.Value.SetString("5000000000000000000", 0)
		s.To = base.HexToAddress(address)
	} else if requestType == "uncles" {
		s.BlockHash = base.HexToHash("0xdeadbeef")
		s.TransactionIndex = types.UncleReward
		s.From = base.UncleRewardSender
		// TODO: This is incorrect for mainnet
		s.Value.SetString("3750000000000000000", 0)
		s.To = base.HexToAddress(address)
	} else if requestType == "withdrawals" {
		s.BlockHash = base.HexToHash("0xdeadbeef")
		s.TransactionIndex = types.WithdrawalAmt
		s.From = base.WithdrawalSender
		s.ValidatorIndex = base.MustParseNumeral(rawTx.ValidatorIndex)
		s.WithdrawalIndex = base.MustParseNumeral(rawTx.WithdrawalIndex)
		s.Value.SetString(rawTx.Amount, 0)
		s.To = base.HexToAddress(address)
		if s.To != base.HexToAddress(rawTx.Address) {
			logger.Fatal("should not happen ==> in rawSlurpTo", s.To, rawTx.Address)
		}
	}

	s.SetRaw(rawTx)
	return s, nil
}

func (p *EtherscanProvider) url(value string, paginator Paginator, requestType string) (string, error) {
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
		return "", fmt.Errorf("cannot find Etherscan action %s", requestType)
	}

	module := "account"
	tt := "address"
	if requestType == "byHash" {
		module = "proxy"
		tt = "txhash"
	}

	const str = "[{BASE_URL}]/api?module=[{MODULE}]&sort=asc&action=[{ACTION}]&[{TT}]=[{VALUE}]&page=[{PAGE}]&offset=[{PER_PAGE}]"
	ret := strings.Replace(str, "[{BASE_URL}]", p.baseUrl, -1)
	ret = strings.Replace(ret, "[{MODULE}]", module, -1)
	ret = strings.Replace(ret, "[{TT}]", tt, -1)
	ret = strings.Replace(ret, "[{ACTION}]", actions[requestType], -1)
	ret = strings.Replace(ret, "[{VALUE}]", value, -1)
	ret = strings.Replace(ret, "[{PAGE}]", fmt.Sprintf("%d", paginator.Page()), -1)
	ret = strings.Replace(ret, "[{PER_PAGE}]", fmt.Sprintf("%d", paginator.PerPage()), -1)
	ret = ret + "&apikey=" + p.apiKey

	return ret, nil
}
