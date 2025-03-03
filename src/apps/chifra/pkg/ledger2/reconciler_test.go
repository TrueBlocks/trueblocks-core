package ledger2

// TestReconciler2Basic runs a simple scenario where the accountedForAddress is "0x1002"
// and we pass in one or two appearances plus some asset transfers.
// func TestReconciler2Basic(t *testing.T) {
// 	// Suppose this is our address of interest
// 	rec := NewReconciler2(base.HexToAddress("0x1002"))

// 	// appearances := []types.Appearance{
// 	// 	{Address: base.HexToAddress("0x1002"), BlockNumber: 123, TransactionIndex: 0},
// 	// 	{Address: base.HexToAddress("0x1002"), BlockNumber: 123, TransactionIndex: 1},
// 	// 	{Address: base.HexToAddress("0x1002"), BlockNumber: 124, TransactionIndex: 0},
// 	// }
// 	tx := types.Transaction{BlockNumber: 123, TransactionIndex: 0}

// 	// We have 3 appearances, each might have multiple AssetTransfers. We'll create a small set:
// 	transfers := []ledger3.AssetTransfer{
// 		// For block=123 tx=0, an incoming amount
// 		{
// 			BlockNumber:      123,
// 			TransactionIndex: 0,
// 			AssetAddr:        base.FAKE_ETH_ADDRESS, // native ETH
// 			AmountIn:         *base.NewWei(50),
// 			LogIndex:         0,
// 			Sender:           base.HexToAddress("0x1001"),
// 			Recipient:        base.HexToAddress("0x1002"),
// 		},
// 		// For block=123 tx=1, an outgoing amount
// 		{
// 			BlockNumber:      123,
// 			TransactionIndex: 1,
// 			AssetAddr:        base.FAKE_ETH_ADDRESS,
// 			AmountOut:        *base.NewWei(20),
// 			LogIndex:         1,
// 			Sender:           base.HexToAddress("0x1002"),
// 			Recipient:        base.HexToAddress("0x1003"),
// 		},
// 		// For block=124 tx=0, an incoming amount
// 		{
// 			BlockNumber:      124,
// 			TransactionIndex: 0,
// 			AssetAddr:        base.FAKE_ETH_ADDRESS,
// 			AmountIn:         *base.NewWei(100),
// 			LogIndex:         2,
// 			Sender:           base.HexToAddress("0x1004"),
// 			Recipient:        base.HexToAddress("0x1002"),
// 		},
// 	}

// 	// Process the appearances + transfers
// 	rec.ProcessTransaction(&tx, transfers)

// 	// We should now have a LedgerBook with one Ledger (asset "0x0").
// 	if len(rec.LedgerBook.Ledgers) != 1 {
// 		t.Fatalf("Expected 1 ledger, got=%d", len(rec.LedgerBook.Ledgers))
// 	}

// 	led, ok := rec.LedgerBook.GetLedger(base.FAKE_ETH_ADDRESS)
// 	if !ok {
// 		t.Fatalf("Expected a ledger for asset=0x0")
// 	}

// 	// We appended 3 appearances => 3 LedgerEntries
// 	if len(led.Entries) != 3 {
// 		t.Fatalf("Expected 3 ledger entries, got=%d", len(led.Entries))
// 	}

// 	totalIn := led.TotalIn()
// 	if totalIn.String() != "150" {
// 		t.Fatalf("Expected totalIn=150, got=%s", totalIn.String())
// 	}

// 	totalOut := led.TotalOut()
// 	if totalOut.String() != "20" {
// 		t.Fatalf("Expected totalOut=20, got=%s", totalOut.String())
// 	}

// 	netVal := led.NetValue()
// 	if netVal.String() != "130" {
// 		t.Fatalf("Expected netVal=130, got=%s", netVal.String())
// 	}
// }
