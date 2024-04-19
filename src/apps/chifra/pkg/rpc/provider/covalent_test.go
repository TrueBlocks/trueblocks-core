package provider

import (
	"context"
	"encoding/json"
	"net/http"
	"net/http/httptest"
	"strings"
	"testing"
	"time"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/identifiers"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"golang.org/x/time/rate"
)

func TestCovalentProvider_url(t *testing.T) {
	paginator := NewPageNumberPaginator(0, 0, 0)
	provider := CovalentProvider{
		chain:   "mainnet",
		apiKey:  "fake",
		baseUrl: covalentBaseUrl,
	}
	var err error
	var result string
	var expected string
	var pageNumber int
	var ok bool

	pageNumber, ok = paginator.Page().(int)
	if !ok {
		t.Fatal("cannot cast page to int")
	}

	result, err = provider.url(
		base.HexToAddress("0xf503017d7baf7fbc0fff7492b751025c6a78179b"),
		pageNumber,
	)
	if err != nil {
		t.Fatal(err)
	}

	expected = "https://api.covalenthq.com/v1/eth-mainnet/address/0xf503017d7baf7fbc0fff7492b751025c6a78179b/transactions_v3/page/0/"
	if result != expected {
		t.Fatal("wrong value", result)
	}

	// Change page

	if err = paginator.NextPage(); err != nil {
		t.Fatal(err)
	}
	pageNumber, ok = paginator.Page().(int)
	if !ok {
		t.Fatal("cannot cast page to int")
	}

	result, err = provider.url(
		base.HexToAddress("0xf503017d7baf7fbc0fff7492b751025c6a78179b"),
		pageNumber,
	)
	if err != nil {
		t.Fatal(err)
	}

	expected = "https://api.covalenthq.com/v1/eth-mainnet/address/0xf503017d7baf7fbc0fff7492b751025c6a78179b/transactions_v3/page/1/"
	if result != expected {
		t.Fatal("wrong value", result)
	}
}

func mockCovalentServer(t *testing.T) (ts *httptest.Server) {
	t.Helper()

	pages := []covalentResponseBody{
		{
			Data: covalentResponseData{
				Items: []covalentTransaction{
					{
						BlockHeight: utils.PointerOf(1),
						TxOffset:    utils.PointerOf(1),

						BlockHash:     utils.PointerOf(""),
						From:          utils.PointerOf(""),
						GasSpent:      utils.PointerOf(int64(0)),
						Successful:    utils.PointerOf(false),
						BlockSignedAt: &time.Time{},
						To:            utils.PointerOf(""),
						Value:         &base.Wei{},
					},
					{
						BlockHeight: utils.PointerOf(1),
						TxOffset:    utils.PointerOf(2),

						BlockHash:     utils.PointerOf(""),
						From:          utils.PointerOf(""),
						GasSpent:      utils.PointerOf(int64(0)),
						Successful:    utils.PointerOf(false),
						BlockSignedAt: &time.Time{},
						To:            utils.PointerOf(""),
						Value:         &base.Wei{},
					},
					{
						BlockHeight: utils.PointerOf(1),
						TxOffset:    utils.PointerOf(3),

						BlockHash:     utils.PointerOf(""),
						From:          utils.PointerOf(""),
						GasSpent:      utils.PointerOf(int64(0)),
						Successful:    utils.PointerOf(false),
						BlockSignedAt: &time.Time{},
						To:            utils.PointerOf(""),
						Value:         &base.Wei{},
					},
				},
				Links: &covalentLinks{
					Next: "/1",
				},
			},
		},
		{
			Data: covalentResponseData{
				Items: []covalentTransaction{
					{
						BlockHeight: utils.PointerOf(2),
						TxOffset:    utils.PointerOf(1),

						BlockHash:     utils.PointerOf(""),
						From:          utils.PointerOf(""),
						GasSpent:      utils.PointerOf(int64(0)),
						Successful:    utils.PointerOf(false),
						BlockSignedAt: &time.Time{},
						To:            utils.PointerOf(""),
						Value:         &base.Wei{},
					},
				},
				Links: &covalentLinks{},
			},
		},
	}
	ts = httptest.NewServer(http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {
		var result covalentResponseBody
		switch r.URL.Path {
		case "/0":
			result = pages[0]
		case "/1":
			result = pages[1]
		default:
			result = covalentResponseBody{}
		}

		b, err := json.Marshal(result)
		if err != nil {
			t.Fatal(err)
		}
		w.Write(b)
	}))

	return ts
}

func TestCovalentProvider_fetchData(t *testing.T) {
	ts := mockCovalentServer(t)
	defer ts.Close()

	provider := CovalentProvider{
		chain:   "mainnet",
		baseUrl: ts.URL + "/[{PAGE}]",
	}
	provider.limiter = rate.NewLimiter(5, 5)
	paginator := provider.NewPaginator()

	var data []SlurpedPageItem
	// var count int
	var err error
	data, _, err = provider.fetchData(context.TODO(), base.HexToAddress("0xf503017d7baf7fbc0fff7492b751025c6a78179b"), paginator, "int")
	if err != nil {
		t.Fatal(err)
	}

	if l := len(data); l == 0 {
		t.Fatal("empty page")
	}
	if paginator.Done() {
		t.Fatal("paginator done but it should not be")
	}

	if err = paginator.NextPage(); err != nil {
		t.Fatal(err)
	}

	data, _, err = provider.fetchData(context.TODO(), base.HexToAddress("0xf503017d7baf7fbc0fff7492b751025c6a78179b"), paginator, "int")
	if err != nil {
		t.Fatal(err)
	}
	if l := len(data); l == 0 {
		t.Fatal("empty page")
	}
	if !paginator.Done() {
		t.Fatal("paginator should be done")
	}
}

func TestCovalentProvider_TransactionsByAddress(t *testing.T) {
	ts := mockCovalentServer(t)
	defer ts.Close()

	provider := CovalentProvider{
		chain:   "mainnet",
		baseUrl: ts.URL + "/[{PAGE}]",
	}
	provider.limiter = rate.NewLimiter(5, 5)

	query := &Query{
		Addresses: []base.Address{
			base.HexToAddress("0xf503017d7baf7fbc0fff7492b751025c6a78179b"),
		},
		Resources: []string{"int"},
	}
	ctx, cancel := context.WithCancel(context.Background())
	defer cancel()
	errors := make(chan error)
	results := provider.TransactionsByAddress(ctx, query, errors)

	count := 0
LOOP:
	for {
		select {
		case err, ok := <-errors:
			if ok {
				cancel()
				t.Fatal(err)
			}
		case data, ok := <-results:
			if !ok {
				break LOOP
			}
			t.Log("got data", data)
			count++
		}
	}

	if count != 4 {
		t.Fatal("wrong count:", count)
	}

	// Filter: all txs filtered out

	query.BlockRange = []identifiers.Identifier{
		{
			Orig: "14000000",
		},
	}
	results = provider.TransactionsByAddress(ctx, query, errors)
	err := <-errors
	if err == nil {
		t.Fatal("expected error")
	}
	if !strings.Contains(err.Error(), "zero transactions reported") {
		t.Fatal("expected zero transactions reported error")
	}
}

func TestCovalentProvider_Appearances(t *testing.T) {
	perPage := 3
	ts := mockEtherscanServer(t)
	defer ts.Close()

	provider := EtherscanProvider{
		perPage: perPage,
		baseUrl: ts.URL,
	}
	provider.limiter = rate.NewLimiter(5, 5)
	provider.convertSlurpType = mockConvertSlurpType(t)

	query := &Query{
		Addresses: []base.Address{
			base.HexToAddress("0xf503017d7baf7fbc0fff7492b751025c6a78179b"),
		},
		Resources: []string{"int"},
	}
	ctx, cancel := context.WithCancel(context.Background())
	defer cancel()
	errors := make(chan error)
	results := provider.Appearances(ctx, query, errors)

	count := 0
LOOP:
	for {
		select {
		case err, ok := <-errors:
			if ok {
				cancel()
				t.Fatal(err)
			}
		case data, ok := <-results:
			if !ok {
				break LOOP
			}
			t.Log("got data", data)
			count++
		}
	}

	if count != 4 {
		t.Fatal("wrong count:", count)
	}
}

func TestCovalentProvider_Count(t *testing.T) {
	perPage := 3
	ts := mockEtherscanServer(t)
	defer ts.Close()

	provider := EtherscanProvider{
		perPage: perPage,
		baseUrl: ts.URL,
	}
	provider.limiter = rate.NewLimiter(5, 5)
	provider.convertSlurpType = mockConvertSlurpType(t)

	query := &Query{
		Addresses: []base.Address{
			base.HexToAddress("0xf503017d7baf7fbc0fff7492b751025c6a78179b"),
		},
		Resources: []string{"int"},
	}
	ctx, cancel := context.WithCancel(context.Background())
	defer cancel()
	errors := make(chan error)

	results := provider.Count(ctx, query, errors)

	count := make([]types.SimpleMonitor, 0, 1)
LOOP:
	for {
		select {
		case err, ok := <-errors:
			if ok {
				cancel()
				t.Fatal(err)
			}
		case data, ok := <-results:
			if !ok {
				break LOOP
			}
			count = append(count, data)

		}
	}

	if l := len(count); l != 1 {
		t.Fatal("wrong len:", l)
	}
	if n := count[0].NRecords; n != 4 {
		t.Fatal("wrong NRecords", n)
	}
}
