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

// TODO: this looks weird, but before we were creating and deleting the client with every call which
// TODO: overran the number of TPC connection the OS would create (on a Mac). Since then, we now
// TODO: open the connection once and just use it allowing the operating system to clean it up
var theClient *ethclient.Client

func GetClient(provider string) *ethclient.Client {
	if theClient == nil {
		// TODO: I don't like the fact that we Dail In every time we want to us this
		// TODO: If we make this a cached item, it needs to be cached per chain, see timestamps
		ec, err := ethclient.Dial(provider)
		if err != nil {
			log.Fatalln(err)
		}
		theClient = ec
	}
	return theClient
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
