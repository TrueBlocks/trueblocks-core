package ledger2

import (
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func TestNewPosting(t *testing.T) {
	blk := base.Blknum(123)
	txIdx := base.Txnum(1)
	idx := base.Lognum(0)
	ts := base.Timestamp(999999)
	p := types.NewPosting(blk, txIdx, idx, ts)
	if p.BlockNumber != blk {
		t.Fatalf("BlockNumber mismatch. got=%d want=%d", p.BlockNumber, blk)
	}
	if p.TransactionIndex != txIdx {
		t.Fatalf("TransactionIndex mismatch. got=%d want=%d", p.TransactionIndex, txIdx)
	}
	if p.LogIndex != idx {
		t.Fatalf("Index mismatch. got=%d want=%d", p.LogIndex, idx)
	}
	if p.Timestamp != ts {
		t.Fatalf("Timestamp mismatch. got=%d want=%d", p.Timestamp, ts)
	}

	// Confirm that all base.Wei fields are zero by default.
	zero := base.NewWei(0)
	fields := []struct {
		name string
		val  base.Wei
	}{
		{"AmountIn", p.AmountIn},
		{"MiningRewardIn", p.MinerBaseRewardIn},
		{"UncleRewardIn", p.MinerUncleRewardIn},
		// {"WithdrawalIn", p.Withdrawal},
		{"SelfDestructIn", p.SelfDestructIn},
		{"PrefundIn", p.PrefundIn},
		{"CorrectingIn", p.CorrectingIn},
		{"TransferOut", p.AmountOut},
		{"GasOut", p.GasOut},
		{"InternalOut", p.InternalOut},
		{"CorrectingOut", p.CorrectingOut},
		// {"RunningBalance", p.RunningBalance},
	}
	for _, fld := range fields {
		if fld.val.Cmp(zero) != 0 {
			t.Fatalf("Expected %s=0. got=%s", fld.name, fld.val.String())
		}
	}

	if p.CorrectingReason != "" {
		t.Fatalf("Expected CorrectingReason to be empty. got=%s", p.CorrectingReason)
	}
}

func TestPostingCalculations(t *testing.T) {
	p := types.NewPosting(10, 0, 1, base.Timestamp(12345))
	p.AmountIn = *base.NewWei(100)
	p.MinerBaseRewardIn = *base.NewWei(10)
	p.MinerUncleRewardIn = *base.NewWei(1)
	p.AmountOut = *base.NewWei(50)
	p.GasOut = *base.NewWei(5)
	// fmt.Println(p.String())

	totalIn := p.TotalIn()
	if totalIn.String() != "111" {
		t.Fatalf("TotalIn mismatch. got=%s want=111", totalIn.String())
	}

	totalOut := p.TotalOut()
	if totalOut.String() != "55" {
		t.Fatalf("TotalOut mismatch. got=%s want=55", totalOut.String())
	}

	netVal := p.AmountNet()
	if netVal.String() != "56" {
		t.Fatalf("NetValue mismatch. got=%s want=56", netVal.String())
	}
}

// func TestPostingString(t *testing.T) {
// 	p := Posting{
// 		BlockNumber:      50,
// 		TransactionIndex: 2,
// 		LogIndex:         12,
// 		Timestamp:        base.Timestamp(123456),
// 	}
// 	p.AmountIn = *base.NewWei(123)
// 	p.AmountOut = *base.NewWei(100)

// 	got := p.String()
// 	want := `{"accountedFor":"0x0","amountIn":{},"amountOut":{},"assetAddress":"0x0","assetSymbol":"","begBal":{},"blockNumber":50,"blockNumberPrev":0,"blockNumberNext":0,"correctingIn":{},"correctingOut":{},"decimals":0,"endBal":{},"gasOut":{},"internalIn":{},"internalOut":{},"logIndex":12,"minerBaseRewardIn":{},"minerNephewRewardIn":{},"minerTxFeeIn":{},"minerUncleRewardIn":{},"prefundIn":{},"prevBal":{},"priceSource":"","recipient":"0x0","rollingBalance":{},"selfDestructIn":{},"selfDestructOut":{},"sender":"0x0","spotPrice":0,"timestamp":123456,"transactionHash":"0x0000000000000000000000000000000000000000000000000000000000000000","transactionIndex":2}`
// 	if got != want {
// 		t.Fatalf("String mismatch.\ngot:  %s\nwant: %s", got, want)
// 	}
// }
