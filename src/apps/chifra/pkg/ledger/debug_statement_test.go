package ledger

import (
	"fmt"
	"strings"
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func TestDebugStatement(t *testing.T) {
	restore := resetLogger()
	defer restore()

	ctx := appBalancer{
		prvBlk:   99,
		curBlk:   100,
		nxtBlk:   101,
		postType: 0,
		address:  base.ZeroAddr,
	}

	stmt := &types.Statement{
		AssetType:           types.TrialBalEth,
		BlockNumber:         100,
		TransactionIndex:    1,
		LogIndex:            2,
		AccountedFor:        base.HexToAddress("0xAAAABBBBCCCCDDDDEEEEFFFF0000111122223333"),
		Sender:              base.HexToAddress("0x1111222233334444555566667777888899990000"),
		Recipient:           base.HexToAddress("0x0000999988887777666655554444333322221111"),
		AssetAddress:        base.HexToAddress("0xDEADBEEFDEADBEEFDEADBEEFDEADBEEFDEADBEEF"),
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

	stmt.DebugStatement(ctx.Prev(), ctx.Next())

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

// ---------------------------------------------------------------------------
// 4. Test that DebugStatement prints block number in the expected format
//    when the asset type is for a token (or NFT).

func TestDebugStatementTokenFormatting(t *testing.T) {
	// Reset the logger to capture logs.
	restore := resetLogger()
	defer restore()

	// Create a statement with asset type set to a token type.
	stmt := &types.Statement{
		AssetType:        types.TrialBalToken, // should trigger printing with 3-part block number
		BlockNumber:      123,
		TransactionIndex: 456,
		LogIndex:         789,
		AccountedFor:     base.HexToAddress("0xAAAABBBBCCCCDDDDEEEEFFFF0000111122223333"),
		Sender:           base.HexToAddress("0x1111222233334444555566667777888899990000"),
		Recipient:        base.HexToAddress("0x0000999988887777666655554444333322221111"),
		AssetAddress:     base.HexToAddress("0xDEADBEEFDEADBEEFDEADBEEFDEADBEEFDEADBEEF"),
		AssetSymbol:      "TKN",
		Decimals:         18,
		TransactionHash:  base.HexToHash("0x1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef"),
		Timestamp:        1610000000,
		SpotPrice:        2000,
		PriceSource:      "TestSource",
	}

	ctx := appBalancer{
		prvBlk:   99,
		curBlk:   100,
		nxtBlk:   101,
		postType: 0,
		address:  base.ZeroAddr,
	}
	stmt.DebugStatement(ctx.Prev(), ctx.Next())

	// Look for a log line that shows the three-part block number (e.g., "123.456.789")
	found := false
	for _, logLine := range capturedLogs {
		if strings.Contains(logLine, "blockNumber:") && strings.Contains(logLine, "123.456.789") {
			found = true
			break
		}
	}
	if !found {
		t.Error("DebugStatement did not format blockNumber as expected for token asset type")
	}
}
