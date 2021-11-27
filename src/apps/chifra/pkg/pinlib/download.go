// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package pinlib

import (
	"errors"
	"net/http"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/pinlib/manifest"
)

// DownloadManifest downloads manifest from the given gateway and parses it into
// Manifest struct. Both JSON and TSV formats are supported, but the server has
// to set the correct Content-Type header.
func DownloadManifest(gatewayUrl string) (*manifest.Manifest, error) {
	response, err := http.Get(gatewayUrl)
	if err != nil {
		return nil, err
	}

	switch response.Header.Get("content-type") {
	case "text/tab-separated-values":
		return manifest.ReadTabManifest(response.Body)
	case "application/json":
		return manifest.ReadJSONManifest(response.Body)
	default:
		return nil, errors.New("unrecognized content type")
	}
}
