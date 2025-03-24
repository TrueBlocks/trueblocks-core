package ledger

// var testAddr = base.HexToAddress("0x1234")
// var testAsset = base.HexToAddress("0x6b175474e89094c44da98b954eedeac495271d0f")

// func setupTest(t *testing.T, assetFilters []base.Address) (*Reconciler, *types.Transaction, []types.Statement) {
// 	_ = t
// 	conn := rpc.TestConnection("mainnet", true, map[walk.CacheType]bool{walk.Cache_Statements: true})
// 	r := &Reconciler{
// 		Opts:       &ReconcilerOptions{AccountFor: testAddr, AssetFilters: assetFilters},
// 		Connection: conn,
// 	}

// 	trans := &types.Transaction{
// 		BlockNumber:      100,
// 		TransactionIndex: 5,
// 	}

// 	expectedStmts := []types.Statement{
// 		{Asset: base.FAKE_ETH_ADDRESS, AmountIn: *base.NewWeiStr("10")},
// 		{Asset: testAsset, AmountIn: *base.NewWeiStr("20")},
// 	}

// 	return r, trans, expectedStmts
// }

// func TestStatementsFromCache(t *testing.T) {
// 	// Sub-test: No Filters, Cache Hit
// 	t.Run("No Filters, Cache Hit", func(t *testing.T) {
// 		// Setup with no asset filters
// 		r, trans, expectedStmts := setupTest(t, nil)

// 		// Create a general statement group (no specific asset) and write to cache
// 		sg := &types.StatementGroup{
// 			Holder:           r.Opts.AccountFor,
// 			Asset:            base.ZeroAddr, // Empty address indicates no filter
// 			BlockNumber:      trans.BlockNumber,
// 			TransactionIndex: trans.TransactionIndex,
// 			Statements:       expectedStmts,
// 		}
// 		if err := r.Connection.WriteToCache(sg, walk.Cache_Statements, 1234567890); err != nil {
// 			t.Fatalf("failed to write to cache: %v", err)
// 		}

// 		// Call StatementsFromCache and verify results
// 		stmts, ok := r.StatementsFromCache(trans)
// 		if !ok {
// 			t.Error("expected cache hit (ok = true)")
// 		}
// 		if !reflect.DeepEqual(stmts, expectedStmts) {
// 			t.Errorf("statements mismatch:\n got: %v\n want: %v", stmts, expectedStmts)
// 		}
// 	})

// 	// Sub-test: No Filters, Cache Miss
// 	t.Run("No Filters, Cache Miss", func(t *testing.T) {
// 		// Setup with no asset filters
// 		r, trans, _ := setupTest(t, nil)

// 		// No cache population, expecting a miss
// 		stmts, ok := r.StatementsFromCache(trans)
// 		if ok {
// 			t.Error("expected cache miss (ok = false)")
// 		}
// 		if stmts != nil {
// 			t.Errorf("expected nil statements, got: %v", stmts)
// 		}
// 	})

// 	// Sub-test: With Filters, All Assets in Cache
// 	t.Run("With Filters, All Assets in Cache", func(t *testing.T) {
// 		// Setup with asset filters
// 		assetFilters := []base.Address{base.FAKE_ETH_ADDRESS, testAsset}
// 		r, trans, _ := setupTest(t, assetFilters)

// 		// Populate cache with statement groups for each asset
// 		sgETH := &types.StatementGroup{
// 			Holder:           r.Opts.AccountFor,
// 			Asset:            base.FAKE_ETH_ADDRESS,
// 			BlockNumber:      trans.BlockNumber,
// 			TransactionIndex: trans.TransactionIndex,
// 			Statements:       []types.Statement{{Asset: base.FAKE_ETH_ADDRESS, AmountIn: *base.NewWeiStr("10")}},
// 		}
// 		if err := r.Connection.WriteToCache(sgETH, walk.Cache_Statements, 1234567890); err != nil {
// 			t.Fatalf("failed to write ETH to cache: %v", err)
// 		}

// 		sgTest := &types.StatementGroup{
// 			Holder:           r.Opts.AccountFor,
// 			Asset:            testAsset,
// 			BlockNumber:      trans.BlockNumber,
// 			TransactionIndex: trans.TransactionIndex,
// 			Statements:       []types.Statement{{Asset: testAsset, AmountIn: *base.NewWeiStr("20")}},
// 		}
// 		if err := r.Connection.WriteToCache(sgTest, walk.Cache_Statements, 1234567890); err != nil {
// 			t.Fatalf("failed to write test asset to cache: %v", err)
// 		}

// 		// Call StatementsFromCache and verify results
// 		stmts, ok := r.StatementsFromCache(trans)
// 		if !ok {
// 			t.Error("expected cache hit (ok = true)")
// 		}
// 		expectedStmts := []types.Statement{
// 			{Asset: base.FAKE_ETH_ADDRESS, AmountIn: *base.NewWeiStr("10")},
// 			{Asset: testAsset, AmountIn: *base.NewWeiStr("20")},
// 		}
// 		if !reflect.DeepEqual(stmts, expectedStmts) {
// 			t.Errorf("statements mismatch:\n got: %v\n want: %v", stmts, expectedStmts)
// 		}
// 	})

// 	// Sub-test: With Filters, Partial Cache Miss
// 	t.Run("With Filters, Partial Cache Miss", func(t *testing.T) {
// 		// Setup with asset filters
// 		assetFilters := []base.Address{base.FAKE_ETH_ADDRESS, testAsset}
// 		r, trans, _ := setupTest(t, assetFilters)

// 		// Populate cache with only one asset (partial miss)
// 		sgETH := &types.StatementGroup{
// 			Holder:           r.Opts.AccountFor,
// 			Asset:            base.FAKE_ETH_ADDRESS,
// 			BlockNumber:      trans.BlockNumber,
// 			TransactionIndex: trans.TransactionIndex,
// 			Statements:       []types.Statement{{Asset: base.FAKE_ETH_ADDRESS, AmountIn: *base.NewWeiStr("10")}},
// 		}
// 		if err := r.Connection.WriteToCache(sgETH, walk.Cache_Statements, 1234567890); err != nil {
// 			t.Fatalf("failed to write to cache: %v", err)
// 		}

// 		// Call StatementsFromCache and verify results
// 		stmts, ok := r.StatementsFromCache(trans)
// 		if ok {
// 			t.Error("expected cache miss (ok = false) due to partial coverage")
// 		}
// 		if stmts != nil {
// 			t.Errorf("expected nil statements, got: %v", stmts)
// 		}
// 	})
// }

// // TestStatementsToCache tests the StatementsToCache function with various scenarios.
// func TestStatementsToCache(t *testing.T) {
// 	// Sub-test: No Filters
// 	t.Run("No Filters", func(t *testing.T) {
// 		// Setup with no asset filters
// 		r, trans, stmts := setupTest(t, nil)

// 		// Call StatementsToCache to write statements
// 		if err := r.StatementsToCache(trans, stmts); err != nil {
// 			t.Fatalf("failed to cache statements: %v", err)
// 		}

// 		// Read from cache and verify
// 		sg := &types.StatementGroup{
// 			Holder:           r.Opts.AccountFor,
// 			Asset:            base.ZeroAddr, // Empty address for no filter
// 			BlockNumber:      trans.BlockNumber,
// 			TransactionIndex: trans.TransactionIndex,
// 		}
// 		if err := r.Connection.ReadFromCache(sg); err != nil {
// 			t.Fatalf("failed to read from cache: %v", err)
// 		}
// 		if !reflect.DeepEqual(sg.Statements, stmts) {
// 			t.Errorf("statements mismatch:\n got: %v\n want: %v", sg.Statements, stmts)
// 		}
// 	})

// 	// Sub-test: With Filters
// 	t.Run("With Filters", func(t *testing.T) {
// 		// Setup with asset filters
// 		assetFilters := []base.Address{base.FAKE_ETH_ADDRESS, testAsset}
// 		r, trans, _ := setupTest(t, assetFilters)

// 		// Define statements for both assets
// 		stmts := []types.Statement{
// 			{Asset: base.FAKE_ETH_ADDRESS, AmountIn: *base.NewWeiStr("10")},
// 			{Asset: testAsset, AmountIn: *base.NewWeiStr("20")},
// 		}

// 		// Call StatementsToCache to write statements
// 		if err := r.StatementsToCache(trans, stmts); err != nil {
// 			t.Fatalf("failed to cache statements: %v", err)
// 		}

// 		// Verify cache for each asset
// 		for _, asset := range assetFilters {
// 			sg := &types.StatementGroup{
// 				Holder:           r.Opts.AccountFor,
// 				Asset:            asset,
// 				BlockNumber:      trans.BlockNumber,
// 				TransactionIndex: trans.TransactionIndex,
// 			}
// 			if err := r.Connection.ReadFromCache(sg); err != nil {
// 				t.Fatalf("failed to read from cache for asset %s: %v", asset, err)
// 			}
// 			expected := []types.Statement{}
// 			for _, stmt := range stmts {
// 				if stmt.Asset == asset {
// 					expected = append(expected, stmt)
// 				}
// 			}
// 			if !reflect.DeepEqual(sg.Statements, expected) {
// 				t.Errorf("statements for asset %s mismatch:\n got: %v\n want: %v", asset, sg.Statements, expected)
// 			}
// 		}
// 	})

// 	// Sub-test: With Filters, Some Assets Empty
// 	t.Run("With Filters, Some Assets Empty", func(t *testing.T) {
// 		// Setup with asset filters
// 		assetFilters := []base.Address{base.FAKE_ETH_ADDRESS, testAsset}
// 		r, trans, _ := setupTest(t, assetFilters)

// 		// Define statements for only one asset
// 		stmts := []types.Statement{
// 			{Asset: base.FAKE_ETH_ADDRESS, AmountIn: *base.NewWeiStr("10")},
// 		}

// 		// Call StatementsToCache to write statements
// 		if err := r.StatementsToCache(trans, stmts); err != nil {
// 			t.Fatalf("failed to cache statements: %v", err)
// 		}

// 		// Verify cache for each asset
// 		for _, asset := range assetFilters {
// 			sg := &types.StatementGroup{
// 				Holder:           r.Opts.AccountFor,
// 				Asset:            asset,
// 				BlockNumber:      trans.BlockNumber,
// 				TransactionIndex: trans.TransactionIndex,
// 			}
// 			err := r.Connection.ReadFromCache(sg)
// 			if asset == base.FAKE_ETH_ADDRESS {
// 				// Expect statements for ETH
// 				if err != nil {
// 					t.Fatalf("failed to read from cache for asset %s: %v", asset, err)
// 				}
// 				expected := []types.Statement{{Asset: base.FAKE_ETH_ADDRESS, AmountIn: *base.NewWeiStr("10")}}
// 				if !reflect.DeepEqual(sg.Statements, expected) {
// 					t.Errorf("statements for asset %s mismatch:\n got: %v\n want: %v", asset, sg.Statements, expected)
// 				}
// 			} else if asset == testAsset {
// 				// Expect no statements for testAsset (cache miss)
// 				if err == nil {
// 					t.Errorf("expected cache miss for asset %s, but found statements: %v", asset, sg.Statements)
// 				}
// 			}
// 		}
// 	})
// }
