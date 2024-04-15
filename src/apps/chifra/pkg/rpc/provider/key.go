package provider

import (
	"context"
	"encoding/json"
	"errors"
	"fmt"
	"strconv"
	"sync"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc/query"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"golang.org/x/time/rate"
)

const keyFirstPage = 0

// TODO: add to config
const keyRequestsPerSecond = 20
const keyMaxPerPage = 1000

type KeyProvider struct {
	PrintProgress bool
	perPage       int
	conn          *rpc.Connection
	limiter       *rate.Limiter
	baseUrl       string
	chain         string
}

func NewKeyProvider(conn *rpc.Connection, chain string) *KeyProvider {
	p := &KeyProvider{
		conn:    conn,
		chain:   chain,
		perPage: keyMaxPerPage,
	}
	p.PrintProgress = true
	p.limiter = rate.NewLimiter(keyRequestsPerSecond, keyRequestsPerSecond)

	return p
}

func (e *KeyProvider) TransactionsByAddress(ctx context.Context, query *Query, errorChan chan error) (txChan chan SlurpedTransaction) {
	txChan = make(chan SlurpedTransaction, channelBuffer)

	var appearanceChan chan types.SimpleAppearance
	appearanceChan = e.Appearances(ctx, query, errorChan)
	go func() {
		defer close(txChan)
		for {
			select {
			case <-ctx.Done():
				break
			case appearance, ok := <-appearanceChan:
				if !ok {
					return
				}
				tx, err := e.conn.GetTransactionByAppearance(&appearance, false)
				if err != nil {
					errorChan <- err
					continue
				}

				txChan <- SlurpedTransaction{
					Transaction: simpleTransactionToSimpleSlurp(tx),
					Address:     &appearance.Address,
				}
			}
		}
	}()

	return
}

func simpleTransactionToSimpleSlurp(tx *types.SimpleTransaction) *types.SimpleSlurp {
	return &types.SimpleSlurp{
		// ArticulatedTx:     tx.ArticulatedTx,
		BlockHash:    tx.BlockHash,
		BlockNumber:  tx.BlockNumber,
		CompressedTx: tx.CompressedTx,
		// ContractAddress:   tx.ContractAddress,
		// CumulativeGasUsed: tx.CumulativeGasUsed,
		// Ether:             tx.Ether,
		From: tx.From,
		// FunctionName:      tx.FunctionName,
		Gas:      tx.Gas,
		GasPrice: tx.GasPrice,
		GasUsed:  tx.GasUsed,
		HasToken: tx.HasToken,
		Hash:     tx.Hash,
		Input:    tx.Input,
		IsError:  tx.IsError,
		// MethodId:          tx.MethodId,
		Nonce:            tx.Nonce,
		Timestamp:        tx.Timestamp,
		To:               tx.To,
		TransactionIndex: tx.TransactionIndex,
		// TxReceiptStatus:   tx.TxReceiptStatus,
		// ValidatorIndex:    tx.ValidatorIndex,
		Value: tx.Value,
		// WithdrawalIndex:   tx.WithdrawalIndex,
	}
}

func (e *KeyProvider) Appearances(ctx context.Context, query *Query, errorChan chan error) (appChan chan types.SimpleAppearance) {
	appChan = make(chan types.SimpleAppearance, channelBuffer)

	totalFetched := 0
	totalFiltered := 0

	go func() {
		defer close(appChan)

		for _, address := range query.Addresses {
			for _, resource := range query.Resources {
				bar := logger.NewBar(logger.BarOptions{
					Type:    logger.Expanding,
					Enabled: e.PrintProgress,
					Prefix:  fmt.Sprintf("%s %s", utils.FormattedHash(false, address.String()), resource),
				})

				paginator := NewPageIdPaginator("", "latest", e.perPage)

				for !paginator.Done {
					select {
					case <-ctx.Done():
						return
					default:
						// TODO: Etherscan uses NextPage here. Should paginator only have NextPage and be direction agnostic?
						// TODO: If so, NextPage context (which page it is) could be set by e.fetchData
						if err := paginator.PreviousPage(); err != nil {
							errorChan <- err
							return
						}
						appearances, fetched, err := e.fetchData(ctx, address, paginator)
						totalFetched += fetched
						if err != nil {
							errorChan <- err
							continue
						}

						for _, appearance := range appearances {
							if ok, err := query.InRange(uint64(appearance.BlockNumber)); !ok {
								if err != nil {
									errorChan <- err
								}
								continue
							}
							totalFiltered++
							bar.Tick()

							appChan <- appearance
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

func (e *KeyProvider) Count(ctx context.Context, query *Query, errorChan chan error) (monitorChan chan types.SimpleMonitor) {
	monitorChan = make(chan types.SimpleMonitor)

	recordCount := make(map[base.Address]types.SimpleMonitor, len(query.Addresses))
	var mu sync.Mutex

	var appsChan chan types.SimpleAppearance
	appsChan = e.Appearances(ctx, query, errorChan)
	go func() {
		defer close(monitorChan)
		for {
			select {
			case <-ctx.Done():
				break
			case appearance, ok := <-appsChan:
				if !ok {
					for _, monitor := range recordCount {
						monitorChan <- monitor
					}
					return
				}
				mu.Lock()
				monitor := recordCount[appearance.Address]
				recordCount[appearance.Address] = types.SimpleMonitor{
					Address:  appearance.Address,
					NRecords: monitor.NRecords + 1,
				}
				mu.Unlock()
			}
		}
	}()

	return
}

type keyAppearancesRequestParam struct {
	Address string `json:"address"`
	PageId  string `json:"pageId"`
	PerPage int    `json:"perPage"`
}

type keyResponseBody struct {
	Data []KeyAppearance `json:"data"`
	Meta Meta            `json:"meta"`
}

type KeyAppearance struct {
	BlockNumber      string `json:"blockNumber"`
	TransactionIndex string `json:"transactionIndex"`
}

func (k *KeyAppearance) SimpleSlurp() (s types.SimpleSlurp, err error) {
	s = types.SimpleSlurp{}
	if err = json.Unmarshal([]byte(k.BlockNumber), &s.BlockNumber); err != nil {
		return
	}
	err = json.Unmarshal([]byte(k.TransactionIndex), &s.TransactionIndex)
	return
}

func (k *KeyAppearance) SimpleAppearance(address base.Address) (a types.SimpleAppearance, err error) {
	a = types.SimpleAppearance{
		Address: address,
	}
	if err = json.Unmarshal([]byte(k.BlockNumber), &a.BlockNumber); err != nil {
		return
	}
	err = json.Unmarshal([]byte(k.TransactionIndex), &a.TransactionIndex)
	return
}

type Meta struct {
	LastIndexedBlock lastIndexedBlock `json:"lastIndexedBlock,omitempty"`
	Address          string           `json:"address,omitempty"`
	PreviousPageId   string           `json:"previousPageId"`
	NextPageId       string           `json:"nextPageId"`
}

// The only purpose of this type is to parse Key's lastIndexedBlock (string) into int
type lastIndexedBlock int

func (l *lastIndexedBlock) UnmarshalJSON(data []byte) (err error) {
	var value int64
	var unquoted string
	unquoted, err = strconv.Unquote(string(data))
	if err != nil {
		return
	}

	value, err = strconv.ParseInt(unquoted, 0, 64)
	*l = lastIndexedBlock(value)
	return
}

func (e *KeyProvider) fetchData(ctx context.Context, address base.Address, paginator *PageIdPaginator) (data []types.SimpleAppearance, count int, err error) {
	if e.baseUrl == "" {
		e.baseUrl = config.GetChain(e.chain).KeyEndpoint
	}

	// TODO: since we don't use Paginator interface, we could just return string here:
	pageId, ok := paginator.Page().(string)
	if !ok {
		err = errors.New("cannot get page id")
		return
	}

	method := "tb_getAppearances"
	params := query.Params{keyAppearancesRequestParam{
		Address: address.Hex(),
		PageId:  pageId,
		PerPage: paginator.PerPage(),
	}}

	var response *keyResponseBody
	if response, err = query.QueryUrl[keyResponseBody](e.baseUrl, method, params); err != nil {
		return
	}

	data = make([]types.SimpleAppearance, 0, len(response.Data))
	for _, keyAppearance := range response.Data {
		appearance, err := keyAppearance.SimpleAppearance(base.HexToAddress(response.Meta.Address))
		if err != nil {
			return []types.SimpleAppearance{}, 0, err
		}
		data = append(data, appearance)
	}
	// update paginator
	paginator.SetNextPage(response.Meta.NextPageId)
	paginator.SetPreviousPage(response.Meta.PreviousPageId)
	paginator.Done = response.Meta.PreviousPageId == ""

	count = len(data)
	return
}
