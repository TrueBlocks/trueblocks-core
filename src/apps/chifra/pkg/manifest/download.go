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
	"path"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/abi"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/articulate"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/call"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/debug"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	ethAbi "github.com/ethereum/go-ethereum/accounts/abi"
)

// ReadUnchainedIndex calls UnchainedIndex smart contract to get the current manifest IPFS CID as
// published by the given publisher
func ReadUnchainedIndex(chain string, publisher base.Address, database string) (string, error) {
	_ = chain
	cid := os.Getenv("TB_OVERRIDE_CID")
	if cid != "" {
		return cid, nil
	}

	callAddress, abiMap, err := getUnchainedAbi()
	if err != nil {
		return "", err
	}

	unchainedChain := "mainnet" // the unchained index is on mainnet
	conn := rpc.TempConnection(unchainedChain)
	// if conn.LatestBlockTimestamp < 1_705_173_443 { // block 19_000_000
	// 	provider := config.GetChain(unchainedChain).RpcProvider
	// 	logger.Fatal(usage.Usage(unchainedWarning, provider))
	// }

	theCall := fmt.Sprintf("manifestHashMap(%s, \"%s\")", publisher, database)
	if contractCall, _, err := call.NewContractCallWithAbi(conn, callAddress, theCall, abiMap); err != nil {
		wrapped := fmt.Errorf("the --calldata value provided (%s) was not found: %s", theCall, err)
		return "", wrapped
	} else {
		contractCall.BlockNumber = conn.GetLatestBlockNumber()
		artFunc := func(str string, function *types.Function) error {
			return articulate.ArticulateFunction(function, "", str[2:])
		}
		if result, err := contractCall.Call(artFunc); err != nil {
			return "", err
		} else {
			return result.Values["hash"], nil
		}
	}
}

// downloadManifest downloads manifest from the given gateway and parses it into
// Manifest struct. Both JSON and TSV formats are supported, but the server has
// to set the correct Content-Type header.
func downloadManifest(chain, gatewayUrl, cid string) (*Manifest, error) {
	_ = chain // linter
	url, err := url.Parse(gatewayUrl)
	if err != nil {
		return nil, err
	}
	url.Path = path.Join(url.Path, cid)

	debug.DebugCurlStr(url.String())
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

func getUnchainedAbi() (base.Address, *abi.SelectorSyncMap, error) {
	abiMap := &abi.SelectorSyncMap{}
	callAddress := base.HexToAddress(config.GetUnchained().SmartContract)

	var unchainedAbiJson = `[
  {
    "name": "manifestHashMap",
    "type": "function",
    "signature": "manifestHashMap(address,string)",
    "encoding": "0x7087e4bd",
    "inputs": [
      {
        "type": "address",
        "name": "publisher",
        "internalType": "address"
      },
      {
        "type": "string",
        "name": "database",
        "internalType": "string"
      }
    ],
    "outputs": [
      {
        "type": "string",
        "name": "hash",
        "internalType": "string"
      }
    ]
  }
]`

	if abi, err := ethAbi.JSON(strings.NewReader(unchainedAbiJson)); err != nil {
		return base.Address{}, abiMap, err
	} else {
		method := abi.Methods["manifestHashMap"]
		function := types.FunctionFromAbiMethod(&method)
		abiMap.SetValue(function.Encoding, function)
	}

	return callAddress, abiMap, nil
}

// var unchainedWarning string = `
// The Unchained Index requires your mainnet RPC to be synced (at least to block 0x1304073 or 19000000).
// Check the progress with the following curl command and try again later.

// curl -X POST -H "Content-Type: application/json" --data '{"method":"eth_blockNumber","params":[],"id":1,"jsonrpc":"2.0"}' {0}
// `
