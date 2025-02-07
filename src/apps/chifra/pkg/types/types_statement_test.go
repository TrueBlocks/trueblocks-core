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
	// TODO: BOGUS NOT DONE
	// stmt.BlockNumber = 1
	// stmt.PrevBal = *base.NewWei(90)
	// stmt.BegBal = *base.NewWei(100)
	// diff = stmt.BegBalDiff()
	// expected = base.NewWei(10)
	// if diff.Cmp(expected) != 0 {
	// 	t.Errorf("BegBalDiff() with BlockNumber!=0: expected %s, got %s", expected.Text(10), diff.Text(10))
	// }
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

	stmt := &Statement{
		AssetType:           TrialBalEth,
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

	stmt.DebugStatement(ledger)

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

func TestReportE(t *testing.T) {
	restore := resetLogger()
	defer restore()

	oneEther := base.NewWei(1000000000000000000) // 1e18 wei
	reportE("ReportE:", oneEther)

	if len(capturedLogs) != 1 {
		t.Fatalf("Expected 1 log entry; got %d", len(capturedLogs))
	}

	expected := "ReportE:1"
	got := capturedLogs[0]
	if got != expected {
		t.Errorf("Expected log %q; got %q", expected, capturedLogs[0])
	}
}

func TestReport2(t *testing.T) {
	restore := resetLogger()
	defer restore()

	val1 := base.NewWei(500000000000000000)
	val2 := base.NewWei(2000000000000000000)

	report2("TestReport2:", val1, val2)
	if len(capturedLogs) != 1 {
		t.Fatalf("Case 1: Expected 1 log entry; got %d", len(capturedLogs))
	}
	expected1 := "TestReport2:0.5 (2)"
	got1 := capturedLogs[0]
	if got1 != expected1 {
		t.Errorf("Case 1: Expected %q; got %q", expected1, capturedLogs[0])
	}

	capturedLogs = nil

	report2("TestReport2NilV1:", nil, val2)
	if len(capturedLogs) != 1 {
		t.Fatalf("Case 2: Expected 1 log entry; got %d", len(capturedLogs))
	}
	expected2 := "TestReport2NilV1: (2)"
	got2 := capturedLogs[0]
	if got2 != expected2 {
		t.Errorf("Case 2: Expected %q; got %q", expected2, capturedLogs[0])
	}

	capturedLogs = nil

	report2("TestReport2BothNil:", nil, nil)
	if len(capturedLogs) != 1 {
		t.Fatalf("Case 3: Expected 1 log entry; got %d", len(capturedLogs))
	}
	expected3 := "TestReport2BothNil:"
	got3 := capturedLogs[0]
	if got3 != expected3 {
		t.Errorf("Case 3: Expected %q; got %q", expected3, capturedLogs[0])
	}
}

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
