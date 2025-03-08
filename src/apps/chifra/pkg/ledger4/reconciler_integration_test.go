//go:build integration
// +build integration

package ledger4

import (
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// type testCase struct {
// 	name        string
// 	appearances []types.Appearance
// 	expected    []ledger10.AssetTransfer
// 	opts        *ledger10.ReconcilerOptions
// }

// func Equal(t1, t2 *ledger10.AssetTransfer) bool {
// 	e1 := t1.BlockNumber == t2.BlockNumber
// 	s1 := fmt.Sprintf("%d\t%d", t1.BlockNumber, t2.BlockNumber)
// 	fmt.Println("bn:     ", e1, s1)

// 	e2 := t1.TransactionIndex == t2.TransactionIndex
// 	s2 := fmt.Sprintf("%d\t%d", t1.TransactionIndex, t2.TransactionIndex)
// 	fmt.Println("txid:   ", e2, s2)

// 	e3 := t1.LogIndex == t2.LogIndex
// 	s3 := fmt.Sprintf("%d\t%d", t1.LogIndex, t2.LogIndex)
// 	fmt.Println("logid:  ", e3, s3)

// 	e4 := t1.Asset == t2.Asset
// 	s4 := fmt.Sprintf("%s\t%s", t1.Asset.Display(3, 3), t2.Asset.Display(3, 3))
// 	fmt.Println("asset:  ", e4, s4)

// 	e5 := t1.Holder == t2.Holder
// 	s5 := fmt.Sprintf("%s\t%s", t1.Holder.Display(3, 3), t2.Holder.Display(3, 3))
// 	fmt.Println("holder: ", e5, s5)

// 	e6 := t1.AmountNet() == t2.AmountNet()
// 	s6 := fmt.Sprintf("%s\t%s", t1.AmountNet().Text(10), t2.AmountNet().Text(10))
// 	fmt.Println("amt:    ", e6, s6)

// 	return e1 && e2 && e3 && e4 && e5 && e6
// }

// func printOne(t1 *ledger10.AssetTransfer) {
// 	fmt.Printf("%d.%d.%d %s %s %s\n",
// 		t1.BlockNumber,
// 		t1.TransactionIndex,
// 		t1.LogIndex,
// 		t1.Asset.Display(3, 3),
// 		t1.Holder.Display(3, 3),
// 		t1.AmountNet().Text(10))
// }

// func TestCreateTransfersIntegration(t *testing.T) {
// 	addr0549 := base.HexToAddress("0xf5b2c59f6db42ffcdfc1625999c81fdf17953384")
// 	addrf5b2 := base.HexToAddress("0xf5b2c59f6db42ffcdfc1625999c81fdf17953384")
// 	tests := []testCase{
// 		{
// 			name: "Single Ether Transfer",
// 			opts: &ledger10.ReconcilerOptions{
// 				Connection: rpc.TempConnection("mainnet"),
// 				AccountFor: addr0549,
// 				AssetFilters: []base.Address{
// 					addrf5b2,
// 				},
// 			},
// 			appearances: []types.Appearance{
// 				{
// 					BlockNumber:      15549162,
// 					TransactionIndex: 83,
// 				},
// 			},
// 			expected: []ledger10.AssetTransfer{
// 				{
// 					BlockNumber:      15549162,
// 					TransactionIndex: 83,
// 					LogIndex:         1774,
// 					Asset:     addrf5b2,
// 					Holder:           addr0549,
// 					AmountIn:         base.MustParseWei("2017000000000000000000"),
// 				},
// 			},
// 		},
// 		{
// 			name: "No Transfers",
// 			appearances: []types.Appearance{
// 				{BlockNumber: 50111, TransactionIndex: 0},
// 			},
// 			expected: nil,
// 		},
// 	}

// 	for _, tc := range tests {
// 		t.Run(tc.name, func(t *testing.T) {
// 			r := NewReconciler(tc.opts)
// 			var allTransfers []ledger10.AssetTransfer
// 			for _, appearance := range tc.appearances {
// 				tx := r.fetchTx(&appearance, false)
// 				if tx == nil {
// 					t.Fatalf("Failed to fetch transaction for appearance %+v", appearance)
// 				}
// 				transfers := r.createTransfers(tx)
// 				allTransfers = append(allTransfers, transfers...)
// 			}

// 			if len(allTransfers) != len(tc.expected) {
// 				for i := 0; i < len(allTransfers); i++ {
// 					printOne(&allTransfers[i])
// 				}
// 				t.Errorf("Expected %d transfers, got %d", len(tc.expected), len(allTransfers))
// 				// t.Logf("Actual transfers: %+v", allTransfers)
// 				return
// 			}

// 			for i, expected := range tc.expected {
// 				actual := allTransfers[i]
// 				if !Equal(&actual, &expected) {
// 					t.Errorf("Transfer %d mismatch:\nExpected: %+v\nGot:      %+v", i, expected, actual)
// 				}
// 			}
// 		})
// 	}
// }

// TestFetchTxIntegration verifies that fetchTx retrieves the correct transaction data from the blockchain.
func TestFetchTxIntegration(t *testing.T) {
	// Define a known transaction appearance
	app := &types.Appearance{
		BlockNumber:      15549162,
		TransactionIndex: 83,
	}

	// Fetch the transaction without traces
	// dummy := Reconciler4{}
	tx := fetchTx(app, false)
	if tx == nil {
		t.Fatal("Failed to fetch transaction")
	}

	if tx.BlockNumber != base.Blknum(app.BlockNumber) || tx.TransactionIndex != base.Txnum(app.TransactionIndex) {
		t.Errorf("Expected block 12345678 tx 0, got block %d tx %d", tx.BlockNumber, tx.TransactionIndex)
	}

	// Optional: Add more checks for fields like Hash, From, To, etc., if desired
}

// TestCreateTransfersIntegration verifies that createTransfers extracts asset transfers from a transaction.
// func TestCreateTransfersIntegration(t *testing.T) {
// 	// Fetch a known transaction with transfers
// 	app := &types.Appearance{
// 		BlockNumber:      12345678,
// 		TransactionIndex: 0,
// 	}
// 	tx := fetchTx(app, false)
// 	if tx == nil {
// 		t.Fatal("Failed to fetch transaction")
// 	}

// 	// Extract transfers
// 	transfers := createTransfers(tx)
// 	if len(transfers) == 0 {
// 		t.Error("Expected at least one transfer, got none")
// 	}

// 	// Example assertion: Check for a specific known transfer
// 	// Replace with actual values from your chosen transaction
// 	expectedTransfer := ledger10.AssetTransfer{
// 		Asset: base.HexToAddress("0xa0b86991c6218b36c1d19d4a2e9eb0ce3606eb48"), // USDC contract address
// 		From:         "0x123...",                                                      // Replace with actual 'From' address
// 		To:           "0x456...",                                                      // Replace with actual 'To' address
// 		Amount:       big.NewInt(1000000),                                             // e.g., 1 USDC (6 decimals)
// 	}
// 	found := false
// 	for _, transfer := range transfers {
// 		if transfer.Asset == expectedTransfer.Asset &&
// 			transfer.From == expectedTransfer.From &&
// 			transfer.To == expectedTransfer.To &&
// 			transfer.Amount.Cmp(expectedTransfer.Amount) == 0 {
// 			found = true
// 			break
// 		}
// 	}
// 	if !found {
// 		t.Errorf("Expected transfer not found: %+v", expectedTransfer)
// 	}
// }
