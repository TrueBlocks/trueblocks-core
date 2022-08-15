// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package manifest

import (
	"context"
	"errors"
	"fmt"
	"net/http"
	"net/url"
	"os"
	"path"
	"path/filepath"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/unchained"
	"github.com/ethereum/go-ethereum"
	"github.com/ethereum/go-ethereum/accounts/abi"
	"github.com/ethereum/go-ethereum/common"
)

// fromRemote gets the CID from the smart contract, calls
// the gateway and returns the parsed manifest
func fromRemote(chain string) (*Manifest, error) {
	cid, err := getManifestCidFromContract(chain)
	if err != nil {
		return nil, err
	}
	return downloadManifest(chain, config.GetIpfsGateway(chain), cid)
}

// getManifestCidFromContract calls UnchainedIndex smart contract to get the current manifest IPFS CID
func getManifestCidFromContract(chain string) (string, error) {
	// TODO: BOGUS - WHERE DOES THE UNCHAINED INDEX SMART CONTRACT LIVE?
	provider := config.GetRpcProvider("mainnet") // chain)
	rpcClient.CheckRpc(provider)
	ethClient := rpcClient.GetClient(provider)
	defer ethClient.Close()

	abiFn := filepath.Join(config.GetPathToRootConfig(), "abis/known-000/unchainedV2.json")
	address := common.HexToAddress(unchained.Address_V2)
	signature := unchained.ReadHashName_V2

	abiSource, err := os.Open(abiFn)
	if err != nil {
		return "", fmt.Errorf("while reading contract ABI: %w", err)
	}
	defer abiSource.Close()

	contractAbi, err := abi.JSON(abiSource)
	if err != nil {
		return "", fmt.Errorf("while parsing contract ABI: %w", err)
	}

	callData, err := contractAbi.Pack(signature, common.HexToAddress(unchained.PreferredPublisher), chain)
	if err != nil {
		return "", fmt.Errorf("while building calldata: %w", err)
	}

	msg := ethereum.CallMsg{
		To:   &address,
		Data: callData,
	}

	response, err := ethClient.CallContract(
		context.Background(),
		msg,
		nil,
	)
	if err != nil {
		return "", fmt.Errorf("while calling contract: %w", err)
	}

	if len(response) == 0 {
		msg := fmt.Sprintf("empty response %s from provider %s on chain %s", response, provider, chain)
		return "", fmt.Errorf(msg)
	}

	unpacked, err := contractAbi.Unpack(signature, response)
	if err != nil {
		return "", fmt.Errorf("while unpacking value: %w", err)
	}

	if len(unpacked) == 0 {
		return "", errors.New("contract returned empty data")
	}

	ret := unpacked[0].(string)
	if len(ret) == 0 {
		return "", errors.New("The Unchained Index returned empty CID for " + chain + ". Has the index for that chain been published?")
	}

	return ret, nil
}

// downloadManifest downloads manifest from the given gateway and parses it into
// Manifest struct. Both JSON and TSV formats are supported, but the server has
// to set the correct Content-Type header.
func downloadManifest(chain, gatewayUrl, cid string) (*Manifest, error) {
	logger.Log(logger.Info, "Chain:", chain)
	logger.Log(logger.Info, "Gateway:", gatewayUrl)
	logger.Log(logger.Info, "CID:", cid)

	url, err := url.Parse(gatewayUrl)
	if err != nil {
		return nil, err
	}
	url.Path = path.Join(url.Path, cid)

	response, err := http.Get(url.String())
	if err != nil {
		return nil, err
	}

	switch response.Header.Get("content-type") {
	case "text/tab-separated-values":
		return readTabManifest(chain, response.Body)
	case "application/json":
		return readJSONManifest(response.Body)
	default:
		return nil, errors.New("unrecognized content type")
	}
}
