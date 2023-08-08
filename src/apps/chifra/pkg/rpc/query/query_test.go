// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package query

import (
	"encoding/json"
	"net/http"
	"net/http/httptest"
	"testing"
)

func TestFromRpcCounter(t *testing.T) {
	ids := []int{}
	server := httptest.NewServer(http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {
		var payload struct {
			ID int
		}
		err := json.NewDecoder(r.Body).Decode(&payload)
		if err != nil {
			t.Fatal(err)
		}
		ids = append(ids, payload.ID)
		_, _ = w.Write([]byte("{}"))
	}))
	defer server.Close()

	var result map[string]string
	for i := 0; i < 20; i++ {
		// TODO: Use rpc.Query
		err := FromRpc(
			server.URL,
			&Payload{},
			&result,
		)
		if err != nil {
			t.Fatal(err)
		}
	}

	seen := map[int]bool{}
	for index, id := range ids {
		if index != len(ids)-1 && ids[index+1]-id != 1 {
			t.Fatal("ids should be in order", id, ids[index+1])
		}
		if id == 0 {
			t.Fatal("0 not allowed")
		}
		alreadySeen, ok := seen[id]
		if ok && alreadySeen {
			t.Fatal("duplicated id", id)
		}
	}
}
