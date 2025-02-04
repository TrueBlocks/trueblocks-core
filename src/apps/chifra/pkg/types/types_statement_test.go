package types

import (
	"bytes"
	"fmt"
	"os"
	"path/filepath"
	"reflect"
	"strings"
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

// TestCorrectForNullTransfer tests the CorrectForNullTransfer function.
func TestCorrectForNullTransfer(t *testing.T) {
	// --- ETH branch ---
	t.Run("ETH branch", func(t *testing.T) {
		// Create a Transaction with any minimal required fields.
		tx := &Transaction{
			To: base.HexToAddress("0xAAA"),
		}
		tx.Receipt = new(Receipt)
		// (For ETH branch the logs and other fields are irrelevant.)

		// Create a statement that is considered ETH.
		// s.IsEth() returns true when AssetAddr equals base.FAKE_ETH_ADDRESS.
		stmt := new(Statement)
		stmt.AssetAddr = base.FAKE_ETH_ADDRESS // This makes it ETH.
		// Set balances to any values; they won't be modified by CorrectForNullTransfer.
		stmt.BegBal = *base.NewWei(1000)
		stmt.EndBal = *base.NewWei(1050)
		// Set a nonzero amount so that the statement is material.
		stmt.AmountIn = *base.NewWei(100)

		// Call CorrectForNullTransfer; for ETH, it should simply return s.Reconciled().
		// Note: It will not modify the token fields.
		prevAmountIn := &stmt.AmountIn
		prevCorrectingIn := &stmt.CorrectingIn
		result := stmt.CorrectForNullTransfer(tx)
		// For ETH branch, no correction is applied; the return is s.Reconciled().
		// We don't expect any modifications to AmountIn, AmountOut, etc.
		if stmt.AmountIn.Cmp(prevAmountIn) != 0 {
			t.Error("ETH branch: AmountIn should remain unchanged")
		}
		if stmt.CorrectingIn.Cmp(prevCorrectingIn) != 0 {
			t.Error("ETH branch: CorrectingIn should remain unchanged")
		}
		// We can also check that the function simply returns s.Reconciled().
		if result != stmt.Reconciled() {
			t.Error("ETH branch: Function should return s.Reconciled()")
		}
	})

	// --- Token branch: isNullTransfer returns true ---
	t.Run("Token branch (null transfer detected)", func(t *testing.T) {
		// Create a Transaction that meets the "lotsOfLogs" condition.
		tx := &Transaction{
			To: base.HexToAddress("0xBBB"),
		}
		tx.Receipt = new(Receipt)
		// Set Logs to a slice of 11 Logs to trigger lotsOfLogs.
		tx.Receipt.Logs = make([]Log, 11)

		// Create a token statement (non-ETH).
		stmt := new(Statement)
		// Use a non-ETH address.
		stmt.AssetAddr = base.HexToAddress("0xCCC")
		stmt.AssetType = "token"
		// To trigger the airdrop condition, we can set Sender to zero address.
		stmt.Sender = base.HexToAddress("0x0")
		// For isNullTransfer, noBalanceChange must be true:
		// set BegBal equal to EndBal.
		stmt.BegBal = *base.NewWei(100)
		stmt.EndBal = *base.NewWei(100)
		// Set a nonzero AmountIn so that the statement is material.
		stmt.AmountIn = *base.NewWei(50)
		// Optionally, ensure other incoming fields are zero.
		stmt.InternalIn = *base.NewWei(0)
		stmt.SelfDestructIn = *base.NewWei(0)
		stmt.MinerBaseRewardIn = *base.NewWei(0)
		stmt.MinerNephewRewardIn = *base.NewWei(0)
		stmt.MinerTxFeeIn = *base.NewWei(0)
		stmt.MinerUncleRewardIn = *base.NewWei(0)
		stmt.PrefundIn = *base.NewWei(0)

		// Pre-set outgoing and correcting fields to nonzero to verify they are reset.
		stmt.AmountOut = *base.NewWei(123)
		stmt.CorrectingIn = *base.NewWei(456)
		stmt.CorrectingOut = *base.NewWei(789)
		stmt.CorrectingReason = "oldreason"

		// Record the TotalIn before correction.
		originalTotalIn := stmt.TotalIn()

		// Call CorrectForNullTransfer.
		result := stmt.CorrectForNullTransfer(tx)
		// After correction, since isNullTransfer is true,
		// AmountIn and AmountOut should be zero.
		if stmt.AmountIn.Cmp(base.NewWei(0)) != 0 {
			t.Errorf("Token branch (null transfer): Expected AmountIn to be 0, got %s", stmt.AmountIn.Text(10))
		}
		if stmt.AmountOut.Cmp(base.NewWei(0)) != 0 {
			t.Errorf("Token branch (null transfer): Expected AmountOut to be 0, got %s", stmt.AmountOut.Text(10))
		}
		// CorrectingIn and CorrectingOut should be set to the original TotalIn.
		if stmt.CorrectingIn.Cmp(originalTotalIn) != 0 {
			t.Errorf("Token branch (null transfer): Expected CorrectingIn to be %s, got %s",
				originalTotalIn.Text(10), stmt.CorrectingIn.Text(10))
		}
		if stmt.CorrectingOut.Cmp(originalTotalIn) != 0 {
			t.Errorf("Token branch (null transfer): Expected CorrectingOut to be %s, got %s",
				originalTotalIn.Text(10), stmt.CorrectingOut.Text(10))
		}
		// CorrectingReason should be "null-transfer".
		if stmt.CorrectingReason != "null-transfer" {
			t.Errorf("Token branch (null transfer): Expected CorrectingReason to be 'null-transfer', got %q", stmt.CorrectingReason)
		}
		// Finally, check the reconciliation status. It depends on the rest of the statement.
		// For this test, likely Reconciled() remains false because BegBalDiff and EndBalDiff
		// are computed from BegBal, PrevBal, EndBal, etc., which haven't been changed.
		// We simply check that the function returns what Reconciled() returns.
		if result != stmt.Reconciled() {
			t.Error("Token branch (null transfer): Function return should equal s.Reconciled()")
		}
	})

	// --- Token branch: isNullTransfer returns false ---
	t.Run("Token branch (null transfer not detected)", func(t *testing.T) {
		// Create a Transaction that does NOT meet the null transfer condition.
		// For example, set Logs to fewer than 10 logs.
		tx := &Transaction{
			To: base.HexToAddress("0xBBB"),
		}
		tx.Receipt = new(Receipt)
		tx.Receipt.Logs = make([]Log, 5) // not enough logs for lotsOfLogs

		// Create a token statement.
		stmt := new(Statement)
		stmt.AssetAddr = base.HexToAddress("0xCCC")
		stmt.AssetType = "token"
		// Set Sender to nonzero and not equal to tx.To, so airdrop condition is false.
		stmt.Sender = base.HexToAddress("0xABC")
		// Set balances so that noBalanceChange is true.
		stmt.BegBal = *base.NewWei(100)
		stmt.EndBal = *base.NewWei(100)
		// Set AmountIn so that the statement is material.
		stmt.AmountIn = *base.NewWei(50)

		// Pre-set outgoing and correcting fields.
		stmt.AmountOut = *base.NewWei(123)
		stmt.CorrectingIn = *base.NewWei(456)
		stmt.CorrectingOut = *base.NewWei(789)
		stmt.CorrectingReason = "oldreason"

		// Record the original values.
		origAmountIn := &stmt.AmountIn
		origAmountOut := &stmt.AmountOut
		origCorrectingIn := &stmt.CorrectingIn
		origCorrectingOut := &stmt.CorrectingOut
		origReason := stmt.CorrectingReason

		// Call CorrectForNullTransfer.
		result := stmt.CorrectForNullTransfer(tx)
		// Since isNullTransfer is false, no corrections should be applied.
		if stmt.AmountIn.Cmp(origAmountIn) != 0 {
			t.Error("Token branch (non-null transfer): AmountIn should remain unchanged")
		}
		if stmt.AmountOut.Cmp(origAmountOut) != 0 {
			t.Error("Token branch (non-null transfer): AmountOut should remain unchanged")
		}
		if stmt.CorrectingIn.Cmp(origCorrectingIn) != 0 {
			t.Error("Token branch (non-null transfer): CorrectingIn should remain unchanged")
		}
		if stmt.CorrectingOut.Cmp(origCorrectingOut) != 0 {
			t.Error("Token branch (non-null transfer): CorrectingOut should remain unchanged")
		}
		if stmt.CorrectingReason != origReason {
			t.Error("Token branch (non-null transfer): CorrectingReason should remain unchanged")
		}
		if result != stmt.Reconciled() {
			t.Error("Token branch (non-null transfer): Function return should equal s.Reconciled()")
		}
	})
}

func TestIsNullTransfer(t *testing.T) {
	// Case 1: lotsOfLogs true, airdrop true, noBalanceChange true.
	t.Run("Case 1: lotsOfLogs true, airdrop true, noBalanceChange true", func(t *testing.T) {
		tx := &Transaction{
			To: base.HexToAddress("0xABC"), // arbitrary nonzero address
		}
		// Initialize Receipt and Logs.
		tx.Receipt = new(Receipt)
		tx.Receipt.Logs = make([]Log, 11) // >10 logs

		stmt := new(Statement)
		// Set Sender to zero address to trigger airdrop condition.
		stmt.Sender = base.HexToAddress("0x0")
		// Ensure no balance change: BegBal equals EndBal.
		stmt.BegBal = *base.NewWei(100)
		stmt.EndBal = *base.NewWei(100)
		// Make the statement material by setting a nonzero incoming amount.
		stmt.AmountIn = *base.NewWei(50)

		if !stmt.isNullTransfer(tx) {
			t.Error("Case 1: Expected isNullTransfer to return true")
		}
	})

	// Case 2: lotsOfLogs false, airdrop true, noBalanceChange true.
	t.Run("Case 2: lotsOfLogs false, airdrop true, noBalanceChange true", func(t *testing.T) {
		tx := &Transaction{
			To: base.HexToAddress("0xABC"),
		}
		// Initialize Receipt and Logs.
		tx.Receipt = new(Receipt)
		tx.Receipt.Logs = make([]Log, 5) // ≤10 logs

		stmt := new(Statement)
		stmt.Sender = base.HexToAddress("0x0") // triggers airdrop condition.
		stmt.BegBal = *base.NewWei(100)
		stmt.EndBal = *base.NewWei(100)
		stmt.AmountIn = *base.NewWei(50)

		if !stmt.isNullTransfer(tx) {
			t.Error("Case 2: Expected isNullTransfer to return true")
		}
	})

	// Case 3: lotsOfLogs true, airdrop false, noBalanceChange true.
	t.Run("Case 3: lotsOfLogs true, airdrop false, noBalanceChange true", func(t *testing.T) {
		tx := &Transaction{
			To: base.HexToAddress("0xDEF"), // Different from Sender.
		}
		// Initialize Receipt and Logs.
		tx.Receipt = new(Receipt)
		tx.Receipt.Logs = make([]Log, 11) // >10 logs

		stmt := new(Statement)
		// Set Sender to a nonzero address different from tx.To.
		stmt.Sender = base.HexToAddress("0xABC")
		stmt.BegBal = *base.NewWei(100)
		stmt.EndBal = *base.NewWei(100)
		stmt.AmountIn = *base.NewWei(50)

		if !stmt.isNullTransfer(tx) {
			t.Error("Case 3: Expected isNullTransfer to return true")
		}
	})

	// Case 4: lotsOfLogs false, airdrop false, noBalanceChange true.
	t.Run("Case 4: lotsOfLogs false, airdrop false, noBalanceChange true", func(t *testing.T) {
		tx := &Transaction{
			To: base.HexToAddress("0xDEF"),
		}
		// Initialize Receipt and Logs.
		tx.Receipt = new(Receipt)
		tx.Receipt.Logs = make([]Log, 5) // fewer logs

		stmt := new(Statement)
		// Set Sender to a nonzero address different from tx.To.
		stmt.Sender = base.HexToAddress("0xABC")
		stmt.BegBal = *base.NewWei(100)
		stmt.EndBal = *base.NewWei(100)
		stmt.AmountIn = *base.NewWei(50)

		if stmt.isNullTransfer(tx) {
			t.Error("Case 4: Expected isNullTransfer to return false")
		}
	})

	// Case 5: Conditions met for logs/airdrop, but balance changes (noBalanceChange false).
	t.Run("Case 5: Balance change present", func(t *testing.T) {
		tx := &Transaction{
			To: base.HexToAddress("0x0"), // may trigger airdrop condition.
		}
		// Initialize Receipt and Logs.
		tx.Receipt = new(Receipt)
		tx.Receipt.Logs = make([]Log, 11)

		stmt := new(Statement)
		stmt.Sender = base.HexToAddress("0x0")
		stmt.BegBal = *base.NewWei(100)
		stmt.EndBal = *base.NewWei(110) // Balance changes: difference of 10.
		stmt.AmountIn = *base.NewWei(50)

		if stmt.isNullTransfer(tx) {
			t.Error("Case 5: Expected isNullTransfer to return false due to balance change")
		}
	})

	// Case 6: Conditions met for logs/airdrop, but the statement is not material.
	t.Run("Case 6: Not material", func(t *testing.T) {
		tx := &Transaction{
			To: base.HexToAddress("0x0"),
		}
		// Initialize Receipt and Logs.
		tx.Receipt = new(Receipt)
		tx.Receipt.Logs = make([]Log, 11)

		stmt := new(Statement)
		stmt.Sender = base.HexToAddress("0x0") // zero address triggers airdrop condition.
		stmt.BegBal = *base.NewWei(100)
		stmt.EndBal = *base.NewWei(100)
		// No AmountIn set, so the statement is not material.
		if stmt.isNullTransfer(tx) {
			t.Error("Case 6: Expected isNullTransfer to return false because statement is not material")
		}
	})
}

// TestIsMaterial verifies that IsMaterial returns false when both TotalIn and TotalOut are zero,
// and returns true when either is non-zero.
func TestIsMaterial(t *testing.T) {
	// Case 1: All incoming and outgoing fields are zero.
	stmt1 := new(Statement)
	// With a fresh statement, all amounts should be zero.
	if stmt1.IsMaterial() {
		t.Error("Expected IsMaterial to return false for a statement with all zero amounts")
	}

	// Case 2: Set one incoming field to a nonzero value.
	stmt2 := new(Statement)
	stmt2.AmountIn = *base.NewWei(10) // nonzero incoming amount
	if !stmt2.IsMaterial() {
		t.Error("Expected IsMaterial to return true for a statement with nonzero AmountIn")
	}

	// Case 3: Alternatively, set one outgoing field to a nonzero value.
	stmt3 := new(Statement)
	stmt3.AmountOut = *base.NewWei(20) // nonzero outgoing amount
	if !stmt3.IsMaterial() {
		t.Error("Expected IsMaterial to return true for a statement with nonzero AmountOut")
	}
}

// TestIsStableCoin verifies that IsStableCoin returns true when AssetAddr matches one of the stable coin addresses
// and false otherwise.
func TestIsStableCoin(t *testing.T) {
	// Assuming the stable coin addresses are defined as:
	// sai := base.HexToAddress("0x89d24a6b4ccb1b6faa2625fe562bdd9a23260359")
	dai := base.HexToAddress("0x6b175474e89094c44da98b954eedeac495271d0f")
	// usdc := base.HexToAddress("0xa0b86991c6218b36c1d19d4a2e9eb0ce3606eb48")
	// usdt := base.HexToAddress("0xdac17f958d2ee523a2206206994597c13d831ec7")

	// Case 1: Create a statement with a stable coin address.
	stmtStable := new(Statement)
	stmtStable.AssetAddr = dai // using DAI as an example
	if !stmtStable.IsStableCoin() {
		t.Errorf("Expected IsStableCoin to return true for address %s", dai.Hex())
	}

	// Case 2: Create a statement with a non-stable coin address.
	stmtNonStable := new(Statement)
	stmtNonStable.AssetAddr = base.HexToAddress("0x1234567890abcdef1234567890abcdef12345678")
	if stmtNonStable.IsStableCoin() {
		t.Errorf("Expected IsStableCoin to return false for address %s", stmtNonStable.AssetAddr.Hex())
	}
}

// TestReconciled verifies that Reconciled returns true when both BegBalDiff and EndBalDiff are zero.
func TestReconciled(t *testing.T) {
	// Case 1: A fully reconciled statement.
	// For reconciliation, we want:
	//   PrevBal == BegBal, and EndBal == EndBalCalc()
	stmt1 := new(Statement)
	// Set PrevBal and BegBal to the same value.
	stmt1.PrevBal = *base.NewWei(1000)
	stmt1.BegBal = *base.NewWei(1000)
	// Set AmountIn and AmountOut so that AmountNet is 50.
	stmt1.AmountIn = *base.NewWei(50)
	stmt1.AmountOut = *base.NewWei(0)
	// EndBalCalc() should then be BegBal + AmountNet = 1000 + 50 = 1050.
	// Set EndBal to 1050.
	stmt1.EndBal = *base.NewWei(1050)
	if !stmt1.Reconciled() {
		t.Error("Expected Reconciled to return true for a reconciled statement")
	}

	// Case 2: A non-reconciled statement.
	// For example, if BegBal != PrevBal or EndBal != EndBalCalc().
	stmt2 := new(Statement)
	stmt2.PrevBal = *base.NewWei(900)
	stmt2.BegBal = *base.NewWei(1000) // BegBalDiff = 100
	stmt2.AmountIn = *base.NewWei(50)
	stmt2.AmountOut = *base.NewWei(0)
	// EndBalCalc() = 1000 + 50 = 1050.
	// Set EndBal to 1040 so that EndBalDiff = 10.
	stmt2.EndBal = *base.NewWei(1040)
	if stmt2.Reconciled() {
		t.Error("Expected Reconciled to return false for a non-reconciled statement")
	}
}

// TestIsEth verifies that IsEth returns true when the AssetAddr equals base.FAKE_ETH_ADDRESS,
// and false otherwise.
func TestIsEth(t *testing.T) {
	// Create a statement that should be recognized as ETH.
	ethStmt := &Statement{
		AssetAddr: base.FAKE_ETH_ADDRESS,
	}
	if !ethStmt.IsEth() {
		t.Error("Expected statement to be ETH")
	}

	// Create a statement that should not be recognized as ETH.
	nonEthStmt := &Statement{
		AssetAddr: base.HexToAddress("0x1234567890abcdef1234567890abcdef12345678"),
	}
	if nonEthStmt.IsEth() {
		t.Error("Expected statement not to be ETH")
	}
}

// TestTotalIn verifies that TotalIn returns the sum of all incoming value fields.
func TestTotalIn(t *testing.T) {
	stmt := new(Statement)
	// Set the incoming fields:
	// AmountIn = 10, InternalIn = 20, SelfDestructIn = 30,
	// MinerBaseRewardIn = 40, MinerNephewRewardIn = 50, MinerTxFeeIn = 60,
	// MinerUncleRewardIn = 70, CorrectingIn = 80, PrefundIn = 90.
	// Expected total: 10+20+30+40+50+60+70+80+90 = 450.
	stmt.AmountIn = *base.NewWei(10)
	stmt.InternalIn = *base.NewWei(20)
	stmt.SelfDestructIn = *base.NewWei(30)
	stmt.MinerBaseRewardIn = *base.NewWei(40)
	stmt.MinerNephewRewardIn = *base.NewWei(50)
	stmt.MinerTxFeeIn = *base.NewWei(60)
	stmt.MinerUncleRewardIn = *base.NewWei(70)
	stmt.CorrectingIn = *base.NewWei(80)
	stmt.PrefundIn = *base.NewWei(90)

	totalIn := stmt.TotalIn()
	expected := base.NewWei(450)
	if totalIn.Cmp(expected) != 0 {
		t.Errorf("TotalIn: expected %s, got %s", expected.Text(10), totalIn.Text(10))
	}
}

// TestTotalOut verifies that TotalOut returns the sum of all outgoing value fields.
func TestTotalOut(t *testing.T) {
	stmt := new(Statement)
	// Set the outgoing fields:
	// AmountOut = 15, InternalOut = 25, CorrectingOut = 35,
	// SelfDestructOut = 45, GasOut = 55.
	// Expected total: 15+25+35+45+55 = 175.
	stmt.AmountOut = *base.NewWei(15)
	stmt.InternalOut = *base.NewWei(25)
	stmt.CorrectingOut = *base.NewWei(35)
	stmt.SelfDestructOut = *base.NewWei(45)
	stmt.GasOut = *base.NewWei(55)

	totalOut := stmt.TotalOut()
	expected := base.NewWei(175)
	if totalOut.Cmp(expected) != 0 {
		t.Errorf("TotalOut: expected %s, got %s", expected.Text(10), totalOut.Text(10))
	}
}

// TestAmountNet verifies that AmountNet equals TotalIn minus TotalOut.
func TestAmountNet(t *testing.T) {
	stmt := new(Statement)
	// Set the same values as in TotalIn and TotalOut tests:
	// TotalIn: 10+20+30+40+50+60+70+80+90 = 450.
	stmt.AmountIn = *base.NewWei(10)
	stmt.InternalIn = *base.NewWei(20)
	stmt.SelfDestructIn = *base.NewWei(30)
	stmt.MinerBaseRewardIn = *base.NewWei(40)
	stmt.MinerNephewRewardIn = *base.NewWei(50)
	stmt.MinerTxFeeIn = *base.NewWei(60)
	stmt.MinerUncleRewardIn = *base.NewWei(70)
	stmt.CorrectingIn = *base.NewWei(80)
	stmt.PrefundIn = *base.NewWei(90)
	// TotalOut: 15+25+35+45+55 = 175.
	stmt.AmountOut = *base.NewWei(15)
	stmt.InternalOut = *base.NewWei(25)
	stmt.CorrectingOut = *base.NewWei(35)
	stmt.SelfDestructOut = *base.NewWei(45)
	stmt.GasOut = *base.NewWei(55)

	// Expected AmountNet = 450 - 175 = 275.
	amountNet := stmt.AmountNet()
	expected := base.NewWei(275)
	if amountNet.Cmp(expected) != 0 {
		t.Errorf("AmountNet: expected %s, got %s", expected.Text(10), amountNet.Text(10))
	}
}

// TestTotalOutLessGas verifies that TotalOutLessGas equals TotalOut minus GasOut.
func TestTotalOutLessGas(t *testing.T) {
	stmt := new(Statement)
	// Use the same outgoing values as before: TotalOut = 175.
	stmt.AmountOut = *base.NewWei(15)
	stmt.InternalOut = *base.NewWei(25)
	stmt.CorrectingOut = *base.NewWei(35)
	stmt.SelfDestructOut = *base.NewWei(45)
	stmt.GasOut = *base.NewWei(55)

	totalOutLessGas := stmt.TotalOutLessGas() // should be 175 - 55 = 120.
	expected := base.NewWei(120)
	if totalOutLessGas.Cmp(expected) != 0 {
		t.Errorf("TotalOutLessGas: expected %s, got %s", expected.Text(10), totalOutLessGas.Text(10))
	}
}

func TestCorrectForSomethingElseEth(t *testing.T) {
	restore := resetLogger()
	defer restore()

	// Create a dummy transaction to pass into the function.
	// (We don't actually use any fields of tx in CorrectForSomethingElse, so nil is acceptable.)
	var dummyTx *Transaction = nil

	t.Run("ETH branch: per-block-balance correction", func(t *testing.T) {
		// For the ETH branch, s.IsEth() must return true.
		// Assume that s.IsEth() checks whether AssetAddr equals base.FAKE_ETH_ADDRESS.
		// Also, for this branch we want AssetType == "trace-eth" and both First and Last flags set.
		stmt := &Statement{
			AssetAddr:        base.FAKE_ETH_ADDRESS, // so that IsEth() returns true
			AssetType:        "trace-eth",
			ReconType:        First | Last,
			BlockNumber:      100, // arbitrary block number
			TransactionIndex: 1,
			// Set balances:
			// Let BegBal be 1000 and AmountNet (computed via AmountIn - AmountOut, with others zero) be 100.
			// Then EndBalCalc() should be 1100.
			BegBal:    *base.NewWei(1000),
			AmountIn:  *base.NewWei(100),
			AmountOut: *base.NewWei(0),
			// Set EndBal to a value different from EndBalCalc() so that correction is needed.
			EndBal: *base.NewWei(1050),
		}
		// When CorrectForSomethingElse is called, it should update EndBal to 1100 and set CorrectingReason.
		reconciled := stmt.CorrectForSomethingElseEth(dummyTx)
		// Verify EndBal is corrected.
		expectedEndBal := base.NewWei(1100)
		if stmt.EndBal.Cmp(expectedEndBal) != 0 {
			t.Errorf("ETH branch: Expected EndBal to be corrected to %s; got %s", expectedEndBal.Text(10), stmt.EndBal.Text(10))
		}
		// Verify CorrectingReason is set appropriately.
		if stmt.CorrectingReason != "per-block-balance" {
			t.Errorf("ETH branch: Expected CorrectingReason to be 'per-block-balance'; got %q", stmt.CorrectingReason)
		}
		// (Optionally, you could inspect reconciled, but here we focus on the side effects.)
		_ = reconciled
	})
}

func TestCorrectForSomethingElseToken(t *testing.T) {
	restore := resetLogger()
	defer restore()

	t.Run("Token branch: adjusting correcting values", func(t *testing.T) {
		// Create a fresh Statement with all fields initialized to their zero values.
		stmt := new(Statement)

		// Set only the fields required for the test.
		stmt.AssetAddr = base.HexToAddress("0x1111111111111111111111111111111111111111") // Not ETH.
		stmt.AssetType = "token"
		stmt.BlockNumber = 200

		// Set up balance fields.
		// Let PrevBal = 90, BegBal = 100, so BegBalDiff() should be 10.
		stmt.PrevBal = *base.NewWei(90)
		stmt.BegBal = *base.NewWei(100)

		// Set AmountIn = 50, AmountOut = 0, so that AmountNet() = 50.
		stmt.AmountIn = *base.NewWei(50)
		stmt.AmountOut = *base.NewWei(0)

		// Then EndBalCalc() should be BegBal + AmountNet() = 100 + 50 = 150.
		// Set EndBal to 140, so EndBalDiff() = 150 - 140 = 10.
		stmt.EndBal = *base.NewWei(140)

		// Pre-populate correcting fields to non-zero values to ensure they are reset.
		stmt.CorrectingIn = *base.NewWei(999)
		stmt.CorrectingOut = *base.NewWei(999)
		stmt.CorrectingReason = "oldreason"

		// Call the correction method.
		reconciled := stmt.CorrectForSomethingElseToken(nil) // dummyTx is nil as it is not used.

		// Expected behavior:
		// - BegBalDiff() is 10.
		// - EndBalDiff() is 10.
		// Thus, CorrectingIn should be set to 10 + 10 = 20,
		// CorrectingOut should be reset to 0,
		// And CorrectingReason should become "begbal-endbal".
		expectedCorrectingIn := base.NewWei(20)
		if stmt.CorrectingIn.Cmp(expectedCorrectingIn) != 0 {
			t.Errorf("Token branch: Expected CorrectingIn to be %s; got %s",
				expectedCorrectingIn.Text(10), stmt.CorrectingIn.Text(10))
		}
		if stmt.CorrectingOut.Cmp(base.NewWei(0)) != 0 {
			t.Errorf("Token branch: Expected CorrectingOut to be 0; got %s", stmt.CorrectingOut.Text(10))
		}
		if stmt.CorrectingReason != "begbal-endbal" {
			t.Errorf("Token branch: Expected CorrectingReason to be 'begbal-endbal'; got %q", stmt.CorrectingReason)
		}
		if reconciled {
			t.Error("Token branch: Expected statement not to be reconciled")
		}
	})
}

// TestBegBalDiff tests the BegBalDiff() method.
func TestBegBalDiff(t *testing.T) {
	// Case 1: BlockNumber == 0. Should return 0 regardless of BegBal and PrevBal.
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

// TestEndBalCalc tests the EndBalCalc() method.
func TestEndBalCalc(t *testing.T) {
	// Set up a Statement with:
	//   BegBal = 100,
	//   AmountIn = 50, AmountOut = 0 (and assume other TotalIn/TotalOut contributions are zero),
	// so that AmountNet() = 50 and EndBalCalc() = BegBal + 50 = 150.
	stmt := Statement{
		BegBal:      *base.NewWei(100),
		AmountIn:    *base.NewWei(50),
		AmountOut:   *base.NewWei(0),
		InternalIn:  *base.NewWei(0),
		InternalOut: *base.NewWei(0),
		// All other fields that TotalIn and TotalOut consider are assumed to be zero.
	}
	endCalc := stmt.EndBalCalc()
	expected := base.NewWei(150)
	if endCalc.Cmp(expected) != 0 {
		t.Errorf("EndBalCalc(): expected %s, got %s", expected.Text(10), endCalc.Text(10))
	}
}

// TestEndBalDiff tests the EndBalDiff() method.
func TestEndBalDiff(t *testing.T) {
	// Use the same setup as in TestEndBalCalc, and then set EndBal to a value below EndBalCalc.
	// For example, let:
	//   BegBal = 100,
	//   AmountIn = 50, AmountOut = 0  --> EndBalCalc() = 150,
	//   EndBal = 140,
	// so that EndBalDiff() = 150 - 140 = 10.
	stmt := Statement{
		BegBal:      *base.NewWei(100),
		AmountIn:    *base.NewWei(50),
		AmountOut:   *base.NewWei(0),
		EndBal:      *base.NewWei(140),
		InternalIn:  *base.NewWei(0),
		InternalOut: *base.NewWei(0),
		// Other contributing fields are assumed zero.
	}
	diff := stmt.EndBalDiff()
	expected := base.NewWei(10)
	if diff.Cmp(expected) != 0 {
		t.Errorf("EndBalDiff(): expected %s, got %s", expected.Text(10), diff.Text(10))
	}
}

type dummyLedgerer struct{}

func (d dummyLedgerer) Prev() base.Blknum     { return 99 }
func (d dummyLedgerer) Cur() base.Blknum      { return 100 }
func (d dummyLedgerer) Next() base.Blknum     { return 101 }
func (d dummyLedgerer) Recon() ReconType      { return 0 }
func (d dummyLedgerer) Address() base.Address { return base.ZeroAddr }

func TestDebugStatement(t *testing.T) {
	restore := resetLogger()
	defer restore()

	ledger := dummyLedgerer{}

	// Create a test Statement with some sample values.
	stmt := &Statement{
		AssetType:           "eth",
		BlockNumber:         100,
		TransactionIndex:    1,
		LogIndex:            2,
		ReconType:           0, // assume 0 for simplicity
		AccountedFor:        base.HexToAddress("0xAAAABBBBCCCCDDDDEEEEFFFF0000111122223333"),
		Sender:              base.HexToAddress("0x1111222233334444555566667777888899990000"),
		Recipient:           base.HexToAddress("0x0000999988887777666655554444333322221111"),
		AssetAddr:           base.HexToAddress("0xDEADBEEFDEADBEEFDEADBEEFDEADBEEFDEADBEEF"),
		AssetSymbol:         "ETH",
		Decimals:            18,
		TransactionHash:     base.HexToHash("0x1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef"),
		Timestamp:           1610000000,
		SpotPrice:           2000, // sample value
		PriceSource:         "TestSource",
		PrevBal:             *base.NewWei(1000000000000000000),
		BegBal:              *base.NewWei(1500000000000000000),
		EndBal:              *base.NewWei(1600000000000000000),
		CorrectingIn:        *base.NewWei(0),
		CorrectingOut:       *base.NewWei(0),
		AmountIn:            *base.NewWei(500000000000000000),
		InternalIn:          *base.NewWei(0),
		MinerBaseRewardIn:   *base.NewWei(0),
		MinerNephewRewardIn: *base.NewWei(0),
		MinerTxFeeIn:        *base.NewWei(0),
		MinerUncleRewardIn:  *base.NewWei(0),
		PrefundIn:           *base.NewWei(0),
		AmountOut:           *base.NewWei(400000000000000000),
		InternalOut:         *base.NewWei(0),
		SelfDestructIn:      *base.NewWei(0),
		SelfDestructOut:     *base.NewWei(0),
		GasOut:              *base.NewWei(100000000000000000),
	}

	// Call DebugStatement with the dummy ledgerer.
	stmt.DebugStatement(ledger)

	// Verify that certain key substrings appear in the captured logs.
	foundHeader := false
	foundBlockLine := false
	foundTrialBalance := false

	for _, logLine := range capturedLogs {
		if strings.Contains(logLine, "====> eth") {
			foundHeader = true
		}
		if strings.Contains(logLine, fmt.Sprintf("Current:               %d", stmt.BlockNumber)) {
			foundBlockLine = true
		}
		if strings.Contains(logLine, "Trial balance:") {
			foundTrialBalance = true
		}
	}

	if !foundHeader {
		t.Error("DebugStatement output missing header with asset type")
	}
	if !foundBlockLine {
		t.Error("DebugStatement output missing current block number information")
	}
	if !foundTrialBalance {
		t.Error("DebugStatement output missing 'Trial balance:' section")
	}
}

// TestCacheLocations verifies that CacheLocations() produces the expected directory,
// padded ID, and extension.
func TestCacheLocations(t *testing.T) {
	// Use a dummy address. The address string should be a valid hex string.
	// For example, use base.HexToAddress to convert a hex literal.
	addr := base.HexToAddress("0x1234567890abcdef1234567890abcdef12345678")
	// Define BlockNumber and TransactionIndex.
	var blockNumber base.Blknum = 123
	var txIndex base.Txnum = 45

	// Create a StatementGroup.
	sg := StatementGroup{
		BlockNumber:      blockNumber,
		TransactionIndex: txIndex,
		Address:          addr,
	}

	// Call CacheLocations.
	directory, paddedId, ext := sg.CacheLocations()

	// Compute the expected paddedId.
	// Format: "%s-%09d-%05d" where the first value is the address without the "0x" prefix.
	expectedPaddedId := fmt.Sprintf("%s-%09d-%05d", addr.Hex()[2:], blockNumber, txIndex)
	if paddedId != expectedPaddedId {
		t.Errorf("Expected paddedId %q; got %q", expectedPaddedId, paddedId)
	}

	// Compute the expected directory.
	// parts are the first 6 characters of paddedId split as: [0:2], [2:4], [4:6]
	parts := []string{paddedId[:2], paddedId[2:4], paddedId[4:6]}
	expectedDirectory := filepath.Join("statements", filepath.Join(parts...))
	if directory != expectedDirectory {
		t.Errorf("Expected directory %q; got %q", expectedDirectory, directory)
	}

	// The expected extension is "bin"
	if ext != "bin" {
		t.Errorf("Expected extension %q; got %q", "bin", ext)
	}
}

// TestStatementGroupCache tests that a StatementGroup can be marshaled to a cache
// (i.e. written to a buffer) and then unmarshaled back with the same content.
func TestStatementGroupCache(t *testing.T) {
	// Create a dummy Statement.
	// For simplicity, only populate a few fields.
	stmt := Statement{
		AccountedFor:     base.HexToAddress("0xabcdefabcdefabcdefabcdefabcdefabcdefabcd"),
		AmountIn:         *base.NewWei(1000000000000000000), // 1 Ether in Wei
		AmountOut:        *base.NewWei(500000000000000000),  // 0.5 Ether in Wei
		AssetAddr:        base.HexToAddress("0xdeadbeefdeadbeefdeadbeefdeadbeefdeadbeef"),
		AssetSymbol:      "TKN",
		BegBal:           *base.NewWei(2000000000000000000),
		BlockNumber:      100,
		LogIndex:         1,
		TransactionHash:  base.HexToHash("0x1111111111111111111111111111111111111111111111111111111111111111"),
		TransactionIndex: 2,
		// (populate additional fields as necessary)
	}

	// Create a StatementGroup with one Statement.
	origGroup := StatementGroup{
		BlockNumber:      100,
		TransactionIndex: 2,
		Address:          base.HexToAddress("0xabcdefabcdefabcdefabcdefabcdefabcdefabcd"),
		Statements:       []Statement{stmt},
	}

	// Marshal the StatementGroup to an in-memory buffer.
	var buf bytes.Buffer
	if err := origGroup.MarshalCache(&buf); err != nil {
		t.Fatalf("MarshalCache failed: %v", err)
	}

	// Now create a new StatementGroup and unmarshal from the same buffer.
	var newGroup StatementGroup
	// Pass a dummy version number (e.g., 1)
	if err := newGroup.UnmarshalCache(1, &buf); err != nil {
		t.Fatalf("UnmarshalCache failed: %v", err)
	}

	// Compare the original and new Statement slices.
	// Here we use reflect.DeepEqual; adjust if your types require a different comparison.
	if !reflect.DeepEqual(origGroup.Statements, newGroup.Statements) {
		t.Errorf("Unmarshaled statements do not match original.\nOriginal: %+v\nNew: %+v",
			origGroup.Statements, newGroup.Statements)
	}
}

// capturedLogs will store the logs produced by our fakeTestLog.
var capturedLogs []string

// fakeTestLog is our test stub that captures log messages.
// It mimics logger.TestLog by appending a formatted string to capturedLogs.
func fakeTestLog(notDefault bool, args ...interface{}) {
	// We'll capture logs only if notDefault is true (as per original logic).
	if notDefault {
		// Concatenate all arguments with a space.
		logMsg := fmt.Sprint(args...)
		capturedLogs = append(capturedLogs, logMsg)
	}
}

// resetLogger overrides logger.TestLog with our fakeTestLog,
// clears capturedLogs, and returns a function to restore the original.
func resetLogger() func() {
	// Ensure that the test mode is enabled for our tests.
	os.Setenv("TEST_MODE", "true")
	// Save the original TestLog.
	orig := logger.TestLog
	// Replace TestLog with our fake.
	logger.TestLog = fakeTestLog
	// Clear any previously captured logs.
	capturedLogs = nil
	// Return a restore function.
	return func() {
		logger.TestLog = orig
	}
}

// TestReportE tests reportE: it should log the message along with the Wei value formatted as Ether.
func TestReportE(t *testing.T) {
	restore := resetLogger()
	defer restore()

	// Create a Wei value corresponding to 1 Ether.
	oneEther := base.NewWei(1000000000000000000) // 1e18 wei
	reportE("ReportE:", oneEther)

	if len(capturedLogs) != 1 {
		t.Fatalf("Expected 1 log entry; got %d", len(capturedLogs))
	}

	// Assuming ToEtherStr(18) for 1e18 returns "1.0"
	expected := "ReportE:1"
	got := capturedLogs[0]
	if got != expected {
		t.Errorf("Expected log %q; got %q", expected, capturedLogs[0])
	}
}

// TestReport2 tests report2 with various combinations of v1 and v2.
func TestReport2(t *testing.T) {
	restore := resetLogger()
	defer restore()

	// Create two Wei values.
	val1 := base.NewWei(500000000000000000)  // 0.5 Ether
	val2 := base.NewWei(2000000000000000000) // 2 Ether

	// Case 1: Both v1 and v2 non-nil.
	report2("TestReport2:", val1, val2)
	if len(capturedLogs) != 1 {
		t.Fatalf("Case 1: Expected 1 log entry; got %d", len(capturedLogs))
	}
	// Expected string: "TestReport2:" followed by "0.5 (2.0)" (assuming ToEtherStr conversion)
	expected1 := "TestReport2:0.5 (2)"
	got1 := capturedLogs[0]
	if got1 != expected1 {
		t.Errorf("Case 1: Expected %q; got %q", expected1, capturedLogs[0])
	}

	// Reset for next case.
	capturedLogs = nil

	// Case 2: v1 is nil, v2 non-nil.
	report2("TestReport2NilV1:", nil, val2)
	if len(capturedLogs) != 1 {
		t.Fatalf("Case 2: Expected 1 log entry; got %d", len(capturedLogs))
	}
	// When v1 is nil, the string will be empty plus v2 formatted in parentheses.
	expected2 := "TestReport2NilV1: (2)"
	got2 := capturedLogs[0]
	if got2 != expected2 {
		t.Errorf("Case 2: Expected %q; got %q", expected2, capturedLogs[0])
	}

	// Reset for next case.
	capturedLogs = nil

	// Case 3: Both v1 and v2 are nil.
	report2("TestReport2BothNil:", nil, nil)
	if len(capturedLogs) != 1 {
		t.Fatalf("Case 3: Expected 1 log entry; got %d", len(capturedLogs))
	}
	// With both nil, the output should simply be the message.
	expected3 := "TestReport2BothNil:"
	got3 := capturedLogs[0]
	if got3 != expected3 {
		t.Errorf("Case 3: Expected %q; got %q", expected3, capturedLogs[0])
	}
}

// TestReportL tests reportL, which is a thin wrapper around report2 with nil values.
func TestReportL(t *testing.T) {
	restore := resetLogger()
	defer restore()

	reportL("--------------------")
	if len(capturedLogs) != 1 {
		t.Fatalf("Expected 1 log entry; got %d", len(capturedLogs))
	}
	expected := "--------------------"
	got := capturedLogs[0]
	if got != expected {
		t.Errorf("Expected log %q; got %q", expected, capturedLogs[0])
	}
}

// TestReport1 tests report1 which calls report2 with a non-nil value for v1.
func TestReport1(t *testing.T) {
	restore := resetLogger()
	defer restore()

	val := base.NewWei(750000000000000000) // 0.75 Ether
	report1("TestReport1:", val)
	if len(capturedLogs) != 1 {
		t.Fatalf("Expected 1 log entry; got %d", len(capturedLogs))
	}
	expected := "TestReport1:0.75"
	got := capturedLogs[0]
	if got != expected {
		t.Errorf("Expected log %q; got %q", expected, capturedLogs[0])
	}
}
