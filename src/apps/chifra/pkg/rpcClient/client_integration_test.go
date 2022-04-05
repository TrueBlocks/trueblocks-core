//go:build integration

// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package rpcClient

import (
	"bytes"
	"context"
	"math/big"
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/ethereum/go-ethereum/common"
)

func Test_Client(t *testing.T) {
	a := DecodeHex("0xAb")
	b := []byte{0xAb}
	c := bytes.Compare(a, b)
	if c != 0 {
		t.Error("incorrect result from DecodeHex")
	}

	addr := common.HexToAddress("0x00000000000000000000000000000000deadbeef")
	expected := common.BigToAddress(big.NewInt(0x00000000000000000000000000000000DEADBeeF))
	if addr != expected {
		t.Error("incorrect result from HexToAddress")
	}

	mainnet := config.GetRpcProvider("mainnet")
	ec := GetClient(mainnet) // won't return if it doesn't connect

	bn, _ := ec.BlockNumber(context.Background())
	if bn < 10000000 {
		t.Error("probably not the right chain")
	}

	chainId, networkId := GetIDs(mainnet)
	if chainId != networkId || chainId != 1 {
		t.Error("mainnet chain id is 1")
	}

	ts := GetBlockTimestamp(mainnet, 1)
	if ts != 1438269988 {
		t.Error("timestamp for block 1 is not correct")
	}

	_, err := TxHashFromHash(mainnet, "0x730724cb08a6eb17bf6b3296359d261570d343ea7944a17a9d7287d77900db08")
	if err != nil {
		t.Error("couldn't get known transaction hash from tx hash")
	}

	_, err = TxHashFromHashAndId(mainnet, "0x0b4c6fb75ded4b90218cf0346b0885e442878f104e1b60bf75d5b6860eeacd53", 0)
	if err != nil {
		t.Error("couldn't get known transaction hash from block hash and tx id")
	}

	_, err = TxHashFromNumberAndId(mainnet, 1001001, 0)
	if err != nil {
		t.Error("couldn't get known transaction hash from block number and tx id")
	}

	_, err = BlockHashFromHash(mainnet, "0x0b4c6fb75ded4b90218cf0346b0885e442878f104e1b60bf75d5b6860eeacd53")
	if err != nil {
		t.Error("couldn't get known block hash from block hash")
	}

	_, err = BlockHashFromNumber(mainnet, 12)
	if err != nil {
		t.Error("couldn't get known block hash from block number")
	}
}
