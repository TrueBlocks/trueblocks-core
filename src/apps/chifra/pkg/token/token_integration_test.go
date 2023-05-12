//go:build integration
// +build integration

// Copyright 2023 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package token

import (
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

// DAI
var tokenAddress = base.HexToAddress("0x6b175474e89094c44da98b954eedeac495271d0f")

// BoredApeYachtClub
var nftAddress = base.HexToAddress("0xBC4CA0EdA7647A8aB7C2061c2E118A18a936f13D")

// These use bytes32 instead of strings
var nonStandard1 = base.HexToAddress("0xf53ad2c6851052a81b42133467480961b2321c09")
var nonStandard2 = base.HexToAddress("0x461733c17b0755ca5649b6db08b3e213fcf22546")

func TestGetState_Erc20(t *testing.T) {
	blockNumber := "0xd59f80" // 14000000
	chain := utils.GetTestChain()

	token, err := GetState(chain, tokenAddress, blockNumber)
	if err != nil {
		t.Fatal(err)
	}

	if !token.IsErc20() || token.Type != TokenErc20 {
		t.Fatal("token reported as non-ERC20")
	}

	if token.IsErc721() || token.Type == TokenErc721 {
		t.Fatal("token reported as ERC721")
	}

	if token.Name != "Dai Stablecoin" {
		t.Fatal("wrong name:", token.Name)
	}

	if token.Symbol != "DAI" {
		t.Fatal("wrong symbol:", token.Symbol)
	}

	if token.Decimals != 18 {
		t.Fatal("wrong decimals:", token.Decimals)
	}

	if token.TotalSupply != "9118918230822796234900723527" {
		t.Fatal("wrong total supply:", token.TotalSupply)
	}
}

func TestGetState_Erc721(t *testing.T) {
	blockNumber := "0xd59f80" // 14000000
	chain := utils.GetTestChain()

	token, err := GetState(chain, nftAddress, blockNumber)
	if err != nil {
		t.Fatal(err)
	}

	if !token.IsErc721() || token.Type != TokenErc721 {
		t.Fatal("token reported as non-ERC721")
	}

	if token.IsErc20() || token.Type == TokenErc20 {
		t.Fatal("token reported as ERC20")
	}

	if token.Name != "BoredApeYachtClub" {
		t.Fatal("wrong name:", token.Name)
	}

	if token.Symbol != "BAYC" {
		t.Fatal("wrong symbol:", token.Symbol)
	}

	if token.Decimals > 0 {
		t.Fatal("NFT should not have decimals set:", token.Decimals)
	}

	if token.TotalSupply != "10000" {
		t.Fatal("wrong total supply:", token.TotalSupply)
	}
}

func TestGetState_NonStandard(t *testing.T) {
	blockNumber := "latest"
	chain := utils.GetTestChain()

	token, err := GetState(chain, nonStandard1, blockNumber)
	if err != nil {
		t.Fatal(err)
	}

	if !token.IsErc20() || token.Type != TokenErc20 {
		t.Fatal("token reported as non-ERC20")
	}

	if token.IsErc721() || token.Type == TokenErc721 {
		t.Fatal("token reported as ERC721")
	}

	if token.Name != "Pooled Ether" {
		t.Fatal("wrong name:", token.Name)
	}

	if token.Symbol != "PETH" {
		t.Fatal("wrong symbol:", token.Symbol)
	}

	if token.Decimals != 18 {
		t.Fatal("wrong decimals:", token.Decimals)
	}

	if token.TotalSupply != "7069797008171168928213" {
		t.Fatal("wrong total supply:", token.TotalSupply)
	}

	// Non-standard 2

	token, err = GetState(chain, nonStandard2, blockNumber)
	if err != nil {
		t.Fatal(err)
	}

	if !token.IsErc20() || token.Type != TokenErc20 {
		t.Fatal("token reported as non-ERC20")
	}

	if token.IsErc721() || token.Type == TokenErc721 {
		t.Fatal("token reported as ERC721")
	}

	if token.Name != "ATN" {
		t.Fatal("wrong name:", token.Name)
	}

	if token.Symbol != "ATN" {
		t.Fatal("wrong symbol:", token.Symbol)
	}

	if token.Decimals != 18 {
		t.Fatal("wrong decimals:", token.Decimals)
	}

	if token.TotalSupply != "210000000000000000000000000" {
		t.Fatal("wrong total supply:", token.TotalSupply)
	}

	// 3
	nonStandard3 := base.HexToAddress("0xc4e0f3ec24972c75df7c716922096f4270b7bb4e")
	token, err = GetState(chain, nonStandard3, blockNumber)
	if err != nil {
		t.Fatal(err)
	}

	if token.IsErc20() || token.Type == TokenErc20 {
		t.Fatal("token reported as ERC20")
	}

	if !token.IsErc721() || token.Type != TokenErc721 {
		t.Fatal("token reported as non-ERC721")
	}

	if token.Name != "Crowdfunded Mirror Editions" {
		t.Fatal("wrong name:", token.Name)
	}

	if token.Symbol != "CROWDFUND_EDITIONS" {
		t.Fatal("wrong symbol:", token.Symbol)
	}

	if token.Decimals != 0 {
		t.Fatal("wrong decimals:", token.Decimals)
	}

	if token.TotalSupply != "" {
		t.Fatal("wrong total supply:", token.TotalSupply)
	}
}
