package provider

import (
	"context"
	"encoding/json"
	"errors"
	"fmt"
	"io"
	"net/http"
	"strings"
	"time"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"golang.org/x/time/rate"
)

// const covalentFirstPage = 0
const covalentRequestsPerSecond = 5
const covalentBaseUrl = "https://api.covalenthq.com/v1/[{CHAIN}]/address/[{ADDRESS}]/transactions_v3/page/[{PAGE}]/"

var tbChainToCovalent = map[string]string{
	"mainnet": "eth-mainnet",
}

type CovalentProvider struct {
	printProgress bool
	conn          *rpc.Connection
	limiter       *rate.Limiter
	baseUrl       string
	chain         string
	apiKey        string
}

func NewCovalentProvider(conn *rpc.Connection, chain string) (p *CovalentProvider, err error) {
	apiKey := config.GetKey("covalent").ApiKey
	if apiKey == "" {
		err = errors.New("missing Covalent API key")
		return
	}
	p = &CovalentProvider{
		conn:    conn,
		chain:   chain,
		apiKey:  apiKey,
		baseUrl: covalentBaseUrl,
	}
	p.printProgress = true
	p.limiter = rate.NewLimiter(covalentRequestsPerSecond, covalentRequestsPerSecond)

	return
}

func (p *CovalentProvider) PrintProgress() bool {
	return p.printProgress
}

func (p *CovalentProvider) SetPrintProgress(print bool) {
	p.printProgress = print
}

func (p *CovalentProvider) NewPaginator(query *Query) Paginator {
	pageNumber := query.StartPage
	return NewPageNumberPaginator(pageNumber, pageNumber, int(query.PerPage))
}

func (p *CovalentProvider) TransactionsByAddress(ctx context.Context, query *Query, errorChan chan error) (txChan chan types.Slurp) {
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

func (p *CovalentProvider) Appearances(ctx context.Context, query *Query, errorChan chan error) (appChan chan types.Appearance) {
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

func (p *CovalentProvider) Count(ctx context.Context, query *Query, errorChan chan error) (monitorChan chan types.Monitor) {
	slurpedChan := fetchAndFilterData(ctx, p, query, errorChan, p.fetchData)
	return countSlurped(ctx, query, slurpedChan)
}

type covalentResponseBody struct {
	Data covalentResponseData `json:"data"`
}

type covalentResponseData struct {
	Items []covalentTransaction `json:"items"`
	Links *covalentLinks        `json:"links"`
}

type covalentTransaction struct {
	BlockHeight   *int       `json:"block_height,omitempty"`
	BlockHash     *string    `json:"block_hash,omitempty"`
	TxHash        *string    `json:"tx_hash,omitempty"`
	TxOffset      *int       `json:"tx_offset,omitempty"`
	Successful    *bool      `json:"successful,omitempty"`
	From          *string    `json:"from_address,omitempty"`
	To            *string    `json:"to_address,omitempty"`
	Value         *base.Wei  `json:"value,omitempty"`
	GasSpent      *int64     `json:"gas_spent,omitempty"`
	GasPrice      *int64     `json:"gas_price,omitempty"`
	BlockSignedAt *time.Time `json:"block_signed_at,omitempty"`
}

func (c *covalentTransaction) Slurp() (s types.Slurp) {
	s = types.Slurp{
		BlockHash:        base.HexToHash(*c.BlockHash),
		BlockNumber:      base.Blknum(*c.BlockHeight),
		From:             base.HexToAddress(*c.From),
		Gas:              base.Gas(*c.GasSpent),
		IsError:          !(*c.Successful),
		Timestamp:        c.BlockSignedAt.Unix(),
		To:               base.HexToAddress(*c.To),
		TransactionIndex: base.Txnum(*c.TxOffset),
		Value:            *c.Value,
	}

	return
}

func (c *covalentTransaction) Appearance(address base.Address) (a types.Appearance) {
	return types.Appearance{
		Address:          address,
		BlockNumber:      uint32(*c.BlockHeight),
		TransactionIndex: uint32(*c.TxOffset),
	}
}

type covalentLinks struct {
	Prev string `json:"prev,omitempty"`
	Next string `json:"next,omitempty"`
}

func (e *CovalentProvider) fetchData(ctx context.Context, address base.Address, paginator Paginator, _ string) (data []SlurpedPageItem, count int, err error) {
	if e.baseUrl == "" {
		e.baseUrl = covalentBaseUrl
	}

	pageNumber, ok := paginator.Page().(int)
	if !ok {
		err = errors.New("cannot get page number")
		return
	}

	url, err := e.url(address, pageNumber)
	if err != nil {
		return
	}

	var response covalentResponseBody
	request, err := http.NewRequestWithContext(ctx, http.MethodGet, url, nil)
	if err != nil {
		return
	}
	request.SetBasicAuth(e.apiKey, "")
	request.Header.Set("Content-Type", "application/json")

	resp, err := http.DefaultClient.Do(request)
	if err != nil {
		return
	}
	if resp.StatusCode != http.StatusOK {
		err = fmt.Errorf("covalent responded with: %s", resp.Status)
		paginator.SetDone(true)
		return
	}
	defer resp.Body.Close()
	respBytes, err := io.ReadAll(resp.Body)
	if err != nil {
		return
	}
	if err = json.Unmarshal(respBytes, &response); err != nil {
		return
	}

	data = make([]SlurpedPageItem, 0, len(response.Data.Items))
	for _, covalentTx := range response.Data.Items {
		appearance := covalentTx.Appearance(address)
		slurpedTx := covalentTx.Slurp()
		data = append(data, SlurpedPageItem{
			Appearance:  &appearance,
			Transaction: &slurpedTx,
		})
	}
	// update paginator
	paginator.SetDone(response.Data.Links.Next == "")

	count = len(data)
	return
}

func (p *CovalentProvider) url(address base.Address, pageNumber int) (url string, err error) {
	covalentChain := tbChainToCovalent[p.chain]
	if covalentChain == "" {
		err = fmt.Errorf("cannot find covalent chain ID for chain %s", p.chain)
		return
	}

	url = p.baseUrl
	url = strings.Replace(url, "[{CHAIN}]", covalentChain, -1)
	url = strings.Replace(url, "[{ADDRESS}]", address.Hex(), -1)
	url = strings.Replace(url, "[{PAGE}]", fmt.Sprint(pageNumber), -1)
	return
}
