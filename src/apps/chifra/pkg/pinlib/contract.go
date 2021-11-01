/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
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
	return config.ReadBlockScrape().UnchainedIndex.Encoding
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
		config.GetConfigPath("abis/known-000/unchained.json"),
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
