// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package manifest

import (
	"context"
	"errors"
	"fmt"
	"net/url"
	"os"
	"path"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/unchained"
	"github.com/ethereum/go-ethereum"
	"github.com/ethereum/go-ethereum/accounts/abi"
	"github.com/ethereum/go-ethereum/common"
)

// getManifestCidFromContract calls UnchainedIndex smart contract to get
// the current manifest IPFS CID
func getManifestCidFromContract(chain string) (string, error) {
	// TODO: BOGUS
	newVersion := unchained.NewUnchained(chain)

	// TODO: BOGUS -- where is the ultimate source of truth?
	provider := config.GetRpcProvider("mainnet") // chain)
	rpcClient.CheckRpc(provider)
	ethClient := rpcClient.GetClient(provider)
	defer ethClient.Close()

	abiFn := config.GetPathToRootConfig() + "abis/known-000/unchained.json"
	address := common.HexToAddress(unchained.Address)
	signature := unchained.ReadHashName
	if newVersion {
		abiFn = config.GetPathToRootConfig() + "abis/known-000/unchainedV2.json"
		address = common.HexToAddress(unchained.Address_V2)
		signature = unchained.ReadHashName_V2
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

	callData, err := contractAbi.Pack(signature)
	if newVersion {
		callData, err = contractAbi.Pack(signature, common.HexToAddress(unchained.PreferredPublisher), chain)
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
		return "", errors.New("unchained index returned empty CID for the " + chain + " chain.")
	}

	return ret, nil
}
