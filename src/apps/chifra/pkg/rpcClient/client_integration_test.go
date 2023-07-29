//go:build integration
// +build integration

// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package rpcClient

import (
	"context"
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

func Test_Client(t *testing.T) {
	chain := utils.GetTestChain()
	provider := config.GetRpcProvider(chain)
	ec := GetClient(provider) // won't return if it doesn't connect

	bn, _ := ec.BlockNumber(context.Background())
	if bn < 10000000 {
		t.Error("probably not the right chain")
	}

	chainId, networkId, _ := GetIDs(chain)
	if chainId != networkId || chainId != 1 {
		t.Error("provider chain id is 1")
	}

	ts := rpc.GetBlockTimestamp(chain, utils.PointerOf(uint64(1)))
	blockOneTimestamp := int64(1438269988)
	if ts != blockOneTimestamp {
		t.Error("timestamp for block 1 is not correct")
	}

	_, err := GetTxHashFromHash(chain, "0x730724cb08a6eb17bf6b3296359d261570d343ea7944a17a9d7287d77900db08")
	if err != nil {
		t.Error("couldn't get known transaction hash from tx hash")
	}

	_, err = GetTxHashFromHashAndId(chain, "0x0b4c6fb75ded4b90218cf0346b0885e442878f104e1b60bf75d5b6860eeacd53", 0)
	if err != nil {
		t.Error("couldn't get known transaction hash from block hash and tx id")
	}

	_, err = GetBlockHashFromHash(chain, "0x0b4c6fb75ded4b90218cf0346b0885e442878f104e1b60bf75d5b6860eeacd53")
	if err != nil {
		t.Error("couldn't get known block hash from block hash")
	}

	_, err = GetBlockHashFromNumber(chain, 12)
	if err != nil {
		t.Error("couldn't get known block hash from block number")
	}
}

func Test_TxFromNumberAndId(t *testing.T) {
	txId := uint64(0)
	_, err := GetTxFromNumberAndId("mainnet", uint64(1424623), txId)
	if err != nil {
		t.Fatal(err)
	}
}
