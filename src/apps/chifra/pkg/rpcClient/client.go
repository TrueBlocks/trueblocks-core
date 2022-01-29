// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package rpcClient

import (
	"context"
	"log"
	"math/big"

	"github.com/ethereum/go-ethereum/common"
	"github.com/ethereum/go-ethereum/common/hexutil"
	"github.com/ethereum/go-ethereum/ethclient"
)

// Get sets up a client instance and returns it
func GetClient(provider string) ethclient.Client {
	// TODO: I don't like the fact that we Dail In every time we want to us this
	// TODO: If we make this a cached item, it needs to be cached per chain, see timestamps
	ec, err := ethclient.Dial(provider)
	if err != nil {
		log.Fatalln(err)
	}
	return *ec
}

// BlockNumber returns the front of chain block
func BlockNumber(provider string) uint64 {
	ec := GetClient(provider)
	r, _ := ec.BlockNumber(context.Background())
	ec.Close()
	return r
}

// ChainID returns chainId
func ChainID(provider string) uint64 {
	ec := GetClient(provider)
	r, _ := ec.ChainID(context.Background())
	ec.Close()
	return r.Uint64()
}

// NetworkID returns networkId
func NetworkID(provider string) uint64 {
	ec := GetClient(provider)
	r, _ := ec.NetworkID(context.Background())
	ec.Close()
	return r.Uint64()
}

func GetBlockTimestamp(provider string, bn uint64) uint64 {
	ec := GetClient(provider)
	r, _ := ec.BlockByNumber(context.Background(), big.NewInt(int64(bn)))
	ec.Close()
	return r.Time()
}

// HexToAddress converts a string with hex to go-ethereum's common.Address
func HexToAddress(hex string) common.Address {
	return common.HexToAddress(hex)
}

// DecodeHex decodes a string with hex into a slice of bytes
func DecodeHex(hex string) []byte {
	return hexutil.MustDecode(hex)
}
