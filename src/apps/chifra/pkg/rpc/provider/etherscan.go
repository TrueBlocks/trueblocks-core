package provider

import (
	"context"
	"encoding/json"
	"errors"
	"fmt"
	"net/http"
	"strings"
	"time"

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
	printProgress bool
	perPage       int
	baseUrl       string
	conn          *rpc.Connection
	limiter       *rate.Limiter
	// TODO: BOGUS - clean raw
	convertSlurpType func(address string, requestType string, rawTx *types.Slurp) (types.Slurp, error)
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
	Message string        `json:"message"`
	Result  []types.Slurp `json:"result"`
	Status  string        `json:"status"`
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
	fromEs := etherscanResponseBody{}
	sleepyTime := 50 * time.Millisecond

	attempts := 0
	var ret []SlurpedPageItem
	for {
		attempts++
		if len(ret) > 0 || attempts > 3 {
			paginator.SetDone(len(ret) < paginator.PerPage())
			return ret, len(ret), nil
		}
		resp, err := http.Get(url)
		if err != nil {
			if attempts > 3 {
				return ret, len(ret), err
			}
		}

		defer resp.Body.Close()
		if resp.StatusCode != http.StatusOK {
			if attempts > 3 {
				return ret, len(ret), fmt.Errorf("etherscan API error: %s", resp.Status)
			}
			time.Sleep(sleepyTime)
			sleepyTime *= 2
		}
		// Check server response
		decoder := json.NewDecoder(resp.Body)
		if err = decoder.Decode(&fromEs); err != nil {
			if attempts > 3 {
				if fromEs.Message == "NOTOK" {
					return ret, len(ret), fmt.Errorf("provider responded with: %s %s", url, fromEs.Message)
				}
				return ret, len(ret), fmt.Errorf("decoder failed: %w", err)
			}
			time.Sleep(sleepyTime)
			sleepyTime *= 2
		}

		if fromEs.Message == "NOTOK" {
			if attempts > 3 {
				// Etherscan sends 200 OK responses even if there's an error. We want to cache the error
				// response so we don't keep asking Etherscan for the same address. The user may later
				// remove empty ABIs with chifra abis --decache.
				if !utils.IsFuzzing() {
					logger.Warn("provider responded with:", url, fromEs.Message, strings.Repeat(" ", 40))
				}
				return ret, len(ret), nil
				// } else if fromEs.Message != "OK" {
				// 	logger.Warn("URL:", url)
				// 	logger.Warn("provider responded with:", url, fromEs.Message)
			}
			time.Sleep(sleepyTime)
			sleepyTime *= 2
		}

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
	}
}

// rawSlurpTo translate Slurp to Slurp. By default it uses `defaultConvertSlurpType`, but this can be changed, e.g. in tests
func (p *EtherscanProvider) rawSlurpTo(address string, requestType string, rawTx *types.Slurp) (types.Slurp, error) {
	return p.convertSlurpType(address, requestType, rawTx)
}

func (p *EtherscanProvider) defaultConvertSlurpType(address string, requestType string, rawTx *types.Slurp) (types.Slurp, error) {
	s := types.Slurp{
		Hash:             rawTx.Hash,
		BlockHash:        rawTx.BlockHash,
		BlockNumber:      rawTx.BlockNumber,
		TransactionIndex: rawTx.TransactionIndex,
		Timestamp:        rawTx.Timestamp,
		From:             rawTx.From,
		To:               rawTx.To,
		Gas:              rawTx.Gas,
		GasPrice:         rawTx.GasPrice,
		GasUsed:          rawTx.GasUsed,
		Input:            rawTx.Input,
		Value:            rawTx.Value,
		ContractAddress:  rawTx.ContractAddress,
		HasToken:         requestType == "nfts" || requestType == "token" || requestType == "1155",
	}
	// s.IsError = rawTx.TxReceiptStatus == "0"

	if requestType == "int" {
		// We use a weird marker here since Etherscan doesn't send the transaction id for internal txs and we don't
		// want to make another RPC call. We tried (see commented code), but EtherScan balks with a weird message
		app, _ := p.conn.GetTransactionAppByHash(s.Hash.Hex())
		s.TransactionIndex = base.Txnum(app.TransactionIndex)
	} else if requestType == "miner" {
		s.BlockHash = base.HexToHash("0xdeadbeef")
		s.TransactionIndex = types.BlockReward
		s.From = base.BlockRewardSender
		// TODO: This is only correct for Eth mainnet
		s.Value.SetString("5000000000000000000", 0)
		s.To = base.HexToAddress(address)
	} else if requestType == "uncles" {
		s.BlockHash = base.HexToHash("0xdeadbeef")
		s.TransactionIndex = types.UncleReward
		s.From = base.UncleRewardSender
		// TODO: This is only correct for Eth mainnet
		s.Value.SetString("3750000000000000000", 0)
		s.To = base.HexToAddress(address)
	} else if requestType == "withdrawals" {
		s.BlockHash = base.HexToHash("0xdeadbeef")
		s.TransactionIndex = types.WithdrawalAmt
		s.From = base.WithdrawalSender
		s.ValidatorIndex = rawTx.ValidatorIndex
		s.WithdrawalIndex = rawTx.WithdrawalIndex
		s.Value = rawTx.Amount
		s.To = base.HexToAddress(address)
	}
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
