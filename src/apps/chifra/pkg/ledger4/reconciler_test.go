package ledger4

import (
	"reflect"
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func TestBuildDoublyLinkedList(t *testing.T) {
	type testCase struct {
		name        string
		appearances []types.Appearance
		expectedLen int
		expectedSeq []types.Appearance
	}

	tests := []testCase{
		{
			name:        "empty input",
			appearances: []types.Appearance{},
			expectedLen: 0,
			expectedSeq: []types.Appearance{},
		},
		{
			name:        "single element",
			appearances: []types.Appearance{{BlockNumber: 1, TransactionIndex: 0}},
			expectedLen: 1,
			expectedSeq: []types.Appearance{{BlockNumber: 1, TransactionIndex: 0}},
		},
		{
			name: "multiple elements",
			appearances: []types.Appearance{
				{BlockNumber: 1, TransactionIndex: 0},
				{BlockNumber: 2, TransactionIndex: 1},
				{BlockNumber: 3, TransactionIndex: 2},
			},
			expectedLen: 3,
			expectedSeq: []types.Appearance{
				{BlockNumber: 1, TransactionIndex: 0},
				{BlockNumber: 2, TransactionIndex: 1},
				{BlockNumber: 3, TransactionIndex: 2},
			},
		},
	}

	for _, tc := range tests {
		t.Run(tc.name, func(t *testing.T) {
			l := BuildDoublyLinkedList(tc.appearances)
			if l.Len() != tc.expectedLen {
				t.Errorf("expected length %d, got %d", tc.expectedLen, l.Len())
			}
			if tc.expectedLen > 0 {
				var seq []types.Appearance
				for e := l.Front(); e != nil; e = e.Next() {
					node := e.Value.(*Node)
					seq = append(seq, *node.Appearance)
				}
				if !reflect.DeepEqual(seq, tc.expectedSeq) {
					t.Errorf("expected sequence %v, got %v", tc.expectedSeq, seq)
				}
			}
		})
	}

	t.Run("backward traversal", func(t *testing.T) {
		l := BuildDoublyLinkedList(tests[2].appearances)
		var seq []types.Appearance
		for e := l.Back(); e != nil; e = e.Prev() {
			node := e.Value.(*Node)
			seq = append(seq, *node.Appearance)
		}
		expectedBackward := []types.Appearance{
			{BlockNumber: 3, TransactionIndex: 2},
			{BlockNumber: 2, TransactionIndex: 1},
			{BlockNumber: 1, TransactionIndex: 0},
		}
		if !reflect.DeepEqual(seq, expectedBackward) {
			t.Errorf("expected backward sequence %v, got %v", expectedBackward, seq)
		}
	})
}

// func TestProcessTransactionsConcurrently(t *testing.T) {
// 	var fetchTxCalls, createTransfersCalls, queryBalancesCalls int
// 	var mu sync.Mutex

// 	var mockFetchTx = func(app *types.Appearance, fetchTraces bool) *types.Transaction {
// 		mu.Lock()
// 		defer mu.Unlock()
// 		fetchTxCalls++
// 		return &types.Transaction{BlockNumber: base.Blknum(app.BlockNumber)}
// 	}

// 	var mockCreateTransfers = func(tx *types.Transaction, accountFor base.Address) []types.AssetTransfer {
// 		mu.Lock()
// 		defer mu.Unlock()
// 		createTransfersCalls++
// 		return []types.AssetTransfer{{
// 			Asset: base.FAKE_ETH_ADDRESS,
// 			AmountIn:     *base.NewWei(100),
// 		}}
// 	}

// 	var mockQueryBalances = func(transfers []types.AssetTransfer) []types.Statement {
// 		mu.Lock()
// 		defer mu.Unlock()
// 		queryBalancesCalls++
// 		return []types.Statement{}
// 	}

// 	appearances := []types.Appearance{
// 		{BlockNumber: 1, TransactionIndex: 0},
// 		{BlockNumber: 2, TransactionIndex: 1},
// 		{BlockNumber: 3, TransactionIndex: 2},
// 	}

// 	l := BuildDoublyLinkedList(appearances)
// 	for e := l.Front(); e != nil; e = e.Next() {
// 		printElementDetails(e)
// 	}

// 	fetchTx = mockFetchTx
// 	createTransfers = mockCreateTransfers
// 	queryBalances = mockQueryBalances

// 	ProcessTransactionsConcurrently(l, base.HexToAddress("0x054993ab0f2b1acc0fdc65405ee203b4271bebe6"))

// 	expectedCalls := len(appearances)
// 	if fetchTxCalls != expectedCalls {
// 		t.Errorf("Expected fetchTx to be called %d times, got %d", expectedCalls, fetchTxCalls)
// 	}
// 	if createTransfersCalls != expectedCalls {
// 		t.Errorf("Expected createTransfers to be called %d times, got %d", expectedCalls, createTransfersCalls)
// 	}
// 	if queryBalancesCalls != expectedCalls {
// 		t.Errorf("Expected queryBalances to be called %d times, got %d", expectedCalls, queryBalancesCalls)
// 	}

// 	for e := l.Front(); e != nil; e = e.Next() {
// 		node := e.Value.(*Node)
// 		if node.Tx == nil {
// 			t.Errorf("Expected node.Tx to be populated for block %d, got nil", node.Appearance.BlockNumber)
// 		}
// 		if len(node.Transfers) == 0 {
// 			t.Errorf("Expected node.Transfers to be populated for block %d, got empty slice", node.Appearance.BlockNumber)
// 		}
// 		if node.Statement == nil {
// 			t.Errorf("Expected node.Statement to be populated for block %d, got nil", node.Appearance.BlockNumber)
// 		}
// 	}
// }

// func printElementDetails(e *list.Element) {
// 	if e == nil {
// 		fmt.Println("Element is nil")
// 		return
// 	}

// 	node := e.Value.(*Node)
// 	var prev, next *Node
// 	if e.Prev() != nil {
// 		prev = e.Prev().Value.(*Node)
// 	}
// 	if e.Next() != nil {
// 		next = e.Next().Value.(*Node)
// 	}
// 	fmt.Printf("%d.%d", node.Appearance.BlockNumber, node.Appearance.TransactionIndex)
// 	if prev != nil {
// 		fmt.Printf(" (%d.%d)", prev.Appearance.BlockNumber, prev.Appearance.TransactionIndex)
// 	} else {
// 		fmt.Printf(" <nil>")
// 	}
// 	if next != nil {
// 		fmt.Printf(" (%d.%d)", next.Appearance.BlockNumber, next.Appearance.TransactionIndex)
// 	} else {
// 		fmt.Print(" <nil>")
// 	}
// 	fmt.Println()
// }
