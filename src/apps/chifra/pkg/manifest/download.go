// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package manifest

import (
	"encoding/json"
	"errors"
	"fmt"
	"net/http"
	"net/url"
	"os"
	"path/filepath"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/articulate"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/call"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/unchained"
)

// fromRemote gets the CID from the smart contract, calls
// the gateway and returns the parsed manifest
func fromRemote(chain string, publisher base.Address) (*Manifest, error) {
	database := chain
	cid, err := ReadUnchainedIndex(chain, publisher, database)
	if err != nil {
		return nil, err
	}

	gatewayUrl := config.GetIpfsGateway(chain)

	logger.InfoTable("Chain:", chain)
	logger.InfoTable("Gateway:", gatewayUrl)
	logger.InfoTable("Publisher:", publisher)
	logger.InfoTable("Database:", database)
	logger.InfoTable("CID:", cid)

	return downloadManifest(chain, gatewayUrl, cid)
}

// ReadUnchainedIndex calls UnchainedIndex smart contract to get the current manifest IPFS CID as
// published by the given publisher
func ReadUnchainedIndex(chain string, publisher base.Address, database string) (string, error) {
	cid := os.Getenv("TB_OVERRIDE_CID")
	if cid != "" {
		return cid, nil
	}

	unchainedChain := "mainnet" // the unchained index is on mainnet
	theCall := fmt.Sprintf("manifestHashMap(%s, \"%s\")", publisher, database)
	conn := rpc.TempConnection(unchainedChain)

	if contractCall, _, err := call.NewContractCall(conn, unchained.GetUnchainedIndexAddress(), theCall); err != nil {
		return "", err
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

	response, err := http.Get(url.String())
	if err != nil {
		return nil, err
	}

	switch response.Header.Get("content-type") {
	case "application/json":
		m := &Manifest{}
		err := json.NewDecoder(response.Body).Decode(m)
		return m, err
	default:
		return nil, errors.New("unrecognized content type: " + response.Header.Get("content-type"))
	}
}
