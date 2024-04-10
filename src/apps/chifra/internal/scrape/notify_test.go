package scrapePkg

import (
	"encoding/json"
	"io"
	"net/http"
	"net/http/httptest"
	"sync"
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/notify"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/uniq"
)

// TODO: Remove rpc.MetaData and use types.MetaData throughout

func TestNotify(t *testing.T) {
	results := make([]string, 0)
	var mutex sync.Mutex
	ts := httptest.NewServer(http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {
		body, err := io.ReadAll(r.Body)
		if err != nil {
			t.Fatal(err)
		}
		defer r.Body.Close()

		mutex.Lock()
		results = append(results, string(body))
		mutex.Unlock()
	}))
	defer ts.Close()

	newAppNotification := notify.Notification[[]notify.NotificationPayloadAppearance]{
		Msg:  notify.MessageAppearance,
		Meta: &types.MetaData{Chain: "ethereum"},
		Payload: []notify.NotificationPayloadAppearance{
			{
				Address:          "0xfffd8963efd1fc6a506488495d951d5263988d25",
				BlockNumber:      "18509161",
				TransactionIndex: 132,
			},
		},
	}

	if err := notifyEndpoint(ts.URL, newAppNotification); err != nil {
		t.Fatal(err)
	}

	result := results[0]
	expected, err := json.Marshal(newAppNotification)
	if err != nil {
		t.Fatal(err)
	}

	if result != string(expected) {
		t.Fatalf("wrong result: %+v", result)
	}
}

func TestNotificationDataAppearance_FromString(t *testing.T) {
	addrMap := make(uniq.AddressBooleanMap, 0)
	key := addrMap.Insert(
		"0xfffd8963efd1fc6a506488495d951d5263988d25",
		18509161,
		132,
	)

	n := &notify.NotificationPayloadAppearance{}
	if err := n.FromString(key); err != nil {
		t.Fatal(err)
	}

	if addr := n.Address; addr != "0xfffd8963efd1fc6a506488495d951d5263988d25" {
		t.Fatal("address", addr)
	}
	if bn := n.BlockNumber; bn != "18509161" {
		t.Fatal("wrong block number", bn)
	}
	if txid := n.TransactionIndex; txid != 132 {
		t.Fatal("wrong transaction id", txid)
	}
}
