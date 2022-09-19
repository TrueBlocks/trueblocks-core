// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package manifest

import (
	"fmt"
	"net/http"
	"net/http/httptest"
	"testing"
)

var manifestJSONSource = `
{
  "version": "2",
  "chain": "mainnet",
  "schemas": "QmUou7zX2g2tY58LP1A2GyP5RF9nbJsoxKTp299ah3svgb",
  "chunks": [
    {
      "fileName": "000000000-000000000",
      "bloomHash": "QmPQEgUm7nzQuW9HYyWp5Ff3aoUwg2rsxDngyuyddJTvrv",
      "indexHash": "QmZ5Atm8Z7aFLz2EycK4pVMHuH4y3PDGspuFejnE9fx2i5"
    },
    {
      "fileName": "000000001-000350277",
      "bloomHash": "QmZgrWAJLidkHJRLDVoZGCWAgmmcQEDCDM65XL5ZbAXxCM",
      "indexHash": "QmP1KvDPUJ1MqsCYcicJgTf5sxN7WjT7dZsrfBk2Jg3mSe"
    }
   ]
}
`

func TestDownloadJSON(t *testing.T) {
	ts := httptest.NewServer(http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {
		w.Header().Set("Content-Type", "application/json")
		fmt.Fprintln(w, manifestJSONSource)
	}))

	defer ts.Close()

	manifest, err := downloadManifest("mainnet", ts.URL, "")
	if err != nil {
		t.Error(err)
	}

	if l := len(manifest.Chunks); l != 2 {
		t.Errorf("Wrong NewPins length: %d", l)
	}
}
