package ledger2

import (
	"fmt"
	"testing"

	"github.com/stretchr/testify/assert"

	// Adjust to your actual import paths
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// createTestLedger builds a Ledger from 'in' and 'out' amounts. Each index
// becomes one Posting in one LedgerEntry for test convenience.
func createTestLedger(
	assetAddress base.Address,
	inAmounts, outAmounts []int64,
) Ledger {
	var entries []LedgerEntry
	maxLen := len(inAmounts)
	if len(outAmounts) > maxLen {
		maxLen = len(outAmounts)
	}

	for i := 0; i < maxLen; i++ {
		posting := types.Posting{}
		if i < len(inAmounts) {
			posting.AmountIn = *base.NewWei(inAmounts[i])
		}
		if i < len(outAmounts) {
			posting.AmountOut = *base.NewWei(outAmounts[i])
		}
		entry := LedgerEntry{
			Postings: []types.Posting{posting},
		}
		entries = append(entries, entry)
	}

	return Ledger{
		AssetAddress: assetAddress,
		Entries:      entries,
	}
}

func TestNewLedgerBook(t *testing.T) {
	accountedFor := base.HexToAddress("0xaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa")
	lb := NewLedgerBook(accountedFor)

	assert.Equal(t, accountedFor, lb.AccountedFor, "LedgerBook's AccountedFor should match")
	assert.NotNil(t, lb.Ledgers, "Ledgers map must be initialized")
	assert.Equal(t, 0, len(lb.Ledgers), "Expected an empty ledger map initially")
}

func TestLedgerBookString(t *testing.T) {
	accountedFor := base.HexToAddress("0xbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb")
	lb := NewLedgerBook(accountedFor)

	// Two ledgers
	assetAddr1 := base.HexToAddress("0x1111111111111111111111111111111111111111")
	assetAddr2 := base.HexToAddress("0x2222222222222222222222222222222222222222")

	l1 := createTestLedger(assetAddr1,
		[]int64{100, 200}, // In
		[]int64{50, 30},   // Out
	)
	l2 := createTestLedger(assetAddr2,
		[]int64{10},
		[]int64{5},
	)
	lb.Ledgers[assetAddr1.Hex()] = l1
	lb.Ledgers[assetAddr2.Hex()] = l2

	// totalIn = (100+200) + 10 = 310
	// totalOut = (50+30) + 5 = 85
	// net = 310 - 85 = 225
	str := lb.String()
	expectedSubstr := fmt.Sprintf("%s 2 310 85 225", accountedFor.Hex())
	assert.Contains(t, str, expectedSubstr, "String() should show address, ledger count, totals, net.")
}

func TestLedgerBookGetLedger(t *testing.T) {
	lb := NewLedgerBook(base.HexToAddress("0xcccccccccccccccccccccccccccccccccccccccc"))

	asset1 := base.HexToAddress("0x3333333333333333333333333333333333333333")
	asset2 := base.HexToAddress("0x4444444444444444444444444444444444444444")

	ledger1 := createTestLedger(asset1, []int64{100}, []int64{20})
	lb.Ledgers[asset1.Hex()] = ledger1

	// Retrieve existing ledger
	gotten, ok := lb.GetLedger(asset1)
	assert.True(t, ok, "Should find ledger in map")
	assert.Equal(t, ledger1, gotten, "Should match the stored ledger")

	// Non-existing
	_, ok = lb.GetLedger(asset2)
	assert.False(t, ok, "Should not find nonexistent ledger")
}

func TestLedgerBookTotalIn(t *testing.T) {
	lb := NewLedgerBook(base.HexToAddress("0xdddddddddddddddddddddddddddddddddddddddd"))

	// No ledgers => totalIn = 0
	totalIn := lb.TotalIn()
	assert.True(t, totalIn.Equal(base.NewWei(0)), "Expected zero TotalIn with no ledgers")

	assetAddr1 := base.HexToAddress("0x5555555555555555555555555555555555555555")
	assetAddr2 := base.HexToAddress("0x6666666666666666666666666666666666666666")

	l1 := createTestLedger(
		assetAddr1,
		[]int64{100, 200}, // in
		[]int64{},         // out
	)
	l2 := createTestLedger(
		assetAddr2,
		[]int64{50}, // in
		[]int64{10}, // out
	)
	lb.Ledgers[assetAddr1.Hex()] = l1
	lb.Ledgers[assetAddr2.Hex()] = l2

	// totalIn = 300 + 50 = 350
	totalIn = lb.TotalIn()
	assert.True(t, totalIn.Equal(base.NewWei(350)), "Should total 350 in")
}

func TestLedgerBookTotalOut(t *testing.T) {
	lb := NewLedgerBook(base.HexToAddress("0xeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee"))

	// No ledgers => totalOut = 0
	totalOut := lb.TotalOut()
	assert.True(t, totalOut.Equal(base.NewWei(0)), "Expected zero TotalOut with no ledgers")

	assetAddr1 := base.HexToAddress("0x7777777777777777777777777777777777777777")
	assetAddr2 := base.HexToAddress("0x8888888888888888888888888888888888888888")

	l1 := createTestLedger(assetAddr1,
		[]int64{},       // in
		[]int64{10, 20}, // out
	)
	l2 := createTestLedger(assetAddr2,
		[]int64{100},    // in
		[]int64{50, 25}, // out
	)
	lb.Ledgers[assetAddr1.Hex()] = l1
	lb.Ledgers[assetAddr2.Hex()] = l2

	// totalOut = (10+20) + (50+25) = 105
	totalOut = lb.TotalOut()
	assert.True(t, totalOut.Equal(base.NewWei(105)), "Should total 105 out")
}

func TestLedgerBookNetValue(t *testing.T) {
	lb := NewLedgerBook(base.HexToAddress("0xffffffffffffffffffffffffffffffffffffffff"))

	// No ledgers => net = 0
	netVal := lb.NetValue()
	assert.True(t, netVal.Equal(base.NewWei(0)), "Expected zero net value with no ledgers")

	// Add ledger => in=100, out=40 => net=60
	assetAddr1 := base.HexToAddress("0x9999999999999999999999999999999999999999")
	l1 := createTestLedger(assetAddr1,
		[]int64{100},
		[]int64{40},
	)
	lb.Ledgers[assetAddr1.Hex()] = l1
	netVal = lb.NetValue()
	assert.True(t, netVal.Equal(base.NewWei(60)), "Expected net=60 after first ledger")

	// Another ledger => in=50, out=90 => net=-40
	// Overall => 60 + (-40) = 20
	assetAddr2 := base.HexToAddress("0xaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa")
	l2 := createTestLedger(assetAddr2,
		[]int64{50},
		[]int64{90},
	)
	lb.Ledgers[assetAddr2.Hex()] = l2

	netVal = lb.NetValue()
	assert.True(t, netVal.Equal(base.NewWei(20)), "Expected overall net=20 after second ledger")

	// Demonstrate usage of LessThan / GreaterThan if desired
	assert.True(t, netVal.GreaterThan(base.NewWei(0)), "Net should be > 0")
	assert.True(t, netVal.LessThan(base.NewWei(25)), "Net should be < 25")
}

func TestLedgerBookStatements_Empty(t *testing.T) {
	lb := NewLedgerBook(base.HexToAddress("0xbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"))

	stmts, err := lb.Statements()
	assert.NoError(t, err, "Statements() should not error on empty ledger book")
	assert.Len(t, stmts, 0, "Expected no statements when ledger book is empty")
}

func TestLedgerBookStatements_OneLedger(t *testing.T) {
	lb := NewLedgerBook(base.HexToAddress("0xcccccccccccccccccccccccccccccccccccccccc"))

	p1 := types.NewPosting(100, 0, 0, 0, base.ZeroAddr)
	p1.AmountIn = *base.NewWei(50)
	p1.Sender = base.HexToAddress("0xf111111111111111111111111111111111111111")
	p1.Recipient = base.HexToAddress("0xf222222222222222222222222222222222222222")

	p2 := types.NewPosting(101, 0, 0, 0, base.ZeroAddr)
	p2.AmountOut = *base.NewWei(30)
	p2.Sender = base.HexToAddress("0xf333333333333333333333333333333333333333")
	p2.Recipient = base.HexToAddress("0xf444444444444444444444444444444444444444")

	singleLedger := Ledger{
		AssetAddress: base.HexToAddress("0x1111111111111111111111111111111111111111"),
		Entries: []LedgerEntry{
			{
				Postings: []types.Posting{
					p1,
				},
			},
			{
				Postings: []types.Posting{
					p2,
				},
			},
		},
	}
	lb.Ledgers[singleLedger.AssetAddress.Hex()] = singleLedger

	stmts, err := lb.Statements()
	assert.NoError(t, err, "Statements() should not fail with one ledger")
	assert.Len(t, stmts, 2, "Expected exactly 2 statements")

	// First statement
	first := stmts[0]
	// fmt.Println(first.AmountIn.String(), first.AmountOut.String())
	// fmt.Println(first.Sender.String(), first.Recipient.String())
	assert.Equal(t, lb.AccountedFor, first.AccountedFor, "AccountedFor should match ledger book address")
	assert.Equal(t, base.Blknum(100), first.BlockNumber, "BlockNumber mismatch on first statement")
	assert.True(t, first.AmountIn.Equal(base.NewWei(50)), "AmountIn mismatch on first statement")
	assert.Equal(t, "0xf111111111111111111111111111111111111111", first.Sender.Hex(), "Sender mismatch on first statement")
	assert.Equal(t, "0xf222222222222222222222222222222222222222", first.Recipient.Hex(), "Recipient mismatch on first statement")

	// Second statement
	second := stmts[1]
	// fmt.Println(second.AmountIn.String(), second.AmountOut.String())
	// fmt.Println(second.Sender.String(), second.Recipient.String())
	assert.Equal(t, lb.AccountedFor, second.AccountedFor, "AccountedFor should match ledger book address")
	assert.Equal(t, base.Blknum(101), second.BlockNumber, "BlockNumber mismatch on second statement")
	assert.True(t, second.AmountOut.Equal(base.NewWei(30)), "AmountOut mismatch on second statement")
	assert.Equal(t, "0xf333333333333333333333333333333333333333", second.Sender.Hex(), "Sender mismatch on second statement")
	assert.Equal(t, "0xf444444444444444444444444444444444444444", second.Recipient.Hex(), "Recipient mismatch on second statement")
}

// func TestLedgerBookStatements_MultipleLedgers(t *testing.T) {
// 	lb := NewLedgerBook(base.HexToAddress("0xdddddddddddddddddddddddddddddddddddddddd"))

// 	ledgerA := Ledger{
// 		AssetAddress: base.HexToAddress("0xaaaaaaaabbbbbbbbccccccccddddddddeeeeeeee"),
// 		Entries: []LedgerEntry{
// 			{
// 				Postings: []Posting{
// 					{
// 						BlockNumber: 100,
// 						AmountIn:    *base.NewWei(10),
// 					},
// 				},
// 			},
// 		},
// 	}

// 	ledgerB := Ledger{
// 		AssetAddress: base.HexToAddress("0xbbbbbbbbcccccccdddddddeeeeeeeeffffffff"),
// 		Entries: []LedgerEntry{
// 			{
// 				Postings: []Posting{
// 					{
// 						BlockNumber: 200,
// 						AmountOut:   *base.NewWei(5),
// 					},
// 					{
// 						BlockNumber: 201,
// 						AmountIn:    *base.NewWei(15),
// 					},
// 				},
// 			},
// 		},
// 	}

// 	lb.Ledgers[ledgerA.AssetAddress.Hex()] = ledgerA
// 	lb.Ledgers[ledgerB.AssetAddress.Hex()] = ledgerB

// 	stmts, err := lb.Statements()
// 	assert.NoError(t, err, "Statements() should not fail with multiple ledgers")
// 	assert.Len(t, stmts, 3, "Expected 3 statements total")

// 	expectedBlocks := []base.Blknum{100, 200, 201}
// 	for i, stmt := range stmts {
// 		assert.Equal(t, lb.AccountedFor, stmt.AccountedFor, "AccountedFor mismatch in statement %d", i)
// 		assert.Equal(t, expectedBlocks[i], stmt.BlockNumber, "BlockNumber mismatch in statement %d", i)
// 	}
// }
