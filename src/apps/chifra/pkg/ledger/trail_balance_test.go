package ledger

import (
	"fmt"
	"os"
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// TestCorrectForNullTransfer tests the correctForNullTransfer function.
func TestCorrectForNullTransfer(t *testing.T) {
	// --- ETH branch ---
	t.Run("ETH branch", func(t *testing.T) {
		// Create a Transaction with any minimal required fields.
		tx := &types.Transaction{
			To: base.HexToAddress("0xAAA"),
		}
		tx.Receipt = new(types.Receipt)
		// (For ETH branch the logs and other fields are irrelevant.)

		// Create a statement that is considered ETH.
		// s.IsEth() returns true when AssetAddr equals base.FAKE_ETH_ADDRESS.
		stmt := new(types.Statement)
		stmt.AssetAddr = base.FAKE_ETH_ADDRESS // This makes it ETH.
		// Set balances to any values; they won't be modified by correctForNullTransfer.
		stmt.BegBal = *base.NewWei(1000)
		stmt.EndBal = *base.NewWei(1050)
		// Set a nonzero amount so that the statement is material.
		stmt.AmountIn = *base.NewWei(100)

		// Call correctForNullTransfer; for ETH, it should simply return s.Reconciled().
		// Note: It will not modify the token fields.
		prevAmountIn := &stmt.AmountIn
		prevCorrectingIn := &stmt.CorrectingIn
		var l Ledger
		result := l.correctForNullTransfer(stmt, tx)
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
		tx := &types.Transaction{
			To: base.HexToAddress("0xBBB"),
		}
		tx.Receipt = new(types.Receipt)
		// Set Logs to a slice of 11 Logs to trigger lotsOfLogs.
		tx.Receipt.Logs = make([]types.Log, 11)

		// Create a token statement (non-ETH).
		stmt := new(types.Statement)
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

		// Call correctForNullTransfer.
		var l Ledger
		result := l.correctForNullTransfer(stmt, tx)
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
		tx := &types.Transaction{
			To: base.HexToAddress("0xBBB"),
		}
		tx.Receipt = new(types.Receipt)
		tx.Receipt.Logs = make([]types.Log, 5) // not enough logs for lotsOfLogs

		// Create a token statement.
		stmt := new(types.Statement)
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

		// Call correctForNullTransfer.
		var l Ledger
		result := l.correctForNullTransfer(stmt, tx)
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
	// 	// Case 1: lotsOfLogs true, airdrop true, noBalanceChange true.
	t.Run("Case 1: lotsOfLogs true, airdrop true, noBalanceChange true", func(t *testing.T) {
		tx := &types.Transaction{
			To: base.HexToAddress("0xABC"), // arbitrary nonzero address
		}
		// Initialize Receipt and Logs.
		tx.Receipt = new(types.Receipt)
		tx.Receipt.Logs = make([]types.Log, 11) // >10 logs

		stmt := new(types.Statement)
		// Set Sender to zero address to trigger airdrop condition.
		stmt.Sender = base.HexToAddress("0x0")
		// Ensure no balance change: BegBal equals EndBal.
		stmt.BegBal = *base.NewWei(100)
		stmt.EndBal = *base.NewWei(100)
		// Make the statement material by setting a nonzero incoming amount.
		stmt.AmountIn = *base.NewWei(50)

		if !isNullTransfer(stmt, tx) {
			t.Error("Case 1: Expected isNullTransfer to return true")
		}
	})

	// Case 2: lotsOfLogs false, airdrop true, noBalanceChange true.
	t.Run("Case 2: lotsOfLogs false, airdrop true, noBalanceChange true", func(t *testing.T) {
		tx := &types.Transaction{
			To: base.HexToAddress("0xABC"),
		}
		// Initialize Receipt and Logs.
		tx.Receipt = new(types.Receipt)
		tx.Receipt.Logs = make([]types.Log, 5) // ≤10 logs

		stmt := new(types.Statement)
		stmt.Sender = base.HexToAddress("0x0") // triggers airdrop condition.
		stmt.BegBal = *base.NewWei(100)
		stmt.EndBal = *base.NewWei(100)
		stmt.AmountIn = *base.NewWei(50)

		if !isNullTransfer(stmt, tx) {
			t.Error("Case 2: Expected isNullTransfer to return true")
		}
	})

	// Case 3: lotsOfLogs true, airdrop false, noBalanceChange true.
	t.Run("Case 3: lotsOfLogs true, airdrop false, noBalanceChange true", func(t *testing.T) {
		tx := &types.Transaction{
			To: base.HexToAddress("0xDEF"), // Different from Sender.
		}
		// Initialize Receipt and Logs.
		tx.Receipt = new(types.Receipt)
		tx.Receipt.Logs = make([]types.Log, 11) // >10 logs

		stmt := new(types.Statement)
		// Set Sender to a nonzero address different from tx.To.
		stmt.Sender = base.HexToAddress("0xABC")
		stmt.BegBal = *base.NewWei(100)
		stmt.EndBal = *base.NewWei(100)
		stmt.AmountIn = *base.NewWei(50)

		if !isNullTransfer(stmt, tx) {
			t.Error("Case 3: Expected isNullTransfer to return true")
		}
	})

	// Case 4: lotsOfLogs false, airdrop false, noBalanceChange true.
	t.Run("Case 4: lotsOfLogs false, airdrop false, noBalanceChange true", func(t *testing.T) {
		tx := &types.Transaction{
			To: base.HexToAddress("0xDEF"),
		}
		// Initialize Receipt and Logs.
		tx.Receipt = new(types.Receipt)
		tx.Receipt.Logs = make([]types.Log, 5) // fewer logs

		stmt := new(types.Statement)
		// Set Sender to a nonzero address different from tx.To.
		stmt.Sender = base.HexToAddress("0xABC")
		stmt.BegBal = *base.NewWei(100)
		stmt.EndBal = *base.NewWei(100)
		stmt.AmountIn = *base.NewWei(50)

		if isNullTransfer(stmt, tx) {
			t.Error("Case 4: Expected isNullTransfer to return false")
		}
	})

	// Case 5: Conditions met for logs/airdrop, but balance changes (noBalanceChange false).
	t.Run("Case 5: Balance change present", func(t *testing.T) {
		tx := &types.Transaction{
			To: base.HexToAddress("0x0"), // may trigger airdrop condition.
		}
		// Initialize Receipt and Logs.
		tx.Receipt = new(types.Receipt)
		tx.Receipt.Logs = make([]types.Log, 11)

		stmt := new(types.Statement)
		stmt.Sender = base.HexToAddress("0x0")
		stmt.BegBal = *base.NewWei(100)
		stmt.EndBal = *base.NewWei(110) // Balance changes: difference of 10.
		stmt.AmountIn = *base.NewWei(50)

		if isNullTransfer(stmt, tx) {
			t.Error("Case 5: Expected isNullTransfer to return false due to balance change")
		}
	})

	// Case 6: Conditions met for logs/airdrop, but the statement is not material.
	t.Run("Case 6: Not material", func(t *testing.T) {
		tx := &types.Transaction{
			To: base.HexToAddress("0x0"),
		}
		// Initialize Receipt and Logs.
		tx.Receipt = new(types.Receipt)
		tx.Receipt.Logs = make([]types.Log, 11)

		stmt := new(types.Statement)
		stmt.Sender = base.HexToAddress("0x0") // zero address triggers airdrop condition.
		stmt.BegBal = *base.NewWei(100)
		stmt.EndBal = *base.NewWei(100)
		// No AmountIn set, so the statement is not material.
		if isNullTransfer(stmt, tx) {
			t.Error("Case 6: Expected isNullTransfer to return false because statement is not material")
		}
	})
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

func TestCorrectForSomethingElseEth(t *testing.T) {
	restore := resetLogger()
	defer restore()

	t.Run("ETH branch: per-block-balance correction", func(t *testing.T) {
		// For the ETH branch, s.IsEth() must return true.
		// Assume that s.IsEth() checks whether AssetAddr equals base.FAKE_ETH_ADDRESS.
		// Also, for this branch we want AssetType == "trace-eth" and both First and Last flags set.
		stmt := &types.Statement{
			AssetAddr:        base.FAKE_ETH_ADDRESS, // so that IsEth() returns true
			AssetType:        "trace-eth",
			ReconType:        types.First | types.Last,
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
		var l Ledger
		reconciled := l.correctForSomethingElseEth(stmt)
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
		stmt := new(types.Statement)

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
		var l Ledger
		reconciled := l.correctForSomethingElseToken(stmt)

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

func TestTrialBalance_Reconciled(t *testing.T) {
	l := &Ledger{
		testMode:      true,
		assetContexts: make(map[assetContextKey]*assetContext),
	}
	l.theTx = &types.Transaction{
		Receipt: new(types.Receipt),
		To:      base.HexToAddress("0xDummy"),
	}
	l.theTx.Receipt.Logs = []types.Log{}

	bn := base.Blknum(100)
	txid := base.Txnum(1)
	assetAddr := base.HexToAddress("0xeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee")
	appCtx := newAppContext(bn-1, bn, bn+1, false, false, l.reversed)
	ac := newAssetContext(appCtx.Prev(), appCtx.Cur(), appCtx.Next(), false, false, l.reversed, assetAddr)
	key := l.getAssetContextKey(bn, txid, assetAddr)
	l.assetContexts[key] = ac

	s := new(types.Statement)
	s.BlockNumber = bn
	s.TransactionIndex = txid
	s.AssetAddr = assetAddr
	s.PrevBal = *base.NewWei(100)
	s.BegBal = *base.NewWei(100)
	s.EndBal = *base.NewWei(100)
	s.AmountIn = *base.NewWei(10)
	s.AmountOut = *base.NewWei(10)

	result := l.trialBalance(trialBalEth, s)
	if !result {
		t.Error("Expected trialBalance to return true for a reconciled statement")
	}
	if s.AssetType != string(trialBalEth) {
		t.Errorf("Expected AssetType to be %s, got %s", trialBalEth, s.AssetType)
	}
	if s.ReconType != ac.Recon() {
		t.Errorf("Expected ReconType to be %v, got %v", ac.Recon(), s.ReconType)
	}
}
