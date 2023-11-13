// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package manifest

import (
	"encoding/json"
	"fmt"
	"net/http"
	"net/url"
	"os"
	"path/filepath"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/articulate"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/call"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// ReadUnchainedIndex calls UnchainedIndex smart contract to get the current manifest IPFS CID as
// published by the given publisher
func ReadUnchainedIndex(chain string, publisher base.Address, database string) (string, error) {
	cid := os.Getenv("TB_OVERRIDE_CID")
	if cid != "" {
		return cid, nil
	}

	unchainedChain := "mainnet" // the unchained index is on mainnet
	theCall := fmt.Sprintf("0x7087e4bd(%s, \"%s\")", publisher, database)  // manifestHashMap
	conn := rpc.TempConnection(unchainedChain)

	if contractCall, _, err := call.NewContractCall(conn, base.HexToAddress(config.GetUnchained().SmartContract), theCall); err != nil {
		wrapped := fmt.Errorf("the --call value provided (%s) was not found: %s", theCall, err)
		return "", wrapped
	} else {
		contractCall.BlockNumber = conn.GetLatestBlockNumber()
		abiCache := articulate.NewAbiCache(chain, true)
		artFunc := func(str string, function *types.SimpleFunction) error {
			return abiCache.ArticulateFunction(function, "", str[2:])
		}
		if result, err := contractCall.Call(artFunc); err != nil {
			return "", err
		} else {
			return result.Values["val_0"], nil
		}
	}
}

// downloadManifest downloads manifest from the given gateway and parses it into
// Manifest struct. Both JSON and TSV formats are supported, but the server has
// to set the correct Content-Type header.
func downloadManifest(chain, gatewayUrl, cid string) (*Manifest, error) {
	url, err := url.Parse(gatewayUrl)
	if err != nil {
		return nil, err
	}
	url.Path = filepath.Join(url.Path, cid)

	resp, err := http.Get(url.String())
	if err != nil {
		return nil, err
	}

	if resp.StatusCode != http.StatusOK {
		return nil, fmt.Errorf("fetch to pinning service (%s) failed: %s", url.String(), resp.Status)
	}

	switch resp.Header.Get("Content-Type") {
	case "application/json":
		m := &Manifest{}
		err := json.NewDecoder(resp.Body).Decode(m)
		return m, err
	default:
		return nil, fmt.Errorf("fetch to %s return unrecognized content type: %s", url.String(), resp.Header.Get("Content-Type"))
	}
}
