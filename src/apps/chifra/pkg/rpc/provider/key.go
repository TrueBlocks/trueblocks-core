package provider

import (
	"context"
	"encoding/json"
	"errors"
	"strconv"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc/query"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"golang.org/x/time/rate"
)

const keyFirstPage = "latest"
const keyRequestsPerSecond = 20
const keyMaxPerPage = 1000

type KeyProvider struct {
	printProgress bool
	perPage       int
	conn          *rpc.Connection
	limiter       *rate.Limiter
	baseUrl       string
	chain         string
}

func NewKeyProvider(conn *rpc.Connection, chain string) (p *KeyProvider, err error) {
	keyEndpoint := config.GetChain(chain).KeyEndpoint
	if keyEndpoint == "" {
		err = errors.New("missing Key endpoint URL")
		return
	}

	p = &KeyProvider{
		conn:    conn,
		chain:   chain,
		perPage: keyMaxPerPage,
		baseUrl: keyEndpoint,
	}
	p.printProgress = true
	p.limiter = rate.NewLimiter(keyRequestsPerSecond, keyRequestsPerSecond)

	return
}

func (p *KeyProvider) PrintProgress() bool {
	return p.printProgress
}

func (p *KeyProvider) SetPrintProgress(print bool) {
	p.printProgress = print
}

func (p *KeyProvider) NewPaginator() Paginator {
	return NewPageIdPaginator(keyFirstPage, keyFirstPage, p.perPage)
}

func (p *KeyProvider) TransactionsByAddress(ctx context.Context, query *Query, errorChan chan error) (txChan chan types.SimpleSlurp) {
	txChan = make(chan types.SimpleSlurp, providerChannelBufferSize)

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
				tx, err := p.conn.GetTransactionByAppearance(item.Appearance, false)
				if err != nil {
					errorChan <- err
					continue
				}
				txChan <- *(simpleTransactionToSimpleSlurp(tx))
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

func (p *KeyProvider) Appearances(ctx context.Context, query *Query, errorChan chan error) (appChan chan types.SimpleAppearance) {
	appChan = make(chan types.SimpleAppearance, providerChannelBufferSize)

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

func (p *KeyProvider) Count(ctx context.Context, query *Query, errorChan chan error) (monitorChan chan types.SimpleMonitor) {
	slurpedChan := fetchAndFilterData(ctx, p, query, errorChan, p.fetchData)
	return countSlurped(ctx, query, slurpedChan)
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

func (e *KeyProvider) fetchData(ctx context.Context, address base.Address, paginator Paginator, _ string) (data []SlurpedPageItem, count int, err error) {
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

	data = make([]SlurpedPageItem, 0, len(response.Data))
	for _, keyAppearance := range response.Data {
		appearance, err := keyAppearance.SimpleAppearance(base.HexToAddress(response.Meta.Address))
		if err != nil {
			return []SlurpedPageItem{}, 0, err
		}
		data = append(data, SlurpedPageItem{
			Appearance: &appearance,
		})
	}
	// update paginator
	_ = paginator.SetNextPage(response.Meta.PreviousPageId)
	paginator.SetDone(response.Meta.PreviousPageId == "")

	count = len(data)
	return
}
