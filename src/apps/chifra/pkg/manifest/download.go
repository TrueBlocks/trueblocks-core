// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package manifest

import (
	"errors"
	"net/http"
	"net/url"
	"path"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

func DownloadRemoteManifest(chain string) (*Manifest, error) {
	cid, err := getManifestCidFromContract(chain)
	if err != nil {
		return nil, err
	}
	logger.Log(logger.Info, "Unchained index returned CID", cid)

	// Download the manifest
	gatewayUrl := config.GetPinGateway(chain)
	logger.Log(logger.Info, "IPFS gateway", gatewayUrl)

	url, err := url.Parse(gatewayUrl)
	if err != nil {
		return nil, err
	}
	url.Path = path.Join(url.Path, cid)

	return downloadManifest(chain, url.String())
}

// downloadManifest downloads manifest from the given gateway and parses it into
// Manifest struct. Both JSON and TSV formats are supported, but the server has
// to set the correct Content-Type header.
func downloadManifest(chain, gatewayUrl string) (*Manifest, error) {
	response, err := http.Get(gatewayUrl)
	if err != nil {
		return nil, err
	}

	switch response.Header.Get("content-type") {
	case "text/tab-separated-values":
		return ReadTabManifest(chain, response.Body)
	case "application/json":
		return ReadJSONManifest(response.Body)
	default:
		return nil, errors.New("unrecognized content type")
	}
}
