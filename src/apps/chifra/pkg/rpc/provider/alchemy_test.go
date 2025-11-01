package provider

import (
	"context"
	"encoding/json"
	"io"
	"net/http"
	"net/http/httptest"
	"reflect"
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/types"
	"golang.org/x/time/rate"
)

func Test_alchemyPrepareQuery(t *testing.T) {
	q := &Query{
		Resources: []string{"int", "ext"},
	}
	result, err := alchemyPrepareQuery(q)
	if err != nil {
		t.Fatal(err)
	}
	expected := []string{
		"internal:to",
		"internal:from",
		"external:to",
		"external:from",
	}
	if !reflect.DeepEqual(result.Resources, expected) {
		t.Fatal("wrong query", result)
	}
}

func mockAlchemyServer(t *testing.T) (ts *httptest.Server) {
	t.Helper()

	pages := []alchemyResponseBody{
		{
			Transfers: []AlchemyTx{
				{
					BlockNumber: "1",
					Hash:        "1",
				},
				{
					BlockNumber: "1",
					Hash:        "2",
				},
				{
					BlockNumber: "1",
					Hash:        "3",
				},
			},
			PageKey: "second",
		},
		{
			Transfers: []AlchemyTx{
				{
					BlockNumber: "2",
					Hash:        "1",
				},
			},
		},
	}
	ts = httptest.NewServer(http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {
		var request struct {
			Params []alchemyRequestParam `json:"params"`
		}
		var result struct {
			Result alchemyResponseBody `json:"result"`
		}

		body, err := io.ReadAll(r.Body)
		if err != nil {
			t.Fatal(err)
		}
		defer r.Body.Close()
		if err := json.Unmarshal(body, &request); err != nil {
			t.Fatal(err)
		}

		switch request.Params[0].PageKey {
		case "":
			fallthrough
		case "first":
			result.Result = pages[0]
			result.Result.PageKey = "second"
		case "second":
			result.Result = pages[1]
			result.Result.PageKey = ""
		default:
			result.Result = alchemyResponseBody{}
			result.Result.PageKey = ""
		}

		w.Header().Set("Content-Type", "application/json")

		b, err := json.Marshal(result)
		if err != nil {
			t.Fatal(err)
		}
		w.Write(b)
	}))

	return ts
}

func TestAlchemyProvider_TransactionsByAddress(t *testing.T) {
	perPage := 3
	ts := mockAlchemyServer(t)
	defer ts.Close()

	provider := AlchemyProvider{
		perPage: perPage,
		baseUrl: ts.URL,
		getTransactionAppearance: func(hash string) (types.Appearance, error) {
			return types.Appearance{}, nil
		},
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

	if count != 8 {
		t.Fatal("wrong count:", count)
	}
}

func TestAlchemyProvider_Appearances(t *testing.T) {
	perPage := 3
	ts := mockAlchemyServer(t)
	defer ts.Close()

	provider := AlchemyProvider{
		perPage: perPage,
		baseUrl: ts.URL,
		getTransactionAppearance: func(hash string) (types.Appearance, error) {
			return types.Appearance{}, nil
		},
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

	if count != 8 {
		t.Fatal("wrong count:", count)
	}
}

func TestAlchemyProvider_Count(t *testing.T) {
	perPage := 3
	ts := mockAlchemyServer(t)
	defer ts.Close()

	provider := AlchemyProvider{
		perPage: perPage,
		baseUrl: ts.URL,
		getTransactionAppearance: func(hash string) (types.Appearance, error) {
			return types.Appearance{}, nil
		},
	}

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

	count := make([]types.Monitor, 0, 1)
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
	if n := count[0].NRecords; n != 8 {
		t.Fatal("wrong NRecords", n)
	}
}
