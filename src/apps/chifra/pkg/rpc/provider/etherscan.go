package provider

import (
	"context"
	"encoding/json"
	"errors"
	"fmt"
	"net/http"
	"strings"
	"sync"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/debug"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"golang.org/x/time/rate"
)

const channelBuffer = 50
const etherscanFirstPage = 1

// TODO: add to config
const etherscanRequestsPerSecond = 5
const etherscanMaxPerPage = 3000

var etherscanBaseUrl = "https://api.etherscan.io"

type EtherscanProvider struct {
	PrintProgress    bool
	perPage          int
	conn             *rpc.Connection
	limiter          *rate.Limiter
	convertSlurpType func(address string, requestType string, rawTx *types.RawSlurp) (types.SimpleSlurp, error)
}

func NewEtherscanProvider(conn *rpc.Connection) *EtherscanProvider {
	p := &EtherscanProvider{
		conn:    conn,
		perPage: etherscanMaxPerPage,
	}
	p.PrintProgress = true
	p.limiter = rate.NewLimiter(etherscanRequestsPerSecond, etherscanRequestsPerSecond)
	p.convertSlurpType = p.defaultConvertSlurpType

	return p
}

func (e *EtherscanProvider) TransactionsByAddress(ctx context.Context, query *Query, errorChan chan error) (txChan chan SlurpedTransaction) {
	txChan = make(chan SlurpedTransaction, channelBuffer)

	totalFetched := 0
	totalFiltered := 0

	go func() {
		defer close(txChan)

		for _, address := range query.Addresses {
			for _, resource := range query.Resources {
				bar := logger.NewBar(logger.BarOptions{
					Type:    logger.Expanding,
					Enabled: e.PrintProgress,
					Prefix:  fmt.Sprintf("%s %s", utils.FormattedHash(false, address.String()), resource),
				})

				paginator := NewPageNumberPaginator(etherscanFirstPage, etherscanFirstPage, e.perPage)

				for !paginator.Done {
					select {
					case <-ctx.Done():
						return
					default:
						if err := paginator.NextPage(); err != nil {
							errorChan <- err
							return
						}
						data, fetched, err := e.fetchData(ctx, address, resource, paginator)
						totalFetched += fetched
						if err != nil {
							errorChan <- err
							continue
						}

						for _, tx := range data {
							if ok, err := query.InRange(tx.BlockNumber); !ok {
								if err != nil {
									errorChan <- err
								}
								continue
							}
							totalFiltered++
							bar.Tick()

							txChan <- SlurpedTransaction{
								Address:     &address,
								Transaction: &tx,
							}
						}
					}
				}
				bar.Finish(true /* newLine */)
			}
			if totalFiltered == 0 {
				msg := fmt.Sprintf("zero transactions reported, %d fetched", totalFetched)
				errorChan <- fmt.Errorf(msg)
			}
		}
	}()

	return
}

func (e *EtherscanProvider) Appearances(ctx context.Context, query *Query, errorChan chan error) (appChan chan types.SimpleAppearance) {
	appChan = make(chan types.SimpleAppearance, channelBuffer)

	var slurpedTxChan chan SlurpedTransaction
	slurpedTxChan = e.TransactionsByAddress(ctx, query, errorChan)
	go func() {
		defer close(appChan)
		for {
			select {
			case <-ctx.Done():
				break
			case slurpedTx, ok := <-slurpedTxChan:
				if !ok {
					return
				}
				appChan <- types.SimpleAppearance{
					Address:          *slurpedTx.Address,
					BlockNumber:      uint32(slurpedTx.Transaction.BlockNumber),
					TransactionIndex: uint32(slurpedTx.Transaction.TransactionIndex),
					Timestamp:        slurpedTx.Transaction.Timestamp,
				}
			}
		}
	}()

	return
}

func (e *EtherscanProvider) Count(ctx context.Context, query *Query, errorChan chan error) (monitorChan chan types.SimpleMonitor) {
	monitorChan = make(chan types.SimpleMonitor)

	recordCount := make(map[base.Address]types.SimpleMonitor, len(query.Addresses))
	var mu sync.Mutex

	var slurpedTxChan chan SlurpedTransaction
	slurpedTxChan = e.TransactionsByAddress(ctx, query, errorChan)
	go func() {
		defer close(monitorChan)
		for {
			select {
			case <-ctx.Done():
				break
			case slurpedTx, ok := <-slurpedTxChan:
				if !ok {
					for _, monitor := range recordCount {
						monitorChan <- monitor
					}
					return
				}
				mu.Lock()
				monitor := recordCount[*slurpedTx.Address]
				recordCount[*slurpedTx.Address] = types.SimpleMonitor{
					Address:  *slurpedTx.Address,
					NRecords: monitor.NRecords + 1,
				}
				mu.Unlock()
			}
		}
	}()

	return
}

type etherscanResponseBody struct {
	Message string           `json:"message"`
	Result  []types.RawSlurp `json:"result"`
	Status  string           `json:"status"`
}

func (e *EtherscanProvider) fetchData(ctx context.Context, address base.Address, requestType string, paginator *PageNumberPaginator) (data []types.SimpleSlurp, count int, err error) {
	url, err := e.url(address.String(), paginator, requestType)
	if err != nil {
		return []types.SimpleSlurp{}, 0, err
	}

	if err = e.limiter.Wait(ctx); err != nil {
		return
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
	fromEs := etherscanResponseBody{}
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
		if transaction, err := e.rawSlurpToSimple(address.String(), requestType, &rawTx); err != nil {
			return nil, 0, err
		} else {
			ret = append(ret, transaction)
		}
	}

	fetchedCount := len(ret)
	paginator.Done = fetchedCount < paginator.PerPage()

	return ret, fetchedCount, nil
}

func (e *EtherscanProvider) rawSlurpToSimple(address string, requestType string, rawTx *types.RawSlurp) (types.SimpleSlurp, error) {
	return e.convertSlurpType(address, requestType, rawTx)
}

func (e *EtherscanProvider) defaultConvertSlurpType(address string, requestType string, rawTx *types.RawSlurp) (types.SimpleSlurp, error) {
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
		app, _ := e.conn.GetTransactionAppByHash(s.Hash.Hex())
		s.TransactionIndex = uint64(app.TransactionIndex)
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
		s.TransactionIndex = types.Withdrawal
		s.From = base.WithdrawalSender
		s.ValidatorIndex = utils.MustParseUint(rawTx.ValidatorIndex)
		s.WithdrawalIndex = utils.MustParseUint(rawTx.WithdrawalIndex)
		s.Value.SetString(rawTx.Amount, 0)
		s.To = base.HexToAddress(address)
		if s.To != base.HexToAddress(rawTx.Address) {
			logger.Fatal("should not happen ==> in rawSlurpToSimple", s.To, rawTx.Address)
		}
	}

	s.SetRaw(rawTx)
	return s, nil
}

func (e *EtherscanProvider) url(value string, paginator *PageNumberPaginator, requestType string) (string, error) {
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

	const str = "[{BASE_URL}]/api?module=[{MODULE}]&sort=asc&action=[{ACTION}]&[{TT}]=[{VALUE}]&page=[{PAGE}]&offset=[{PER_PAGE}]"
	ret := strings.Replace(str, "[{BASE_URL}]", etherscanBaseUrl, -1)
	ret = strings.Replace(ret, "[{MODULE}]", module, -1)
	ret = strings.Replace(ret, "[{TT}]", tt, -1)
	ret = strings.Replace(ret, "[{ACTION}]", actions[requestType], -1)
	ret = strings.Replace(ret, "[{VALUE}]", value, -1)
	ret = strings.Replace(ret, "[{PAGE}]", fmt.Sprintf("%d", paginator.Page()), -1)
	ret = strings.Replace(ret, "[{PER_PAGE}]", fmt.Sprintf("%d", paginator.PerPage()), -1)
	ret = ret + "&apikey=" + key

	return ret, nil
}
