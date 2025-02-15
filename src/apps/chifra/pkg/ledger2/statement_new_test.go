package ledger2

import (
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
)

func TestNewStatementNew(t *testing.T) {
	sb := base.NewWei(100)
	eb := base.NewWei(200)
	ti := base.NewWei(300)
	to := base.NewWei(50)

	st := NewStatementNew(
		"0xASSET",
		10,
		20,
		*sb,
		*eb,
		*ti,
		*to,
		base.Timestamp(99999),
	)

	if st.AssetAddr != "0xASSET" {
		t.Fatalf("AssetAddr mismatch. got=%s want=0xASSET", st.AssetAddr)
	}
	if st.StartBlock != 10 {
		t.Fatalf("StartBlock mismatch. got=%d want=10", st.StartBlock)
	}
	if st.EndBlock != 20 {
		t.Fatalf("EndBlock mismatch. got=%d want=20", st.EndBlock)
	}
	if st.StartBalance.Cmp(sb) != 0 {
		t.Fatalf("StartBalance mismatch. got=%s want=%s", st.StartBalance.String(), sb.String())
	}
	if st.EndBalance.Cmp(eb) != 0 {
		t.Fatalf("EndBalance mismatch. got=%s want=%s", st.EndBalance.String(), eb.String())
	}
	if st.TotalIn.Cmp(ti) != 0 {
		t.Fatalf("TotalIn mismatch. got=%s want=%s", st.TotalIn.String(), ti.String())
	}
	if st.TotalOut.Cmp(to) != 0 {
		t.Fatalf("TotalOut mismatch. got=%s want=%s", st.TotalOut.String(), to.String())
	}
	if st.GeneratedAt != 99999 {
		t.Fatalf("GeneratedAt mismatch. got=%d want=99999", st.GeneratedAt)
	}
}

func TestStatementNewString(t *testing.T) {
	st := NewStatementNew(
		"0xABC",
		1,
		10,
		*base.NewWei(50),
		*base.NewWei(100),
		*base.NewWei(150),
		*base.NewWei(20),
		base.Timestamp(12345),
	)

	got := st.String()
	want := "StatementNew(Asset=0xABC StartBlock=1 EndBlock=10 StartBal=50 EndBal=100 In=150 Out=20 GeneratedAt=12345)"
	if got != want {
		t.Fatalf("String mismatch.\ngot:  %s\nwant: %s", got, want)
	}
}
