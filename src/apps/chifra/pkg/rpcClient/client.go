// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package rpcClient

import (
	"context"
	"log"
	"math/big"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/ethereum/go-ethereum/common"
	"github.com/ethereum/go-ethereum/common/hexutil"
	"github.com/ethereum/go-ethereum/ethclient"
)

// Get sets up a client instance and returns it
func Get() ethclient.Client {
	// TODO: I don't like the fact that we Dail In every time we want to us this
	client, err := ethclient.Dial(config.GetRpcProvider())
	if err != nil {
		log.Fatalln(err)
	}

	return *client
}

// HexToAddress converts a string with hex to go-ethereum's common.Address
func HexToAddress(hex string) common.Address {
	return common.HexToAddress(hex)
}

// DecodeHex decodes a string with hex into a slice of bytes
func DecodeHex(hex string) []byte {
	return hexutil.MustDecode(hex)
}

func GetBlockTimestamp(bn uint64) uint64 {
	client := Get()
	block, err := client.BlockByNumber(context.Background(), big.NewInt(int64(bn)))
	if err != nil {
		return 0
	}
	return block.Time()
}
