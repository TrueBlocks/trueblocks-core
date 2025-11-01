// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package rpc

import (
	"context"
	"fmt"
	"strings"
	"sync"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/rpc/query"
	"github.com/ethereum/go-ethereum/ethclient"
)

// GetClientVersion returns the version of the client
func (conn *Connection) GetClientVersion() (version string, err error) {
	// TODO: C++ code used to cache version info
	method := "web3_clientVersion"
	params := query.Params{}

	if version, err := query.Query[string](conn.Chain, method, params); err != nil {
		return "", err
	} else {
		return *version, nil
	}
}

// GetClientIDs returns both chainId and networkId from the node
func (conn *Connection) GetClientIDs() (uint64, uint64, error) {
	if ec, err := conn.getClient(); err != nil {
		return 0, 0, err
	} else {
		defer ec.Close()

		ch, err := ec.ChainID(context.Background())
		if err != nil {
			return 0, 0, err
		}

		n, err := ec.NetworkID(context.Background())
		if err != nil {
			return ch.Uint64(), 0, err
		}

		return ch.Uint64(), n.Uint64(), nil
	}
}

// GetLatestBlockNumber returns the block number at the front of the chain (i.e. latest)
func (conn *Connection) GetLatestBlockNumber() base.Blknum {
	if ec, err := conn.getClient(); err != nil {
		logger.Error("Could not connect to RPC client", err)
		return 0
	} else {
		defer ec.Close()

		bn, err := ec.BlockNumber(context.Background())
		if err != nil {
			logger.Error("Could not connect to RPC client", err)
			return 0
		}
		return base.Blknum(bn)
	}
}

// TODO: this looks weird, but before we were creating and deleting the client with every call which
// TODO: overran the number of TPC connection the OS would create (on a Mac). Since then, we now
// TODO: open the connection once and just use it allowing the operating system to clean it up
var clientMutex sync.Mutex
var perProviderClientMap = map[string]*ethclient.Client{}

func (conn *Connection) getClient() (*ethclient.Client, error) {
	provider := config.GetChain(conn.Chain).GetRpcProvider()
	if provider == "" || provider == "https://" {
		var noProvider = `

  Warning: The RPC server ([{PROVIDER}]) was not available. Either start it, or edit the rpcProvider
  value in the file [{FILE}]. Quitting...
`
		msg := noProvider
		msg = strings.ReplaceAll(msg, "[{PROVIDER}]", provider)
		msg = strings.ReplaceAll(msg, "[{FILE}]", config.PathToConfigFile())
		msg = strings.ReplaceAll(msg, "https://", "<empty>")
		return nil, fmt.Errorf("%s", msg)
	}

	clientMutex.Lock()
	defer clientMutex.Unlock()

	if perProviderClientMap[provider] == nil {
		ec, err := ethclient.Dial(provider)
		if err != nil || ec == nil {
			logger.Panic("Missdial("+provider+"):", err)
		}
		perProviderClientMap[provider] = ec
	}
	return perProviderClientMap[provider], nil
}
