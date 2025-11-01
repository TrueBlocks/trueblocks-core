package types

import (
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/base"
)

func TestIsMaterial(t *testing.T) {
	stmt1 := new(Statement)
	if stmt1.IsMaterial() {
		t.Error("Expected IsMaterial to return false for a statement with all zero amounts")
	}

	stmt2 := new(Statement)
	stmt2.AmountIn = *base.NewWei(10) // nonzero incoming amount
	if !stmt2.IsMaterial() {
		t.Error("Expected IsMaterial to return true for a statement with nonzero AmountIn")
	}

	stmt3 := new(Statement)
	stmt3.AmountOut = *base.NewWei(20) // nonzero outgoing amount
	if !stmt3.IsMaterial() {
		t.Error("Expected IsMaterial to return true for a statement with nonzero AmountOut")
	}
}

func TestIsStableCoin(t *testing.T) {
	dai := base.HexToAddress("0x6b175474e89094c44da98b954eedeac495271d0f")
	stmtStable := new(Statement)
	stmtStable.Asset = dai // using DAI as an example
	if !stmtStable.IsStableCoin() {
		t.Errorf("Expected IsStableCoin to return true for address %s", dai.Hex())
	}

	stmtNonStable := new(Statement)
	stmtNonStable.Asset = base.HexToAddress("0x1234567890abcdef1234567890abcdef12345678")
	if stmtNonStable.IsStableCoin() {
		t.Errorf("Expected IsStableCoin to return false for address %s", stmtNonStable.Asset.Hex())
	}
}

func TestReconciled(t *testing.T) {
	stmt1 := new(Statement)
	stmt1.PrevBal = *base.NewWei(1000)
	stmt1.BegBal = *base.NewWei(1000)
	stmt1.AmountIn = *base.NewWei(50)
	stmt1.AmountOut = *base.NewWei(0)
	stmt1.EndBal = *base.NewWei(1050)
	if !stmt1.Reconciled() {
		t.Error("Expected Reconciled to return true for a reconciled statement")
	}

	stmt2 := new(Statement)
	stmt2.PrevBal = *base.NewWei(900)
	stmt2.BegBal = *base.NewWei(1000)
	stmt2.AmountIn = *base.NewWei(50)
	stmt2.AmountOut = *base.NewWei(0)
	stmt2.EndBal = *base.NewWei(1040)
	if stmt2.Reconciled() {
		t.Error("Expected Reconciled to return false for a non-reconciled statement")
	}
}

func TestIsEth(t *testing.T) {
	ethStmt := &Statement{
		Asset: base.FAKE_ETH_ADDRESS,
	}
	if !ethStmt.IsEth() {
		t.Error("Expected statement to be ETH")
	}

	nonEthStmt := &Statement{
		Asset: base.HexToAddress("0x1234567890abcdef1234567890abcdef12345678"),
	}
	if nonEthStmt.IsEth() {
		t.Error("Expected statement not to be ETH")
	}
}

func TestTotalIn(t *testing.T) {
	stmt := new(Statement)
	stmt.AmountIn = *base.NewWei(10)
	stmt.InternalIn = *base.NewWei(20)
	stmt.SelfDestructIn = *base.NewWei(30)
	stmt.MinerBaseRewardIn = *base.NewWei(40)
	stmt.MinerNephewRewardIn = *base.NewWei(50)
	stmt.MinerTxFeeIn = *base.NewWei(60)
	stmt.MinerUncleRewardIn = *base.NewWei(70)
	stmt.CorrectBegBalIn = *base.NewWei(2)
	stmt.CorrectAmountIn = *base.NewWei(80)
	stmt.CorrectEndBalIn = *base.NewWei(2)
	stmt.PrefundIn = *base.NewWei(90)

	totalIn := stmt.TotalIn()
	expected := base.NewWei(454)
	if totalIn.Cmp(expected) != 0 {
		t.Errorf("TotalIn: expected %s, got %s", expected.Text(10), totalIn.Text(10))
	}
}

func TestTotalOut(t *testing.T) {
	stmt := new(Statement)
	stmt.AmountOut = *base.NewWei(15)
	stmt.InternalOut = *base.NewWei(25)
	stmt.CorrectBegBalOut = *base.NewWei(2)
	stmt.CorrectAmountOut = *base.NewWei(35)
	stmt.CorrectEndBalOut = *base.NewWei(2)
	stmt.SelfDestructOut = *base.NewWei(45)
	stmt.GasOut = *base.NewWei(55)

	totalOut := stmt.TotalOut()
	expected := base.NewWei(179)
	if totalOut.Cmp(expected) != 0 {
		t.Errorf("TotalOut: expected %s, got %s", expected.Text(10), totalOut.Text(10))
	}
}

func TestAmountNet(t *testing.T) {
	stmt := new(Statement)
	stmt.AmountIn = *base.NewWei(10)
	stmt.InternalIn = *base.NewWei(20)
	stmt.SelfDestructIn = *base.NewWei(30)
	stmt.MinerBaseRewardIn = *base.NewWei(40)
	stmt.MinerNephewRewardIn = *base.NewWei(50)
	stmt.MinerTxFeeIn = *base.NewWei(60)
	stmt.MinerUncleRewardIn = *base.NewWei(70)
	stmt.CorrectAmountIn = *base.NewWei(80)
	stmt.PrefundIn = *base.NewWei(90)
	stmt.AmountOut = *base.NewWei(15)
	stmt.InternalOut = *base.NewWei(25)
	stmt.CorrectAmountOut = *base.NewWei(35)
	stmt.SelfDestructOut = *base.NewWei(45)
	stmt.GasOut = *base.NewWei(55)

	amountNet := stmt.AmountNet()
	expected := base.NewWei(275)
	if amountNet.Cmp(expected) != 0 {
		t.Errorf("AmountNet: expected %s, got %s", expected.Text(10), amountNet.Text(10))
	}
}

func TestBegBalDiff(t *testing.T) {
	stmt := Statement{
		BlockNumber: 0,
		PrevBal:     *base.NewWei(90),  // e.g., 90
		BegBal:      *base.NewWei(100), // e.g., 100
	}
	diff := stmt.BegBalDiff()
	expected := base.NewWei(0)
	if diff.Cmp(expected) != 0 {
		t.Errorf("BegBalDiff() with BlockNumber==0: expected %s, got %s", expected.Text(10), diff.Text(10))
	}

	// Case 2: BlockNumber != 0.
	// Let PrevBal = 90 and BegBal = 100, then BegBalDiff should be 10.
	stmt.BlockNumber = 1
	stmt.PrevBal = *base.NewWei(90)
	stmt.BegBal = *base.NewWei(100)
	diff = stmt.BegBalDiff()
	expected = base.NewWei(10)
	if diff.Cmp(expected) != 0 {
		t.Errorf("BegBalDiff() with BlockNumber!=0: expected %s, got %s", expected.Text(10), diff.Text(10))
	}
}

func TestEndBalCalc(t *testing.T) {
	stmt := Statement{
		BegBal:      *base.NewWei(100),
		AmountIn:    *base.NewWei(50),
		AmountOut:   *base.NewWei(0),
		InternalIn:  *base.NewWei(0),
		InternalOut: *base.NewWei(0),
	}
	endCalc := stmt.EndBalCalc()
	expected := base.NewWei(150)
	if endCalc.Cmp(expected) != 0 {
		t.Errorf("EndBalCalc(): expected %s, got %s", expected.Text(10), endCalc.Text(10))
	}
}

func TestEndBalDiff(t *testing.T) {
	stmt := Statement{
		BegBal:      *base.NewWei(100),
		AmountIn:    *base.NewWei(50),
		AmountOut:   *base.NewWei(0),
		EndBal:      *base.NewWei(140),
		InternalIn:  *base.NewWei(0),
		InternalOut: *base.NewWei(0),
	}
	diff := stmt.EndBalDiff()
	expected := base.NewWei(10)
	if diff.Cmp(expected) != 0 {
		t.Errorf("EndBalDiff(): expected %s, got %s", expected.Text(10), diff.Text(10))
	}
}

// ---------------------------------------------------------------------------
// 5. Test arithmetic edge cases by using very large Wei values

func TestArithmeticEdgeCasesLargeNumbers(t *testing.T) {
	// Use a "huge" Wei value, e.g. 1e18.
	huge := base.NewWei(1000000000000000000) // 1e18
	stmt := &Statement{
		// For TotalIn, there are 9 fields:
		AmountIn:            *huge,
		InternalIn:          *huge,
		SelfDestructIn:      *huge,
		MinerBaseRewardIn:   *huge,
		MinerNephewRewardIn: *huge,
		MinerTxFeeIn:        *huge,
		MinerUncleRewardIn:  *huge,
		CorrectAmountIn:     *huge,
		PrefundIn:           *huge,
		// For TotalOut, there are 5 fields:
		AmountOut:        *huge,
		InternalOut:      *huge,
		CorrectAmountOut: *huge,
		SelfDestructOut:  *huge,
		GasOut:           *huge,
	}

	// Expected TotalIn: 9 * 1e18
	totalIn := stmt.TotalIn()
	expectedTotalIn := base.NewWei(9 * 1000000000000000000)
	if totalIn.Cmp(expectedTotalIn) != 0 {
		t.Errorf("TotalIn with huge values: expected %s, got %s", expectedTotalIn.Text(10), totalIn.Text(10))
	}

	// Expected TotalOut: 5 * 1e18
	totalOut := stmt.TotalOut()
	expectedTotalOut := base.NewWei(5 * 1000000000000000000)
	if totalOut.Cmp(expectedTotalOut) != 0 {
		t.Errorf("TotalOut with huge values: expected %s, got %s", expectedTotalOut.Text(10), totalOut.Text(10))
	}

	// AmountNet should be TotalIn - TotalOut = 4e18.
	amountNet := stmt.AmountNet()
	expectedAmountNet := base.NewWei(4 * 1000000000000000000)
	if amountNet.Cmp(expectedAmountNet) != 0 {
		t.Errorf("AmountNet with huge values: expected %s, got %s", expectedAmountNet.Text(10), amountNet.Text(10))
	}

	// Test BegBalDiff: when BlockNumber != 0, it should be BegBal - PrevBal.
	stmt.BlockNumber = 1
	stmt.BegBal = *base.NewWei(2 * 1000000000000000000) // 2e18
	stmt.PrevBal = *huge                                // 1e18
	begBalDiff := stmt.BegBalDiff()
	expectedBegBalDiff := huge // 1e18
	if begBalDiff.Cmp(expectedBegBalDiff) != 0 {
		t.Errorf("BegBalDiff with huge values: expected %s, got %s", expectedBegBalDiff.Text(10), begBalDiff.Text(10))
	}
}

func TestIsNullTransfer(t *testing.T) {
	// 	// Case 1: lotsOfLogs true, airdrop true, noBalanceChange true.
	t.Run("Case 1: lotsOfLogs true, airdrop true, noBalanceChange true", func(t *testing.T) {
		stmt := new(Statement)
		stmt.Sender = base.HexToAddress("0x0")
		stmt.BegBal = *base.NewWei(100)
		stmt.EndBal = *base.NewWei(100)
		stmt.AmountIn = *base.NewWei(50)

		if !stmt.IsNullTransfer(11, base.HexToAddress("0xABC")) {
			t.Error("Case 1: Expected IsNullTransfer to return true")
		}
	})

	t.Run("Case 2: lotsOfLogs false, airdrop true, noBalanceChange true", func(t *testing.T) {
		stmt := new(Statement)
		stmt.Sender = base.HexToAddress("0x0") // triggers airdrop condition.
		stmt.BegBal = *base.NewWei(100)
		stmt.EndBal = *base.NewWei(100)
		stmt.AmountIn = *base.NewWei(50)

		if !stmt.IsNullTransfer(5, base.HexToAddress("0xABC")) {
			t.Error("Case 2: Expected IsNullTransfer to return true")
		}
	})

	t.Run("Case 3: lotsOfLogs true, airdrop false, noBalanceChange true", func(t *testing.T) {
		stmt := new(Statement)
		stmt.Sender = base.HexToAddress("0xABC")
		stmt.BegBal = *base.NewWei(100)
		stmt.EndBal = *base.NewWei(100)
		stmt.AmountIn = *base.NewWei(50)

		if !stmt.IsNullTransfer(11, base.HexToAddress("0xDEF")) {
			t.Error("Case 3: Expected IsNullTransfer to return true")
		}
	})

	t.Run("Case 4: lotsOfLogs false, airdrop false, noBalanceChange true", func(t *testing.T) {
		stmt := new(Statement)
		stmt.Sender = base.HexToAddress("0xABC")
		stmt.BegBal = *base.NewWei(100)
		stmt.EndBal = *base.NewWei(100)
		stmt.AmountIn = *base.NewWei(50)

		if stmt.IsNullTransfer(5, base.HexToAddress("0xDEF")) {
			t.Error("Case 4: Expected IsNullTransfer to return false")
		}
	})

	t.Run("Case 5: Balance change present", func(t *testing.T) {
		stmt := new(Statement)
		stmt.Sender = base.HexToAddress("0x0")
		stmt.BegBal = *base.NewWei(100)
		stmt.EndBal = *base.NewWei(110) // Balance changes: difference of 10.
		stmt.AmountIn = *base.NewWei(50)

		if stmt.IsNullTransfer(11, base.HexToAddress("0x0")) {
			t.Error("Case 5: Expected IsNullTransfer to return false due to balance change")
		}
	})

	t.Run("Case 6: Not material", func(t *testing.T) {
		stmt := new(Statement)
		stmt.Sender = base.HexToAddress("0x0")
		stmt.BegBal = *base.NewWei(100)
		stmt.EndBal = *base.NewWei(100)
		if stmt.IsNullTransfer(11, base.HexToAddress("0x0")) {
			t.Error("Case 6: Expected IsNullTransfer to return false because statement is not material")
		}
	})
}
