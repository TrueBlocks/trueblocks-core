package ledger

// func TestGetStatementsFromLogCredit(t *testing.T) {
// 	conn := rpc.TempConnection("mainnet")
// 	ledger := &Ledger{
// 		accountFor:    base.HexToAddress("0x1111111111111111111111111111111111111111"),
// 		assetContexts: make(map[base.Address]*assetContext),
// 		appContexts:   make(map[appContextKey]*appContext),
// 		connection:    conn,
// 		names:         make(map[base.Address]types.Name),
// 		testMode:      true,
// 	}
// 	bn := base.Blknum(100)
// 	txid := base.Txnum(1)
// 	key := ledger.getAppContextKey(bn, txid)
// 	ledger.appContexts[key] = newAppContext(bn-1, bn, bn+1, true, false, false)
// 	sender := base.HexToAddress("0xaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa")
// 	recipient := ledger.accountFor
// 	// dummyData represents 1000000 in hexadecimal
// 	dummyData := "0x" + "00000000000000000000000000000000000000000000000000000000000f4240"
// 	logEntry := &types.Log{
// 		BlockNumber:      bn,
// 		TransactionIndex: txid,
// 		LogIndex:         0,
// 		TransactionHash:  base.HexToHash("0xdeadbeef"),
// 		Timestamp:        base.Timestamp(1234567890),
// 		Address:          base.HexToAddress("0x2222222222222222222222222222222222222222"),
// 		Topics: []base.Hash{
// 			topics.TransferTopic,
// 			base.HexToHash(sender.Hex()),
// 			base.HexToHash(recipient.Hex()),
// 		},
// 		Data: dummyData,
// 	}
// 	stmt, err := ledger.getStatementsFromLog(logEntry)
// 	if err != nil {
// 		t.Fatalf("getStatementsFromLog error: %v", err)
// 	}
// 	expectedWei := base.NewWeiStr("1000000")
// 	if stmt.AmountIn.Cmp(expectedWei) != 0 {
// 		t.Errorf("Expected AmountIn %s, got %s", expectedWei.Text(10), stmt.AmountIn.Text(10))
// 	}
// 	if stmt.AmountOut.Cmp(base.NewWei(0)) != 0 {
// 		t.Errorf("Expected AmountOut 0, got %s", stmt.AmountOut.Text(10))
// 	}
// 	if stmt.RollingBalance.Cmp(expectedWei) != 0 {
// 		t.Errorf("Expected RollingBalance %s, got %s", expectedWei.Text(10), stmt.RollingBalance.Text(10))
// 	}
// }

// func TestGetStatementsFromLogDebit(t *testing.T) {
// 	conn := rpc.TempConnection("mainnet")
// 	ledger := &Ledger{
// 		accountFor:    base.HexToAddress("0x1111111111111111111111111111111111111111"),
// 		assetContexts: make(map[base.Address]*assetContext),
// 		appContexts:   make(map[appContextKey]*appContext),
// 		connection:    conn,
// 		names:         make(map[base.Address]types.Name),
// 		testMode:      true,
// 	}
// 	bn := base.Blknum(100)
// 	txid := base.Txnum(1)
// 	key := ledger.getAppContextKey(bn, txid)
// 	ledger.appContexts[key] = newAppContext(bn-1, bn, bn+1, true, false, false)
// 	sender := ledger.accountFor
// 	recipient := base.HexToAddress("0xbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb")
// 	dummyData := "0x" + "00000000000000000000000000000000000000000000000000000000000f4240"
// 	logEntry := &types.Log{
// 		BlockNumber:      bn,
// 		TransactionIndex: txid,
// 		LogIndex:         0,
// 		TransactionHash:  base.HexToHash("0xdeadbeef"),
// 		Timestamp:        base.Timestamp(1234567890),
// 		Address:          base.HexToAddress("0x2222222222222222222222222222222222222222"),
// 		Topics: []base.Hash{
// 			topics.TransferTopic,
// 			base.HexToHash(sender.Hex()),
// 			base.HexToHash(recipient.Hex()),
// 		},
// 		Data: dummyData,
// 	}
// 	stmt, err := ledger.getStatementsFromLog(logEntry)
// 	if err != nil {
// 		t.Fatalf("getStatementsFromLog error: %v", err)
// 	}
// 	expectedWei := base.NewWeiStr("1000000")
// 	if stmt.AmountOut.Cmp(expectedWei) != 0 {
// 		t.Errorf("Expected AmountOut %s, got %s", expectedWei.Text(10), stmt.AmountOut.Text(10))
// 	}
// 	if stmt.AmountIn.Cmp(base.NewWei(0)) != 0 {
// 		t.Errorf("Expected AmountIn 0, got %s", stmt.AmountIn.Text(10))
// 	}
// 	negativeWei := base.NewWeiStr("-1000000")
// 	if stmt.RollingBalance.Cmp(negativeWei) != 0 {
// 		t.Errorf("Expected RollingBalance %s, got %s", negativeWei.Text(10), stmt.RollingBalance.Text(10))
// 	}
// }

// func TestGetStatementsFromLogSelfTransfer(t *testing.T) {
// 	conn := rpc.TempConnection("mainnet")
// 	ledger := &Ledger{
// 		accountFor:    base.HexToAddress("0x1111111111111111111111111111111111111111"),
// 		assetContexts: make(map[base.Address]*assetContext),
// 		appContexts:   make(map[appContextKey]*appContext),
// 		connection:    conn,
// 		names:         make(map[base.Address]types.Name),
// 		testMode:      true,
// 	}
// 	bn := base.Blknum(100)
// 	txid := base.Txnum(1)
// 	key := ledger.getAppContextKey(bn, txid)
// 	ledger.appContexts[key] = newAppContext(bn-1, bn, bn+1, true, false, false)
// 	sender := ledger.accountFor
// 	recipient := ledger.accountFor
// 	dummyData := "0x" + "00000000000000000000000000000000000000000000000000000000000f4240"
// 	logEntry := &types.Log{
// 		BlockNumber:      bn,
// 		TransactionIndex: txid,
// 		LogIndex:         0,
// 		TransactionHash:  base.HexToHash("0xdeadbeef"),
// 		Timestamp:        base.Timestamp(1234567890),
// 		Address:          base.HexToAddress("0x2222222222222222222222222222222222222222"),
// 		Topics: []base.Hash{
// 			topics.TransferTopic,
// 			base.HexToHash(sender.Hex()),
// 			base.HexToHash(recipient.Hex()),
// 		},
// 		Data: dummyData,
// 	}
// 	stmt, err := ledger.getStatementsFromLog(logEntry)
// 	if err != nil {
// 		t.Fatalf("getStatementsFromLog error: %v", err)
// 	}
// 	expectedWei := base.NewWeiStr("1000000")
// 	if stmt.AmountIn.Cmp(expectedWei) != 0 {
// 		t.Errorf("Expected AmountIn %s, got %s", expectedWei.Text(10), stmt.AmountIn.Text(10))
// 	}
// 	if stmt.AmountOut.Cmp(expectedWei) != 0 {
// 		t.Errorf("Expected AmountOut %s, got %s", expectedWei.Text(10), stmt.AmountOut.Text(10))
// 	}
// 	if stmt.RollingBalance.Cmp(base.NewWei(0)) != 0 {
// 		t.Errorf("Expected RollingBalance 0, got %s", stmt.RollingBalance.Text(10))
// 	}
// }
