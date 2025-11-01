//go:build integration
// +build integration

package rpc

import (
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/utils"
)

func TestGetContractDeployBlock(t *testing.T) {
	conn := TempConnection(utils.GetTestChain())

	// Finding the first block
	unchainedIndex := base.HexToAddress("0x0C316b7042b419d07d343F2f4F5Bd54FF731183d")
	expected := base.Blknum(14957097)
	result, err := conn.GetContractDeployBlock(unchainedIndex)
	if err != nil {
		t.Fatal(err)
	}
	if result != expected {
		t.Fatal("wrong result", result)
	}
	cached, ok := deployedCache[unchainedIndex]
	if !ok {
		t.Fatal("block was not cached")
	}
	if cached != expected {
		t.Fatal("cached value is incorrect", cached)
	}

	// Reading from cache shortcut
	fakeBlock := base.Blknum(13000000)
	deployedCache[unchainedIndex] = fakeBlock
	result, err = conn.GetContractDeployBlock(unchainedIndex)
	if err != nil {
		t.Fatal(err)
	}
	if result != fakeBlock {
		t.Fatal("result was not read from cache", result)
	}

	// Error when the address is not a contract
	_, err = conn.GetContractDeployBlock(base.ZeroAddr)
	if err != ErrNotAContract {
		t.Fatal("expected ErrNotAContract, but got", err)
	}
}

func TestGetProxy(t *testing.T) {
	//0x7cfb89d6df1b89a678dcaf42b9c262593e881e22
	//0xafc2f2d803479a2af3a72022d54cc0901a0ec0d6

	// 0x4Fabb145d64652a948d72533023f6E7A623C7C53
	// 0x00000000441378008ea67f4284a57932b1c000a5
	conn := TempConnection(utils.GetTestChain())

	proxy, err := conn.GetContractProxyAt(base.HexToAddress("0x4Fabb145d64652a948d72533023f6E7A623C7C53"), base.Blknum(12983248))
	if err != nil {
		t.Fatal(err)
	}
	if proxy.IsZero() {
		t.Fatal("got zero address")
	}
	if proxy.Hex() != "0x5864c777697bf9881220328bf2f16908c9afcd7e" {
		t.Fatal("wrong proxy address:", proxy)
	}
}
