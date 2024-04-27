package provider

import (
	"context"
	"errors"
	"strconv"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc/query"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"golang.org/x/time/rate"
)

const alchemyFirstPage = ""
const alchemyRequestsPerSecond = 5 // 330 Compute Units per second
// const alchemyMaxPerPage = 1000
const alchemyBaseUrl = "https://eth-mainnet.g.alchemy.com/v2/"

func alchemyPrepareQuery(q *Query) (result *Query, err error) {
	result = q.Dup()
	result.Resources = make([]string, 0, len(q.Resources)*2)
	for _, resource := range q.Resources {
		cat := alchemyCategory(resource)
		if cat == "" {
			// Ignore unknown categories, so the user can simply use
			// `slurp --types all`
			continue
		}
		result.Resources = append(result.Resources, cat+":to", cat+":from")
	}

	return
}

func alchemyCategory(requestType string) (category string) {
	switch requestType {
	case "ext":
		category = "external"
	case "int":
		category = "internal"
	case "token":
		category = "erc20"
	case "nfts":
		category = "erc721"
	case "1155":
		category = "erc1155"
	}
	return
}

type AlchemyProvider struct {
	printProgress            bool
	perPage                  int //nolint:unused
	conn                     *rpc.Connection
	limiter                  *rate.Limiter
	baseUrl                  string
	chain                    string
	getTransactionAppearance func(hash string) (types.RawAppearance, error)
}

func NewAlchemyProvider(conn *rpc.Connection, chain string) (p *AlchemyProvider, err error) {
	apiKey := config.GetKey("alchemy").ApiKey
	if apiKey == "" {
		err = errors.New("missing Alchemy API key")
		return
	}

	p = &AlchemyProvider{
		conn:    conn,
		chain:   chain,
		baseUrl: alchemyBaseUrl + apiKey,
	}
	p.printProgress = true
	p.limiter = rate.NewLimiter(alchemyRequestsPerSecond, alchemyRequestsPerSecond)
	p.getTransactionAppearance = p.defaultGetTransactionAppearance

	return
}

func (p *AlchemyProvider) PrintProgress() bool {
	return p.printProgress
}

func (p *AlchemyProvider) SetPrintProgress(print bool) {
	p.printProgress = print
}

func (p *AlchemyProvider) NewPaginator(firstPage any, perPage int) Paginator {
	pageId, ok := firstPage.(string)
	if !ok {
		pageId = alchemyFirstPage
	}

	return NewPageIdPaginator(pageId, pageId, perPage)
}

func (p *AlchemyProvider) TransactionsByAddress(ctx context.Context, query *Query, errorChan chan error) (txChan chan types.Slurp) {
	txChan = make(chan types.Slurp, providerChannelBufferSize)

	prepQuery, err := alchemyPrepareQuery(query)
	if err != nil {
		errorChan <- err
		return
	}
	slurpedChan := fetchAndFilterData(ctx, p, prepQuery, errorChan, p.fetchData)
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

func (p *AlchemyProvider) Appearances(ctx context.Context, query *Query, errorChan chan error) (appChan chan types.Appearance) {
	appChan = make(chan types.Appearance, providerChannelBufferSize)

	prepQuery, err := alchemyPrepareQuery(query)
	if err != nil {
		errorChan <- err
		return
	}
	slurpedChan := fetchAndFilterData(ctx, p, prepQuery, errorChan, p.fetchData)
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

func (p *AlchemyProvider) Count(ctx context.Context, query *Query, errorChan chan error) (monitorChan chan types.Monitor) {
	prepQuery, err := alchemyPrepareQuery(query)
	if err != nil {
		errorChan <- err
		return
	}
	slurpedChan := fetchAndFilterData(ctx, p, prepQuery, errorChan, p.fetchData)
	return countSlurped(ctx, query, slurpedChan)
}

type alchemyRequestParam struct {
	ToBlock     string   `json:"toBlock,omitempty"`
	ToAddress   string   `json:"toAddress,omitempty"`
	FromAddress string   `json:"fromAddress,omitempty"`
	Category    []string `json:"category,omitempty"`
	PageKey     string   `json:"pageKey,omitempty"`
}

type alchemyResponseBody struct {
	Transfers []AlchemyTx `json:"transfers"`
	PageKey   string      `json:"pageKey"`
}

type AlchemyTx struct {
	BlockNumber string `json:"blockNum"`
	Hash        string `json:"hash"`
	From        string `json:"from"`
	To          string `json:"to"`
}

func (tx *AlchemyTx) SimpleSlurp() (s types.Slurp, err error) {
	bn, err := strconv.ParseUint(tx.BlockNumber, 0, 64)
	if err != nil {
		return
	}

	s = types.Slurp{
		BlockNumber: bn,
		Hash:        base.HexToHash(tx.Hash),
		From:        base.HexToAddress(tx.From),
		To:          base.HexToAddress(tx.To),
	}

	return
}

func (e *AlchemyProvider) fetchData(ctx context.Context, address base.Address, paginator Paginator, categoryToken string) (data []SlurpedPageItem, count int, err error) {
	pageKey, ok := paginator.Page().(string)
	if !ok {
		err = errors.New("cannot get page id")
		return
	}

	// categoryToken has form of alchemyCategory[:from|to]
	category := strings.Split(categoryToken, ":")

	method := "alchemy_getAssetTransfers"
	requestParam := alchemyRequestParam{
		ToBlock:  "latest",
		Category: []string{category[0]},
		PageKey:  pageKey,
	}
	if len(category) > 1 && category[1] == "to" {
		requestParam.ToAddress = address.Hex()
	} else {
		requestParam.FromAddress = address.Hex()
	}
	params := query.Params{requestParam}

	var response *alchemyResponseBody
	if response, err = query.QueryUrl[alchemyResponseBody](e.baseUrl, method, params); err != nil {
		return
	}

	// log.Printf("Got: %+v\n", response)

	data = make([]SlurpedPageItem, 0, len(response.Transfers))
	for _, alchemyTx := range response.Transfers {
		app, err := e.getTransactionAppearance(alchemyTx.Hash)
		if err != nil {
			return []SlurpedPageItem{}, 0, err
		}
		tx, err := alchemyTx.SimpleSlurp()
		if err != nil {
			return []SlurpedPageItem{}, 0, err
		}
		tx.TransactionIndex = uint64(app.TransactionIndex)
		data = append(data, SlurpedPageItem{
			Appearance: &types.Appearance{
				TransactionIndex: app.TransactionIndex,
				BlockNumber:      app.BlockNumber,
				Address:          address,
			},
			Transaction: &tx,
		})
	}
	// update paginator
	_ = paginator.SetNextPage(response.PageKey)
	paginator.SetDone(response.PageKey == "")

	count = len(data)
	return
}

func (p *AlchemyProvider) defaultGetTransactionAppearance(hash string) (types.RawAppearance, error) {
	return p.conn.GetTransactionAppByHash(hash)
}
