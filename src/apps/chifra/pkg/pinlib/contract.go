// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package pinlib

import (
	"context"
	"errors"
	"fmt"
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/ethereum/go-ethereum"
	"github.com/ethereum/go-ethereum/accounts/abi"
)

// GetUnchainedIndexAddress returns UnchainedIndex smart contract address
func GetUnchainedIndexAddress() string {
	return config.ReadBlockScrape().UnchainedIndex.Address
}

// GetManifestHashEncoding returns encoding of manifestHash getter of
// UnchainedIndex smart contract
func GetManifestHashEncoding() string {
	return config.ReadBlockScrape().UnchainedIndex.ManifestHashEncoding
}

// GetManifestCidFromContract calls UnchainedIndex smart contract to get
// the current manifest IPFS CID
func GetManifestCidFromContract() (string, error) {
	ethClient := rpcClient.Get()
	defer ethClient.Close()

	address := rpcClient.HexToAddress(GetUnchainedIndexAddress())
	data := rpcClient.DecodeHex(GetManifestHashEncoding())

	response, err := ethClient.CallContract(
		context.Background(),
		ethereum.CallMsg{
			To:   &address,
			Data: data,
		},
		nil,
	)
	if err != nil {
		return "", fmt.Errorf("while calling contract: %w", err)
	}

	abiSource, err := os.Open(
		config.GetPathToConfig("abis/known-000/unchained.json"),
	)
	if err != nil {
		return "", fmt.Errorf("while reading contract ABI: %w", err)
	}

	contractAbi, err := abi.JSON(abiSource)
	if err != nil {
		return "", fmt.Errorf("while parsing contract ABI: %w", err)
	}

	unpacked, err := contractAbi.Unpack("manifestHash", response)
	if err != nil {
		return "", fmt.Errorf("while unpacking value: %w", err)
	}

	if len(unpacked) == 0 {
		return "", errors.New("contract returned empty data")
	}

	return unpacked[0].(string), nil
}
