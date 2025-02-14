package ledger2

import (
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
)

func TestNewAssetTransfer(t *testing.T) {
	blockNum := base.Blknum(12345)
	txIndex := base.Txnum(3)
	assetAddr := base.HexToAddress("0x1234")
	assetName := "TEST"
	amount := base.NewWei(999)
	indexVal := "trace_1"
	fromAddr := base.HexToAddress("0x1001")
	toAddr := base.HexToAddress("0x1003")

	at := NewAssetTransfer(
		blockNum,
		txIndex,
		assetAddr,
		assetName,
		*amount,
		indexVal,
		fromAddr,
		toAddr,
	)

	if at.BlockNumber != blockNum {
		t.Fatalf("BlockNumber mismatch. got=%d want=%d", at.BlockNumber, blockNum)
	}
	if at.TransactionIndex != txIndex {
		t.Fatalf("TransactionIndex mismatch. got=%d want=%d", at.TransactionIndex, txIndex)
	}
	if at.AssetAddress != assetAddr {
		t.Fatalf("AssetAddress mismatch. got=%s want=%s", at.AssetAddress, assetAddr)
	}
	if at.AssetName != assetName {
		t.Fatalf("AssetName mismatch. got=%s want=%s", at.AssetName, assetName)
	}
	if at.Amount.Cmp(amount) != 0 {
		t.Fatalf("Amount mismatch. got=%s want=%s", at.Amount.String(), amount.String())
	}
	if at.Index != indexVal {
		t.Fatalf("Index mismatch. got=%s want=%s", at.Index, indexVal)
	}
	if at.FromAddress != fromAddr {
		t.Fatalf("FromAddress mismatch. got=%s want=%s", at.FromAddress, fromAddr)
	}
	if at.ToAddress != toAddr {
		t.Fatalf("ToAddress mismatch. got=%s want=%s", at.ToAddress, toAddr)
	}

	zero := base.NewWei(0)
	fields := []struct {
		name string
		val  base.Wei
	}{
		{"GasCost", at.GasCost},
		{"MiningReward", at.MiningReward},
		{"UncleReward", at.UncleReward},
		{"Withdrawal", at.Withdrawal},
		{"SelfDestruct", at.SelfDestruct},
		{"Prefund", at.Prefund},
		{"InternalTxFees", at.InternalTxFees},
	}
	for _, fld := range fields {
		if fld.val.Cmp(zero) != 0 {
			t.Fatalf("Expected %s=0. got=%s", fld.name, fld.val.String())
		}
	}
}

// func TestAssetTransferString(t *testing.T) {
// 	at := NewAssetTransfer(
// 		100,
// 		2,
// 		base.HexToAddress("0xabcdef"),
// 		"TEST",
// 		*base.NewWei(500),
// 		"log_5",
// 		base.HexToAddress("0xFrom"),
// 		base.HexToAddress("0xTo"),
// 	)

// 	got := at.String()
// 	want := "AssetTransfer(Block=100 Tx=2 Asset=0xAsset Amount=500 Index=log_5 From=0xFrom To=0xTo)"
// 	if got != want {
// 		t.Fatalf("String mismatch.\ngot:  %s\nwant: %s", got, want)
// 	}
// }
