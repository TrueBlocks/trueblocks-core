package ledger

import (
	"fmt"
	"os"
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func TestCorrectForNullTransfer(t *testing.T) {
	t.Run("ETH branch", func(t *testing.T) {
		tx := &types.Transaction{
			To: base.HexToAddress("0xAAA"),
		}
		tx.Receipt = new(types.Receipt)
		stmt := new(types.Statement)
		stmt.AssetAddr = base.FAKE_ETH_ADDRESS // This makes it ETH.
		stmt.BegBal = *base.NewWei(1000)
		stmt.EndBal = *base.NewWei(1050)
		stmt.AmountIn = *base.NewWei(100)

		prevAmountIn := &stmt.AmountIn
		prevCorrectingIn := &stmt.CorrectingIn
		var l Ledger
		result := l.correctForNullTransfer(stmt, tx)
		if stmt.AmountIn.Cmp(prevAmountIn) != 0 {
			t.Error("ETH branch: AmountIn should remain unchanged")
		}
		if stmt.CorrectingIn.Cmp(prevCorrectingIn) != 0 {
			t.Error("ETH branch: CorrectingIn should remain unchanged")
		}
		if result != stmt.Reconciled() {
			t.Error("ETH branch: Function should return s.Reconciled()")
		}
	})

	t.Run("Token branch (null transfer detected)", func(t *testing.T) {
		tx := &types.Transaction{
			To: base.HexToAddress("0xBBB"),
		}
		tx.Receipt = new(types.Receipt)
		tx.Receipt.Logs = make([]types.Log, 11)

		stmt := new(types.Statement)
		stmt.AssetAddr = base.HexToAddress("0xCCC")
		stmt.AssetType = types.TrialBalToken
		stmt.Sender = base.HexToAddress("0x0")
		stmt.BegBal = *base.NewWei(100)
		stmt.EndBal = *base.NewWei(100)
		stmt.AmountIn = *base.NewWei(50)
		stmt.InternalIn = *base.NewWei(0)
		stmt.SelfDestructIn = *base.NewWei(0)
		stmt.MinerBaseRewardIn = *base.NewWei(0)
		stmt.MinerNephewRewardIn = *base.NewWei(0)
		stmt.MinerTxFeeIn = *base.NewWei(0)
		stmt.MinerUncleRewardIn = *base.NewWei(0)
		stmt.PrefundIn = *base.NewWei(0)

		stmt.AmountOut = *base.NewWei(123)
		stmt.CorrectingIn = *base.NewWei(456)
		stmt.CorrectingOut = *base.NewWei(789)
		stmt.CorrectingReason = "oldreason"

		originalTotalIn := stmt.TotalIn()

		var l Ledger
		result := l.correctForNullTransfer(stmt, tx)
		if stmt.AmountIn.Cmp(base.NewWei(0)) != 0 {
			t.Errorf("Token branch (null transfer): Expected AmountIn to be 0, got %s", stmt.AmountIn.Text(10))
		}
		if stmt.AmountOut.Cmp(base.NewWei(0)) != 0 {
			t.Errorf("Token branch (null transfer): Expected AmountOut to be 0, got %s", stmt.AmountOut.Text(10))
		}
		if stmt.CorrectingIn.Cmp(originalTotalIn) != 0 {
			t.Errorf("Token branch (null transfer): Expected CorrectingIn to be %s, got %s",
				originalTotalIn.Text(10), stmt.CorrectingIn.Text(10))
		}
		if stmt.CorrectingOut.Cmp(originalTotalIn) != 0 {
			t.Errorf("Token branch (null transfer): Expected CorrectingOut to be %s, got %s",
				originalTotalIn.Text(10), stmt.CorrectingOut.Text(10))
		}
		if stmt.CorrectingReason != "null-transfer" {
			t.Errorf("Token branch (null transfer): Expected CorrectingReason to be 'null-transfer', got %q", stmt.CorrectingReason)
		}
		if result != stmt.Reconciled() {
			t.Error("Token branch (null transfer): Function return should equal s.Reconciled()")
		}
	})

	t.Run("Token branch (null transfer not detected)", func(t *testing.T) {
		tx := &types.Transaction{
			To: base.HexToAddress("0xBBB"),
		}
		tx.Receipt = new(types.Receipt)
		tx.Receipt.Logs = make([]types.Log, 5) // not enough logs for lotsOfLogs

		stmt := new(types.Statement)
		stmt.AssetAddr = base.HexToAddress("0xCCC")
		stmt.AssetType = types.TrialBalToken
		stmt.Sender = base.HexToAddress("0xABC")
		stmt.BegBal = *base.NewWei(100)
		stmt.EndBal = *base.NewWei(100)
		stmt.AmountIn = *base.NewWei(50)

		stmt.AmountOut = *base.NewWei(123)
		stmt.CorrectingIn = *base.NewWei(456)
		stmt.CorrectingOut = *base.NewWei(789)
		stmt.CorrectingReason = "oldreason"

		origAmountIn := &stmt.AmountIn
		origAmountOut := &stmt.AmountOut
		origCorrectingIn := &stmt.CorrectingIn
		origCorrectingOut := &stmt.CorrectingOut
		origReason := stmt.CorrectingReason

		var l Ledger
		result := l.correctForNullTransfer(stmt, tx)
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
			To: base.HexToAddress("0xABC"),
		}
		tx.Receipt = new(types.Receipt)
		tx.Receipt.Logs = make([]types.Log, 11) // >10 logs

		stmt := new(types.Statement)
		stmt.Sender = base.HexToAddress("0x0")
		stmt.BegBal = *base.NewWei(100)
		stmt.EndBal = *base.NewWei(100)
		stmt.AmountIn = *base.NewWei(50)

		if !isNullTransfer(stmt, tx) {
			t.Error("Case 1: Expected isNullTransfer to return true")
		}
	})

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

	t.Run("Case 3: lotsOfLogs true, airdrop false, noBalanceChange true", func(t *testing.T) {
		tx := &types.Transaction{
			To: base.HexToAddress("0xDEF"),
		}
		tx.Receipt = new(types.Receipt)
		tx.Receipt.Logs = make([]types.Log, 11) // >10 logs

		stmt := new(types.Statement)
		stmt.Sender = base.HexToAddress("0xABC")
		stmt.BegBal = *base.NewWei(100)
		stmt.EndBal = *base.NewWei(100)
		stmt.AmountIn = *base.NewWei(50)

		if !isNullTransfer(stmt, tx) {
			t.Error("Case 3: Expected isNullTransfer to return true")
		}
	})

	t.Run("Case 4: lotsOfLogs false, airdrop false, noBalanceChange true", func(t *testing.T) {
		tx := &types.Transaction{
			To: base.HexToAddress("0xDEF"),
		}
		tx.Receipt = new(types.Receipt)
		tx.Receipt.Logs = make([]types.Log, 5) // fewer logs

		stmt := new(types.Statement)
		stmt.Sender = base.HexToAddress("0xABC")
		stmt.BegBal = *base.NewWei(100)
		stmt.EndBal = *base.NewWei(100)
		stmt.AmountIn = *base.NewWei(50)

		if isNullTransfer(stmt, tx) {
			t.Error("Case 4: Expected isNullTransfer to return false")
		}
	})

	t.Run("Case 5: Balance change present", func(t *testing.T) {
		tx := &types.Transaction{
			To: base.HexToAddress("0x0"), // may trigger airdrop condition.
		}
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

	t.Run("Case 6: Not material", func(t *testing.T) {
		tx := &types.Transaction{
			To: base.HexToAddress("0x0"),
		}
		tx.Receipt = new(types.Receipt)
		tx.Receipt.Logs = make([]types.Log, 11)

		stmt := new(types.Statement)
		stmt.Sender = base.HexToAddress("0x0")
		stmt.BegBal = *base.NewWei(100)
		stmt.EndBal = *base.NewWei(100)
		if isNullTransfer(stmt, tx) {
			t.Error("Case 6: Expected isNullTransfer to return false because statement is not material")
		}
	})
}

var capturedLogs []string

func fakeTestLog(notDefault bool, args ...interface{}) {
	if notDefault {
		logMsg := fmt.Sprint(args...)
		capturedLogs = append(capturedLogs, logMsg)
	}
}

func resetLogger() func() {
	os.Setenv("TEST_MODE", "true")
	orig := logger.TestLog
	logger.TestLog = fakeTestLog
	capturedLogs = nil
	return func() {
		logger.TestLog = orig
	}
}

func TestCorrectForSomethingElseEth(t *testing.T) {
	restore := resetLogger()
	defer restore()

	t.Run("ETH branch: per-block-balance correction", func(t *testing.T) {
		stmt := &types.Statement{
			AssetAddr:        base.FAKE_ETH_ADDRESS, // so that IsEth() returns true
			AssetType:        types.TrialBalTraceEth,
			ReconType:        types.First | types.Last,
			BlockNumber:      100, // arbitrary block number
			TransactionIndex: 1,
			BegBal:           *base.NewWei(1000),
			AmountIn:         *base.NewWei(100),
			AmountOut:        *base.NewWei(0),
			EndBal:           *base.NewWei(1050),
		}
		var l Ledger
		reconciled := l.correctForSomethingElseEth(stmt)
		expectedEndBal := base.NewWei(1100)
		if stmt.EndBal.Cmp(expectedEndBal) != 0 {
			t.Errorf("ETH branch: Expected EndBal to be corrected to %s; got %s", expectedEndBal.Text(10), stmt.EndBal.Text(10))
		}
		if stmt.CorrectingReason != "per-block-balance" {
			t.Errorf("ETH branch: Expected CorrectingReason to be 'per-block-balance'; got %q", stmt.CorrectingReason)
		}
		_ = reconciled
	})
}

// func TestCorrectForSomethingElseToken(t *testing.T) {
// 	restore := resetLogger()
// 	defer restore()

// 	t.Run("Token branch: adjusting correcting values", func(t *testing.T) {
// 		stmt := new(types.Statement)

// 		stmt.AssetAddr = base.HexToAddress("0x1111111111111111111111111111111111111111") // Not ETH.
// 		stmt.AssetType = types.TrialBalToken
// 		stmt.BlockNumber = 200

// 		stmt.PrevBal = *base.NewWei(90)
// 		stmt.BegBal = *base.NewWei(100)

// 		stmt.AmountIn = *base.NewWei(50)
// 		stmt.AmountOut = *base.NewWei(0)

// 		stmt.EndBal = *base.NewWei(140)

// 		stmt.CorrectingIn = *base.NewWei(999)
// 		stmt.CorrectingOut = *base.NewWei(999)
// 		stmt.CorrectingReason = "oldreason"

// 		var l Ledger
// 		reconciled := l.correctForSomethingElseToken(stmt)

// 		expectedCorrectingIn := base.NewWei(20)
// 		if stmt.CorrectingIn.Cmp(expectedCorrectingIn) != 0 {
// 			t.Errorf("Token branch: Expected CorrectingIn to be %s; got %s",
// 				expectedCorrectingIn.Text(10), stmt.CorrectingIn.Text(10))
// 		}
// 		if stmt.CorrectingOut.Cmp(base.NewWei(0)) != 0 {
// 			t.Errorf("Token branch: Expected CorrectingOut to be 0; got %s", stmt.CorrectingOut.Text(10))
// 		}
// 		if stmt.CorrectingReason != "begbal-endbal" {
// 			t.Errorf("Token branch: Expected CorrectingReason to be 'begbal-endbal'; got %q", stmt.CorrectingReason)
// 		}
// 		if reconciled {
// 			t.Error("Token branch: Expected statement not to be reconciled")
// 		}
// 	})
// }

// func TestTrialBalance_Reconciled(t *testing.T) {
// 	l := &Ledger{
// 		testMode:       true,
// 	}
// 	l.theTx = &types.Transaction{
// 		Receipt: new(types.Receipt),
// 		To:      base.HexToAddress("0xDummy"),
// 	}
// 	l.theTx.Receipt.Logs = []types.Log{}

// 	bn := base.Blknum(100)
// 	txid := base.Txnum(1)
// 	assetAddr := base.HexToAddress("0xeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee")
// 	appBal := newAppBalancer(bn-1, bn, bn+1, false, false, l.reversed)
// 	ac := new AppBalancer(appBal.Prev(), appBal.Cur(), appBal.Next(), false, false, l.reversed, assetAddr)
// 	l.appBalancers[assetAddr] = ac

// 	s := new(types.Statement)
// 	s.BlockNumber = bn
// 	s.TransactionIndex = txid
// 	s.AssetAddr = assetAddr
// 	s.PrevBal = *base.NewWei(100)
// 	s.BegBal = *base.NewWei(100)
// 	s.EndBal = *base.NewWei(100)
// 	s.AmountIn = *base.NewWei(10)
// 	s.AmountOut = *base.NewWei(10)

// 	result := l.trialBalance(types.TrialBalEth, s)
// 	if !result {
// 		t.Error("Expected trialBalance to return true for a reconciled statement")
// 	}
// 	if s.AssetType != string(types.TrialBalEth) {
// 		t.Errorf("Expected AssetType to be %s, got %s", types.TrialBalEth, s.AssetType)
// 	}
// 	if s.ReconType != ac.Recon() {
// 		t.Errorf("Expected ReconType to be %v, got %v", ac.Recon(), s.ReconType)
// 	}
// }
