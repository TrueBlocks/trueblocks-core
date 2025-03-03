package ledger2

import (
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/ledger3"
)

func TestNewAssetTransfer(t *testing.T) {
	blockNum := base.Blknum(12345)
	txIndex := base.Txnum(3)
	assetAddress := base.HexToAddress("0x1234")
	assetSymbol := "TEST"
	amount := base.NewWei(999)
	indexVal := base.Lognum(1)
	fromAddr := base.HexToAddress("0x1001")
	toAddr := base.HexToAddress("0x1003")

	at := ledger3.AssetTransfer{
		BlockNumber:      blockNum,
		TransactionIndex: txIndex,
		AssetAddress:     assetAddress,
		AssetSymbol:      assetSymbol,
		AmountIn:         *amount,
		LogIndex:         indexVal,
		Sender:           fromAddr,
		Recipient:        toAddr,
	}

	if at.BlockNumber != blockNum {
		t.Fatalf("BlockNumber mismatch. got=%d want=%d", at.BlockNumber, blockNum)
	}
	if at.TransactionIndex != txIndex {
		t.Fatalf("TransactionIndex mismatch. got=%d want=%d", at.TransactionIndex, txIndex)
	}
	if at.AssetAddress != assetAddress {
		t.Fatalf("AssetAddress mismatch. got=%s want=%s", at.AssetAddress, assetAddress)
	}
	if at.AssetSymbol != assetSymbol {
		t.Fatalf("AssetSymbol mismatch. got=%s want=%s", at.AssetSymbol, assetSymbol)
	}
	if at.AmountIn.Cmp(amount) != 0 {
		t.Fatalf("Amount mismatch. got=%s want=%s", at.AmountIn.String(), amount.String())
	}
	if at.LogIndex != indexVal {
		t.Fatalf("Index mismatch. got=%d want=%d", at.LogIndex, indexVal)
	}
	if at.Sender != fromAddr {
		t.Fatalf("FromAddress mismatch. got=%s want=%s", at.Sender, fromAddr)
	}
	if at.Recipient != toAddr {
		t.Fatalf("ToAddress mismatch. got=%s want=%s", at.Recipient, toAddr)
	}

	zero := base.NewWei(0)
	fields := []struct {
		name string
		val  base.Wei
	}{
		{"GasCost", at.GasOut},
		{"MiningReward", at.MinerBaseRewardIn},
		{"UncleReward", at.MinerUncleRewardIn},
		// {"Withdrawal", at.Withdrawal},
		{"SelfDestruct", at.SelfDestructIn},
		{"Prefund", at.PrefundIn},
		{"InternalTxFees", at.InternalOut},
	}
	for _, fld := range fields {
		if fld.val.Cmp(zero) != 0 {
			t.Fatalf("Expected %s=0. got=%s", fld.name, fld.val.String())
		}
	}
}
