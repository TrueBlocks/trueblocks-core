package ledger

// func TestIntegration_GetStatements(t *testing.T) {
// 	conn := rpc.TempConnection("mainnet")
// 	accountFor := base.HexToAddress("0x054993ab0f2b1acc0fdc65405ee203b4271bebe6")
// 	appearances := []types.Appearance{
// 		{
// 			BlockNumber:      10277711,
// 			TransactionIndex: 64,
// 		},
// 	}
// 	ledger := NewLedger(conn, appearances, accountFor, 10277710, 10277712, true, true, false, false, false, nil)
// 	tx, err := conn.GetTransactionByNumberAndId(10277711, 64)
// 	if err != nil {
// 		t.Fatalf("failed to get transaction: %v", err)
// 	}
// 	filter := &filter.AppearanceFilter{}
// 	statements, err := ledger.GetStatements(filter, tx)
// 	if err != nil {
// 		t.Fatalf("GetStatements failed: %v", err)
// 	}
// 	if len(statements) != 2 {
// 		t.Fatalf("expected 2 statements, got %d", len(statements))
// 	}
// 	var foundETH, foundDAI bool
// 	daiAddr := base.HexToAddress("0x6b175474e89094c44da98b954eedeac495271d0f")
// 	for _, s := range statements {
// 		if s.AssetAddr == base.FAKE_ETH_ADDRESS {
// 			foundETH = true
// 		}
// 		if s.AssetAddr == daiAddr {
// 			foundDAI = true
// 		}
// 		t.Logf("Statement: %s", s.String())
// 	}
// 	if !foundETH {
// 		t.Errorf("ETH statement not found")
// 	}
// 	if !foundDAI {
// 		t.Errorf("DAI statement not found")
// 	}
// }

// func TestIntegration_GetStatements_Detailed(t *testing.T) {
// 	conn := rpc.TempConnection("mainnet")
// 	accountFor := base.HexToAddress("0x054993ab0f2b1acc0fdc65405ee203b4271bebe6")
// 	ledger := NewLedger(conn, []types.Appearance{
// 		{BlockNumber: 10277711, TransactionIndex: 64},
// 	}, accountFor, 10277711, 10277711, true, true, false, false, false, nil)

// 	tx, err := conn.GetTransactionByNumberAndId(10277711, 64)
// 	if err != nil {
// 		t.Fatalf("failed to get transaction: %v", err)
// 	}
// 	filter := &filter.AppearanceFilter{}
// 	stmts, err := ledger.GetStatements(filter, tx)
// 	if err != nil {
// 		t.Fatalf("GetStatements failed: %v", err)
// 	}
// 	if len(stmts) != 2 {
// 		t.Fatalf("expected 2 statements, got %d", len(stmts))
// 	}
// 	appKey := ledger.getAppContextKey(10277711, 64)
// 	appCtx, exists := ledger.appContexts[appKey]
// 	if !exists {
// 		t.Fatalf("no app context for key %s", appKey)
// 	}
// 	ethCheckpoint := ""
// 	if appCtx.Cur() > 0 {
// 		ethCheckpoint = fmt.Sprintf("0x%x", appCtx.Cur()-1)
// 	} else {
// 		ethCheckpoint = "0x0"
// 	}
// 	ethBeg, err := conn.GetBalanceAt(accountFor, 10277710)
// 	if err != nil {
// 		t.Fatalf("failed to get ETH balance: %v", err)
// 	}
// 	daiAddr := base.HexToAddress("0x6b175474e89094c44da98b954eedeac495271d0f")
// 	daiBeg, err := conn.GetBalanceAtToken(daiAddr, accountFor, ethCheckpoint)
// 	if err != nil {
// 		t.Fatalf("failed to get DAI balance: %v", err)
// 	}
// 	var foundETH, foundDAI bool
// 	for _, s := range stmts {
// 		if s.AssetAddr == base.FAKE_ETH_ADDRESS {
// 			foundETH = true
// 			if s.BegBal.Cmp(ethBeg) != 0 {
// 				t.Errorf("ETH: expected BegBal %s, got %s", ethBeg.Text(10), s.BegBal.Text(10))
// 			}
// 		}
// 		if s.AssetAddr == daiAddr {
// 			foundDAI = true
// 			if s.BegBal.Cmp(daiBeg) != 0 {
// 				t.Errorf("DAI: expected BegBal %s, got %s", daiBeg.Text(10), s.BegBal.Text(10))
// 			}
// 		}
// 		t.Logf("Statement: %s", s.String())
// 	}
// 	if !foundETH {
// 		t.Error("ETH statement not found")
// 	}
// 	if !foundDAI {
// 		t.Error("DAI statement not found")
// 	}
// }
