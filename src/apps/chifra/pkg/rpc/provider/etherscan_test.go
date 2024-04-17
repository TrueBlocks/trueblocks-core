package provider

import (
	"context"
	"encoding/json"
	"net/http"
	"net/http/httptest"
	"strconv"
	"strings"
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/identifiers"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"golang.org/x/time/rate"
)

func Test_url(t *testing.T) {
	paginator := NewPageNumberPaginator(1, 1, 10)
	provider := &EtherscanProvider{
		baseUrl: etherscanBaseUrl,
	}
	var err error
	var result string
	var expected string

	result, err = provider.url(
		"0xf503017d7baf7fbc0fff7492b751025c6a78179b",
		paginator,
		"int",
	)
	if err != nil {
		t.Fatal(err)
	}

	result = result[:142]
	expected = "https://api.etherscan.io/api?module=account&sort=asc&action=txlistinternal&address=0xf503017d7baf7fbc0fff7492b751025c6a78179b&page=1&offset=10"
	if result != expected {
		t.Fatal("wrong value", result)
	}

	// Change page

	if err = paginator.NextPage(); err != nil {
		t.Fatal(err)
	}

	result, err = provider.url(
		"0xf503017d7baf7fbc0fff7492b751025c6a78179b",
		paginator,
		"int",
	)
	if err != nil {
		t.Fatal(err)
	}

	result = result[:142]
	expected = "https://api.etherscan.io/api?module=account&sort=asc&action=txlistinternal&address=0xf503017d7baf7fbc0fff7492b751025c6a78179b&page=2&offset=10"
	if result != expected {
		t.Fatal("wrong value", result)
	}
}

func mockEtherscanServer(t *testing.T) (ts *httptest.Server) {
	t.Helper()

	pages := []etherscanResponseBody{
		{
			Result: []types.RawSlurp{
				{
					BlockNumber:      "1",
					TransactionIndex: "1",
				},
				{
					BlockNumber:      "1",
					TransactionIndex: "2",
				},
				{
					BlockNumber:      "1",
					TransactionIndex: "3",
				},
			},
		},
		{
			Result: []types.RawSlurp{
				{
					BlockNumber:      "2",
					TransactionIndex: "1",
				},
			},
		},
	}
	ts = httptest.NewServer(http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {
		query := r.URL.Query()
		var result etherscanResponseBody
		switch query.Get("page") {
		case "1":
			result = pages[0]
		case "2":
			result = pages[1]
		default:
			result = etherscanResponseBody{}
		}

		b, err := json.Marshal(result)
		if err != nil {
			t.Fatal(err)
		}
		w.Write(b)
	}))

	return ts
}

func mockConvertSlurpType(t *testing.T) func(address string, requestType string, rawTx *types.RawSlurp) (types.SimpleSlurp, error) {
	t.Helper()
	return func(address string, requestType string, rawTx *types.RawSlurp) (types.SimpleSlurp, error) {
		bn, err := strconv.ParseUint(rawTx.BlockNumber, 10, 64)
		if err != nil {
			t.Fatal(err)
		}
		txid, err := strconv.ParseUint(rawTx.TransactionIndex, 10, 64)
		if err != nil {
			t.Fatal(err)
		}
		return types.SimpleSlurp{
			BlockNumber:      bn,
			TransactionIndex: txid,
		}, nil
	}
}

func TestEtherscanProvider_fetchData(t *testing.T) {
	perPage := 3
	ts := mockEtherscanServer(t)
	defer ts.Close()

	etherscanBaseUrl = ts.URL

	provider := EtherscanProvider{
		perPage: perPage,
		baseUrl: ts.URL,
	}
	provider.limiter = rate.NewLimiter(5, 5)
	provider.convertSlurpType = mockConvertSlurpType(t)
	paginator := NewPageNumberPaginator(1, 1, perPage)

	var data []SlurpedPageItem
	var count int
	var err error
	data, count, err = provider.fetchData(context.TODO(), base.HexToAddress("0xf503017d7baf7fbc0fff7492b751025c6a78179b"), paginator, "int")
	if err != nil {
		t.Fatal(err)
	}

	if l := len(data); l != perPage {
		t.Fatal("wrong len of page 1:", l)
	}
	if count != perPage {
		t.Fatal("wrong count", count)
	}
	if paginator.Done() {
		t.Fatal("paginator done but it should not be")
	}

	if err = paginator.NextPage(); err != nil {
		t.Fatal(err)
	}

	data, count, err = provider.fetchData(context.TODO(), base.HexToAddress("0xf503017d7baf7fbc0fff7492b751025c6a78179b"), paginator, "int")
	if err != nil {
		t.Fatal(err)
	}
	if l := len(data); l != 1 {
		t.Fatal("wrong len of page 2:", l)
	}
	if !paginator.Done() {
		t.Fatal("paginator should be done")
	}
}

func TestEtherscanProvider_TransactionsByAddress(t *testing.T) {
	perPage := 3
	ts := mockEtherscanServer(t)
	defer ts.Close()

	etherscanBaseUrl = ts.URL

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

func TestEtherscanProvider_Appearances(t *testing.T) {
	perPage := 3
	ts := mockEtherscanServer(t)
	defer ts.Close()

	etherscanBaseUrl = ts.URL

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

func TestEtherscanProvider_Count(t *testing.T) {
	perPage := 3
	ts := mockEtherscanServer(t)
	defer ts.Close()

	etherscanBaseUrl = ts.URL

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
