package ledger

import (
	"reflect"
	"strings"
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/walk"
	"github.com/go-test/deep"
)

// -----------------------------------------------------------------
var testAddr = base.HexToAddress("0x1234")
var testAsset = base.HexToAddress("0x6b175474e89094c44da98b954eedeac495271d0f")

// -----------------------------------------------------------------
func setupTest(t *testing.T, assetFilters []base.Address) (*Reconciler, *types.Transaction, []types.Statement) {
	_ = t
	conn := rpc.TestConnection("mainnet", true, map[walk.CacheType]bool{walk.Cache_Statements: true})
	r := &Reconciler{
		Opts:       &ReconcilerOptions{AccountFor: testAddr, AssetFilters: assetFilters},
		Connection: conn,
	}

	trans := &types.Transaction{
		BlockNumber:      100,
		TransactionIndex: 5,
	}

	expectedStmts := []types.Statement{
		{Asset: base.FAKE_ETH_ADDRESS, AmountIn: *base.NewWeiStr("10"), BlockNumber: trans.BlockNumber, TransactionIndex: trans.TransactionIndex},
		{Asset: testAsset, AmountIn: *base.NewWeiStr("20"), BlockNumber: trans.BlockNumber, TransactionIndex: trans.TransactionIndex},
	}

	return r, trans, expectedStmts
}

// -----------------------------------------------------------------
func TestStatementsFromCache(t *testing.T) {
	t.Run("No Filters, Cache Hit", func(t *testing.T) {
		r, trans, expectedStmts := setupTest(t, nil)

		sg := &types.StatementGroup{
			Holder:           r.Opts.AccountFor,
			Asset:            base.ZeroAddr,
			BlockNumber:      trans.BlockNumber,
			TransactionIndex: trans.TransactionIndex,
			Statements:       expectedStmts,
		}
		if err := r.Connection.WriteToCache(sg, walk.Cache_Statements, 1234567890); err != nil {
			t.Fatalf("failed to write to cache: %v", err)
		}

		stmts, ok := r.statementsFromCache(trans)
		if !ok {
			t.Error("expected cache hit (ok = true)")
		}
		for i := range stmts {
			stmts[i].Transaction = nil
			stmts[i].Log = nil
		}
		if diff := deep.Equal(stmts, expectedStmts); diff != nil {
			t.Errorf("statements mismatch:\n%s", strings.Join(diff, "\n"))
		}
	})

	t.Run("No Filters, Cache Miss", func(t *testing.T) {
		r, trans, _ := setupTest(t, nil)

		stmts, ok := r.statementsFromCache(trans)
		if ok {
			t.Error("expected cache miss (ok = false)")
		}
		if stmts != nil {
			t.Errorf("expected nil statements, got: %v", stmts)
		}
	})

	t.Run("With Filters, All Assets in Cache", func(t *testing.T) {
		assetFilters := []base.Address{base.FAKE_ETH_ADDRESS, testAsset}
		r, trans, _ := setupTest(t, assetFilters)

		sgETH := &types.StatementGroup{
			Holder:           r.Opts.AccountFor,
			Asset:            base.FAKE_ETH_ADDRESS,
			BlockNumber:      trans.BlockNumber,
			TransactionIndex: trans.TransactionIndex,
			Statements:       []types.Statement{{Asset: base.FAKE_ETH_ADDRESS, AmountIn: *base.NewWeiStr("10")}},
		}
		if err := r.Connection.WriteToCache(sgETH, walk.Cache_Statements, 1234567890); err != nil {
			t.Fatalf("failed to write ETH to cache: %v", err)
		}

		sgTest := &types.StatementGroup{
			Holder:           r.Opts.AccountFor,
			Asset:            testAsset,
			BlockNumber:      trans.BlockNumber,
			TransactionIndex: trans.TransactionIndex,
			Statements:       []types.Statement{{Asset: testAsset, AmountIn: *base.NewWeiStr("20")}},
		}
		if err := r.Connection.WriteToCache(sgTest, walk.Cache_Statements, 1234567890); err != nil {
			t.Fatalf("failed to write test asset to cache: %v", err)
		}

		stmts, ok := r.statementsFromCache(trans)
		if !ok {
			t.Error("expected cache hit (ok = true)")
		}
		for i := range stmts {
			stmts[i].Transaction = nil
			stmts[i].Log = nil
		}
		expectedStmts := []types.Statement{
			{Asset: base.FAKE_ETH_ADDRESS, AmountIn: *base.NewWeiStr("10")},
			{Asset: testAsset, AmountIn: *base.NewWeiStr("20")},
		}
		if diff := deep.Equal(stmts, expectedStmts); diff != nil {
			t.Errorf("statements mismatch:\n%s", strings.Join(diff, "\n"))
		}
	})

	t.Run("With Filters, Partial Cache Miss", func(t *testing.T) {
		assetFilters := []base.Address{base.FAKE_ETH_ADDRESS, testAsset}
		r, trans, _ := setupTest(t, assetFilters)

		sgETH := &types.StatementGroup{
			Holder:           r.Opts.AccountFor,
			Asset:            base.FAKE_ETH_ADDRESS,
			BlockNumber:      trans.BlockNumber,
			TransactionIndex: trans.TransactionIndex,
			Statements:       []types.Statement{{Asset: base.FAKE_ETH_ADDRESS, AmountIn: *base.NewWeiStr("10")}},
		}
		if err := r.Connection.WriteToCache(sgETH, walk.Cache_Statements, 1234567890); err != nil {
			t.Fatalf("failed to write to cache: %v", err)
		}

		stmts, ok := r.statementsFromCache(trans)
		if ok {
			t.Error("expected cache miss (ok = false) due to partial coverage")
		}
		for i := range stmts {
			stmts[i].Transaction = nil
			stmts[i].Log = nil
		}
		if stmts != nil {
			t.Errorf("expected nil statements, got: %v", stmts)
		}
	})
}

// -----------------------------------------------------------------
func TestStatementsToCache(t *testing.T) {
	t.Run("No Filters", func(t *testing.T) {
		r, trans, stmts := setupTest(t, nil)

		if err := r.statementsToCache(trans, stmts); err != nil {
			t.Fatalf("failed to cache statements: %v", err)
		}

		sg := &types.StatementGroup{
			Holder:           r.Opts.AccountFor,
			Asset:            base.ZeroAddr,
			BlockNumber:      trans.BlockNumber,
			TransactionIndex: trans.TransactionIndex,
		}
		if err := r.Connection.ReadFromCache(sg); err != nil {
			t.Fatalf("failed to read from cache: %v", err)
		}
		if !reflect.DeepEqual(sg.Statements, stmts) {
			t.Errorf("statements mismatch:\n got: %v\n want: %v", sg.Statements, stmts)
		}
	})

	t.Run("With Filters", func(t *testing.T) {
		assetFilters := []base.Address{base.FAKE_ETH_ADDRESS, testAsset}
		r, trans, _ := setupTest(t, assetFilters)

		stmts := []types.Statement{
			{Asset: base.FAKE_ETH_ADDRESS, AmountIn: *base.NewWeiStr("10")},
			{Asset: testAsset, AmountIn: *base.NewWeiStr("20")},
		}

		if err := r.statementsToCache(trans, stmts); err != nil {
			t.Fatalf("failed to cache statements: %v", err)
		}

		for _, asset := range assetFilters {
			sg := &types.StatementGroup{
				Holder:           r.Opts.AccountFor,
				Asset:            asset,
				BlockNumber:      trans.BlockNumber,
				TransactionIndex: trans.TransactionIndex,
			}
			if err := r.Connection.ReadFromCache(sg); err != nil {
				t.Fatalf("failed to read from cache for asset %s: %v", asset, err)
			}
			expected := []types.Statement{}
			for _, stmt := range stmts {
				if stmt.Asset == asset {
					expected = append(expected, stmt)
				}
			}
			if !reflect.DeepEqual(sg.Statements, expected) {
				t.Errorf("statements for asset %s mismatch:\n got: %v\n want: %v", asset, sg.Statements, expected)
			}
		}
	})

	t.Run("With Filters, Some Assets Empty", func(t *testing.T) {
		assetFilters := []base.Address{base.FAKE_ETH_ADDRESS, testAsset}
		r, trans, _ := setupTest(t, assetFilters)

		stmts := []types.Statement{
			{Asset: base.FAKE_ETH_ADDRESS, AmountIn: *base.NewWeiStr("10")},
		}

		if err := r.statementsToCache(trans, stmts); err != nil {
			t.Fatalf("failed to cache statements: %v", err)
		}

		for _, asset := range assetFilters {
			sg := &types.StatementGroup{
				Holder:           r.Opts.AccountFor,
				Asset:            asset,
				BlockNumber:      trans.BlockNumber,
				TransactionIndex: trans.TransactionIndex,
			}
			err := r.Connection.ReadFromCache(sg)
			if asset == base.FAKE_ETH_ADDRESS {
				if err != nil {
					t.Fatalf("failed to read from cache for asset %s: %v", asset, err)
				}
				expected := []types.Statement{{Asset: base.FAKE_ETH_ADDRESS, AmountIn: *base.NewWeiStr("10")}}
				if !reflect.DeepEqual(sg.Statements, expected) {
					t.Errorf("statements for asset %s mismatch:\n got: %v\n want: %v", asset, sg.Statements, expected)
				}
			} else if asset == testAsset {
				if err == nil {
					t.Errorf("expected cache miss for asset %s, but found statements: %v", asset, sg.Statements)
				}
			}
		}
	})
}
