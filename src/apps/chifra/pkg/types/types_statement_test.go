package types

import (
	"bytes"
	"fmt"
	"path/filepath"
	"reflect"
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
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
	stmtStable.AssetAddr = dai // using DAI as an example
	if !stmtStable.IsStableCoin() {
		t.Errorf("Expected IsStableCoin to return true for address %s", dai.Hex())
	}

	stmtNonStable := new(Statement)
	stmtNonStable.AssetAddr = base.HexToAddress("0x1234567890abcdef1234567890abcdef12345678")
	if stmtNonStable.IsStableCoin() {
		t.Errorf("Expected IsStableCoin to return false for address %s", stmtNonStable.AssetAddr.Hex())
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
		AssetAddr: base.FAKE_ETH_ADDRESS,
	}
	if !ethStmt.IsEth() {
		t.Error("Expected statement to be ETH")
	}

	nonEthStmt := &Statement{
		AssetAddr: base.HexToAddress("0x1234567890abcdef1234567890abcdef12345678"),
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
	stmt.CorrectingIn = *base.NewWei(80)
	stmt.PrefundIn = *base.NewWei(90)

	totalIn := stmt.TotalIn()
	expected := base.NewWei(450)
	if totalIn.Cmp(expected) != 0 {
		t.Errorf("TotalIn: expected %s, got %s", expected.Text(10), totalIn.Text(10))
	}
}

func TestTotalOut(t *testing.T) {
	stmt := new(Statement)
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

func TestAmountNet(t *testing.T) {
	stmt := new(Statement)
	stmt.AmountIn = *base.NewWei(10)
	stmt.InternalIn = *base.NewWei(20)
	stmt.SelfDestructIn = *base.NewWei(30)
	stmt.MinerBaseRewardIn = *base.NewWei(40)
	stmt.MinerNephewRewardIn = *base.NewWei(50)
	stmt.MinerTxFeeIn = *base.NewWei(60)
	stmt.MinerUncleRewardIn = *base.NewWei(70)
	stmt.CorrectingIn = *base.NewWei(80)
	stmt.PrefundIn = *base.NewWei(90)
	stmt.AmountOut = *base.NewWei(15)
	stmt.InternalOut = *base.NewWei(25)
	stmt.CorrectingOut = *base.NewWei(35)
	stmt.SelfDestructOut = *base.NewWei(45)
	stmt.GasOut = *base.NewWei(55)

	amountNet := stmt.AmountNet()
	expected := base.NewWei(275)
	if amountNet.Cmp(expected) != 0 {
		t.Errorf("AmountNet: expected %s, got %s", expected.Text(10), amountNet.Text(10))
	}
}

func TestTotalOutLessGas(t *testing.T) {
	stmt := new(Statement)
	stmt.AmountOut = *base.NewWei(15)
	stmt.InternalOut = *base.NewWei(25)
	stmt.CorrectingOut = *base.NewWei(35)
	stmt.SelfDestructOut = *base.NewWei(45)
	stmt.GasOut = *base.NewWei(55)

	totalOutLessGas := stmt.TotalOutLessGas()
	expected := base.NewWei(120)
	if totalOutLessGas.Cmp(expected) != 0 {
		t.Errorf("TotalOutLessGas: expected %s, got %s", expected.Text(10), totalOutLessGas.Text(10))
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

func TestCacheLocations(t *testing.T) {
	addr := base.HexToAddress("0x1234567890abcdef1234567890abcdef12345678")
	var blockNumber base.Blknum = 123
	var txIndex base.Txnum = 45

	sg := StatementGroup{
		BlockNumber:      blockNumber,
		TransactionIndex: txIndex,
		Address:          addr,
	}

	directory, paddedId, ext := sg.CacheLocations()

	expectedPaddedId := fmt.Sprintf("%s-%09d-%05d", addr.Hex()[2:], blockNumber, txIndex)
	if paddedId != expectedPaddedId {
		t.Errorf("Expected paddedId %q; got %q", expectedPaddedId, paddedId)
	}

	parts := []string{paddedId[:2], paddedId[2:4], paddedId[4:6]}
	expectedDirectory := filepath.Join("statements", filepath.Join(parts...))
	if directory != expectedDirectory {
		t.Errorf("Expected directory %q; got %q", expectedDirectory, directory)
	}

	if ext != "bin" {
		t.Errorf("Expected extension %q; got %q", "bin", ext)
	}
}

func TestStatementGroupCache(t *testing.T) {
	stmt := Statement{
		AccountedFor:     base.HexToAddress("0xabcdefabcdefabcdefabcdefabcdefabcdefabcd"),
		AmountIn:         *base.NewWei(1000000000000000000),
		AmountOut:        *base.NewWei(500000000000000000),
		AssetAddr:        base.HexToAddress("0xdeadbeefdeadbeefdeadbeefdeadbeefdeadbeef"),
		AssetSymbol:      "TKN",
		BegBal:           *base.NewWei(2000000000000000000),
		BlockNumber:      100,
		LogIndex:         1,
		TransactionHash:  base.HexToHash("0x1111111111111111111111111111111111111111111111111111111111111111"),
		TransactionIndex: 2,
	}

	origGroup := StatementGroup{
		BlockNumber:      100,
		TransactionIndex: 2,
		Address:          base.HexToAddress("0xabcdefabcdefabcdefabcdefabcdefabcdefabcd"),
		Statements:       []Statement{stmt},
	}

	var buf bytes.Buffer
	if err := origGroup.MarshalCache(&buf); err != nil {
		t.Fatalf("MarshalCache failed: %v", err)
	}

	var newGroup StatementGroup
	if err := newGroup.UnmarshalCache(1, &buf); err != nil {
		t.Fatalf("UnmarshalCache failed: %v", err)
	}

	if !reflect.DeepEqual(origGroup.Statements, newGroup.Statements) {
		t.Errorf("Unmarshaled statements do not match original.\nOriginal: %+v\nNew: %+v",
			origGroup.Statements, newGroup.Statements)
	}
}

// var capturedLogs []string

// func fakeTestLog(notDefault bool, args ...interface{}) {
// 	if notDefault {
// 		logMsg := fmt.Sprint(args...)
// 		capturedLogs = append(capturedLogs, logMsg)
// 	}
// }

// func resetLogger() func() {
// 	os.Setenv("TEST_MODE", "true")
// 	orig := logger.TestLog
// 	logger.TestLog = fakeTestLog
// 	capturedLogs = nil
// 	return func() {
// 		logger.TestLog = orig
// 	}
// }

// ---------------------------------------------------------------------------
// Helper types for simulating errors during cache marshalling/unmarshalling

type failingWriter struct{}

func (fw *failingWriter) Write(p []byte) (n int, err error) {
	return 0, fmt.Errorf("write error")
}

type failingReader struct{}

func (fr *failingReader) Read(p []byte) (n int, err error) {
	return 0, fmt.Errorf("read error")
}

// ---------------------------------------------------------------------------
// 1. Test error handling in MarshalCache by simulating a writer error

func TestMarshalCacheError(t *testing.T) {
	stmt := &Statement{
		// (We don’t need to populate all fields because the failing writer
		//  will return an error on the very first write.)
	}
	fw := &failingWriter{}
	err := stmt.MarshalCache(fw)
	if err == nil {
		t.Error("Expected MarshalCache to return an error, but got nil")
	} else if err.Error() != "write error" {
		t.Errorf("Expected error 'write error', got %v", err)
	}
}

// ---------------------------------------------------------------------------
// 2. Test error handling in UnmarshalCache by simulating a reader error

func TestUnmarshalCacheError(t *testing.T) {
	stmt := &Statement{}
	fr := &failingReader{}
	err := stmt.UnmarshalCache(1, fr)
	if err == nil {
		t.Error("Expected UnmarshalCache to return an error, but got nil")
	} else if err.Error() != "read error" {
		t.Errorf("Expected error 'read error', got %v", err)
	}
}

// ---------------------------------------------------------------------------
// 3. Test a single Statement’s cache roundtrip (marshal then unmarshal)

func TestStatementCacheRoundtrip(t *testing.T) {
	// Populate a statement with representative (nonzero) values.
	stmt := &Statement{
		AccountedFor:        base.HexToAddress("0xAAAABBBBCCCCDDDDEEEEFFFF0000111122223333"),
		AmountIn:            *base.NewWei(10),
		AmountOut:           *base.NewWei(20),
		AssetAddr:           base.HexToAddress("0x1111222233334444555566667777888899990000"),
		AssetSymbol:         "TKN",
		AssetType:           TrialBalEth, // using an example type; it could be any valid value
		BegBal:              *base.NewWei(100),
		BlockNumber:         123,
		CorrectingIn:        *base.NewWei(5),
		CorrectingOut:       *base.NewWei(3),
		CorrectingReason:    "Test Correction",
		Decimals:            18,
		EndBal:              *base.NewWei(122),
		GasOut:              *base.NewWei(2),
		InternalIn:          *base.NewWei(7),
		InternalOut:         *base.NewWei(4),
		LogIndex:            1,
		MinerBaseRewardIn:   *base.NewWei(8),
		MinerNephewRewardIn: *base.NewWei(9),
		MinerTxFeeIn:        *base.NewWei(10),
		MinerUncleRewardIn:  *base.NewWei(11),
		PrefundIn:           *base.NewWei(12),
		PrevBal:             *base.NewWei(90),
		PriceSource:         "TestSource",
		Recipient:           base.HexToAddress("0x222233334444555566667777888899990000AAAA"),
		ReconType:           1, // arbitrary test value
		RollingBalance:      *base.NewWei(150),
		SelfDestructIn:      *base.NewWei(13),
		SelfDestructOut:     *base.NewWei(14),
		Sender:              base.HexToAddress("0x3333444455556666777788889999AAAABBBBCCCC"),
		SpotPrice:           100,
		Timestamp:           1610000000,
		TransactionHash:     base.HexToHash("0x1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef"),
		TransactionIndex:    2,
	}

	var buf bytes.Buffer
	if err := stmt.MarshalCache(&buf); err != nil {
		t.Fatalf("MarshalCache failed: %v", err)
	}

	newStmt := &Statement{}
	if err := newStmt.UnmarshalCache(1, &buf); err != nil {
		t.Fatalf("UnmarshalCache failed: %v", err)
	}

	// Compare the two statements.
	if !reflect.DeepEqual(stmt, newStmt) {
		t.Errorf("Roundtrip statement does not match original.\nOriginal: %+v\nNew: %+v", stmt, newStmt)
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
		CorrectingIn:        *huge,
		PrefundIn:           *huge,
		// For TotalOut, there are 5 fields:
		AmountOut:       *huge,
		InternalOut:     *huge,
		CorrectingOut:   *huge,
		SelfDestructOut: *huge,
		GasOut:          *huge,
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

	// TotalOutLessGas should be TotalOut - GasOut = 4e18.
	totalOutLessGas := stmt.TotalOutLessGas()
	expectedTotalOutLessGas := base.NewWei(4 * 1000000000000000000)
	if totalOutLessGas.Cmp(expectedTotalOutLessGas) != 0 {
		t.Errorf("TotalOutLessGas with huge values: expected %s, got %s", expectedTotalOutLessGas.Text(10), totalOutLessGas.Text(10))
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
