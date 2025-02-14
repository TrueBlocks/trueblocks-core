package ledger2

import (
	"fmt"
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
)

func TestNewPosting(t *testing.T) {
	blk := base.Blknum(123)
	txIdx := base.Txnum(1)
	idx := "log_0"
	ts := base.Timestamp(999999)

	p := NewPosting(blk, txIdx, idx, ts)
	if p.BlockNumber != blk {
		t.Fatalf("BlockNumber mismatch. got=%d want=%d", p.BlockNumber, blk)
	}
	if p.TransactionIndex != txIdx {
		t.Fatalf("TransactionIndex mismatch. got=%d want=%d", p.TransactionIndex, txIdx)
	}
	if p.Index != idx {
		t.Fatalf("Index mismatch. got=%s want=%s", p.Index, idx)
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
		{"TransferIn", p.TransferIn},
		{"MiningRewardIn", p.MiningRewardIn},
		{"UncleRewardIn", p.UncleRewardIn},
		{"WithdrawalIn", p.WithdrawalIn},
		{"SelfDestructIn", p.SelfDestructIn},
		{"PrefundIn", p.PrefundIn},
		{"CorrectingIn", p.CorrectingIn},
		{"TransferOut", p.TransferOut},
		{"GasOut", p.GasOut},
		{"InternalTxFeesOut", p.InternalTxFeesOut},
		{"CorrectingOut", p.CorrectingOut},
		{"RunningBalance", p.RunningBalance},
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
	p := NewPosting(10, 0, "abc", base.Timestamp(12345))
	p.TransferIn = *base.NewWei(100)
	p.MiningRewardIn = *base.NewWei(10)
	p.UncleRewardIn = *base.NewWei(1)
	p.TransferOut = *base.NewWei(50)
	p.GasOut = *base.NewWei(5)
	fmt.Println(p.String())

	totalIn := p.TotalIn()
	if totalIn.String() != "111" {
		t.Fatalf("TotalIn mismatch. got=%s want=111", totalIn.String())
	}

	totalOut := p.TotalOut()
	if totalOut.String() != "55" {
		t.Fatalf("TotalOut mismatch. got=%s want=55", totalOut.String())
	}

	netVal := p.NetValue()
	if netVal.String() != "56" {
		t.Fatalf("NetValue mismatch. got=%s want=56", netVal.String())
	}
}

func TestPostingString(t *testing.T) {
	p := NewPosting(50, 2, "xyz", 123456)
	p.TransferIn = *base.NewWei(123)
	p.TransferOut = *base.NewWei(100)

	got := p.String()
	want := "Posting(Block=50 Tx=2 Index=xyz In=123 Out=100 Net=23 Time=123456)"
	if got != want {
		t.Fatalf("String mismatch.\ngot:  %s\nwant: %s", got, want)
	}
}
