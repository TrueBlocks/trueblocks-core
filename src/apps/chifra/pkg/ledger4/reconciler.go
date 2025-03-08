package ledger4

import (
	"container/list"
	"encoding/json"
	"sync"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/ledger1"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/ledger10"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/names"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/walk"
)

type Reconciler4 struct {
	opts              *ledger10.ReconcilerOptions
	names             map[base.Address]types.Name
	hasStartBlock     bool
	transfers         map[blockTxKey][]ledger10.AssetTransfer
	accountLedger     map[assetHolderKey]base.Wei
	ledgerAssets      map[base.Address]bool
	correctionCounter base.Value
	entryCounter      base.Value
	ledgers           map[base.Address]Ledger
}

func (r *Reconciler4) String() string {
	bytes, _ := json.MarshalIndent(r, "", "  ")
	return string(bytes)
}

func NewReconciler(opts *ledger10.ReconcilerOptions) *Reconciler4 {
	parts := types.Custom | types.Prefund | types.Regular
	names, _ := names.LoadNamesMap(opts.Connection.Chain, parts, []string{})
	r := &Reconciler4{
		opts:  opts,
		names: names,
	}
	_ = r.hasStartBlock
	_ = r.transfers
	_ = r.accountLedger
	_ = r.ledgerAssets
	_ = r.correctionCounter
	_ = r.entryCounter
	_ = r.ledgers
	return r
}

type Ledger struct{}

// type blockTxKey struct {
// 	BlockNumber      base.Blknum
// 	TransactionIndex base.Txnum
// }

// type assetHolderKey struct {
// 	Asset base.Address
// 	Holder       base.Address
// }

type Node struct {
	Appearance *types.Appearance
	Tx         *types.Transaction
	Transfers  []ledger10.AssetTransfer
	Statement  []types.Statement
	Reconciled bool
}

func BuildDoublyLinkedList(appearances []types.Appearance) *list.List {
	l := list.New()
	for _, app := range appearances {
		node := &Node{Appearance: &app}
		l.PushBack(node)
	}
	return l
}

func (r *Reconciler4) ProcessTransactionsConcurrently(l *list.List) {
	var wg sync.WaitGroup
	for e := l.Front(); e != nil; e = e.Next() {
		wg.Add(1)
		go func(elem *list.Element) {
			defer wg.Done()
			node := elem.Value.(*Node)
			node.Tx = fetchTx(node.Appearance, false)
			if node.Tx != nil {
				node.Transfers = r.createTransfers(node.Tx)
				node.Statement = r.queryBalances(node.Transfers)
			}
		}(e)
	}
	wg.Wait()
}

var conn *rpc.Connection = nil

func getConnection() *rpc.Connection {
	if conn == nil {
		conn = rpc.NewConnection("mainnet", false, map[walk.CacheType]bool{})
	}
	return conn
}

func (r *Reconciler4) createTransfers(tx *types.Transaction) []ledger10.AssetTransfer {
	r1 := ledger1.NewReconciler(r.opts)
	pos := &types.AppPosition{}
	transfers, err := r1.GetStatements1(pos, tx)
	if err != nil {
		return []ledger10.AssetTransfer{}
	}
	return transfers
}

func (r *Reconciler4) queryBalances(transfers []ledger10.AssetTransfer) []types.Statement {
	statements := make([]types.Statement, 0)
	for range transfers {
		s := types.Statement{
			BegBal: *base.NewWei(100),
			EndBal: *base.NewWei(100),
		}
		statements = append(statements, s)
	}
	return statements
}

type blockTxKey struct {
	BlockNumber      base.Blknum
	TransactionIndex base.Txnum
}

type assetHolderKey struct {
	Asset  base.Address
	Holder base.Address
}
