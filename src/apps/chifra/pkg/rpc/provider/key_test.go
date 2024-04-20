package provider

import (
	"context"
	"encoding/json"
	"io"
	"net/http"
	"net/http/httptest"
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"golang.org/x/time/rate"
)

func mockKeyServer(t *testing.T) (ts *httptest.Server) {
	t.Helper()

	pages := []keyResponseBody{
		{
			Data: []KeyAppearance{
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
			Meta: Meta{
				PreviousPageId: "second",
			},
		},
		{
			Data: []KeyAppearance{
				{
					BlockNumber:      "2",
					TransactionIndex: "1",
				},
			},
		},
	}
	ts = httptest.NewServer(http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {
		var request struct {
			Params []keyAppearancesRequestParam `json:"params"`
		}
		var result struct {
			Result keyResponseBody `json:"result"`
		}

		body, err := io.ReadAll(r.Body)
		if err != nil {
			t.Fatal(err)
		}
		defer r.Body.Close()
		if err := json.Unmarshal(body, &request); err != nil {
			t.Fatal(err)
		}

		switch request.Params[0].PageId {
		case "":
			fallthrough
		case "latest":
			result.Result = pages[0]
			result.Result.Meta.PreviousPageId = "second"
		case "second":
			result.Result = pages[1]
			result.Result.Meta.PreviousPageId = ""
		default:
			result.Result = keyResponseBody{}
			result.Result.Meta.PreviousPageId = ""
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

func TestKeyProvider_Appearances(t *testing.T) {
	perPage := 3
	ts := mockKeyServer(t)
	defer ts.Close()

	provider := KeyProvider{
		perPage: perPage,
		baseUrl: ts.URL,
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

	if count != 4 {
		t.Fatal("wrong count:", count)
	}
}

func TestKeyProvider_Count(t *testing.T) {
	perPage := 3
	ts := mockKeyServer(t)
	defer ts.Close()

	provider := KeyProvider{
		perPage: perPage,
		baseUrl: ts.URL,
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
	if n := count[0].NRecords; n != 4 {
		t.Fatal("wrong NRecords", n)
	}
}
