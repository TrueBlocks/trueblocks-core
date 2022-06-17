// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package pinlib

import (
	"context"
	"errors"
	"fmt"
	"net/url"
	"os"
	"path"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/pinlib/manifest"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/unchained"
	"github.com/ethereum/go-ethereum"
	"github.com/ethereum/go-ethereum/accounts/abi"
	"github.com/ethereum/go-ethereum/common"
)

// GetManifestCidFromContract calls UnchainedIndex smart contract to get
// the current manifest IPFS CID
func GetManifestCidFromContract(chain string) (string, error) {
	// TODO: BOGUS
	newVersion := os.Getenv("NEW_UNCHAINED") == "true"

	provider := config.GetRpcProvider(chain)
	rpcClient.CheckRpc(provider)
	ethClient := rpcClient.GetClient(provider)
	defer ethClient.Close()

	abiFn := config.GetPathToRootConfig() + "abis/known-000/unchained.json"
	address := common.HexToAddress(unchained.Address)
	callData := rpcClient.DecodeHex(unchained.ReadHash)
	signature := unchained.ReadHashName
	if newVersion {
		abiFn = config.GetPathToRootConfig() + "abis/known-000/unchainedV2.json"
		address = common.HexToAddress(unchained.Address_V2)
		callData = rpcClient.DecodeHex(unchained.ReadHash_V2)
		signature = unchained.ReadHashName_V2
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

	abiSource, err := os.Open(abiFn)
	if err != nil {
		return "", fmt.Errorf("while reading contract ABI: %w", err)
	}
	defer abiSource.Close()

	contractAbi, err := abi.JSON(abiSource)
	if err != nil {
		return "", fmt.Errorf("while parsing contract ABI: %w", err)
	}

	unpacked, err := contractAbi.Unpack(signature, response)
	if err != nil {
		return "", fmt.Errorf("while unpacking value: %w", err)
	}

	if len(unpacked) == 0 {
		return "", errors.New("contract returned empty data")
	}

	return unpacked[0].(string), nil
}

func DownloadRemoteManifest(chain string) (*manifest.Manifest, error) {
	// Fetch manifest's CID
	cid, err := GetManifestCidFromContract(chain)
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

	downloadedManifest, err := DownloadManifest(chain, url.String())
	if err != nil {
		return nil, err
	}

	return downloadedManifest, nil
}
