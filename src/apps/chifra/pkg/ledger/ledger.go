package ledger

import (
	"fmt"
	"sort"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/names"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// TODO: Two things to note. (1) if balances were part of this structure, we could fill those
// TODO: balances in a concurrent way before spinning through the appearances. And (2) if we did that
// TODO: prior to doing the accounting, we could easily travers in reverse order.

// Ledger represents the ledger state and provides methods to process and reconcile
// transactions and their associated logs. It holds configuration details such as the
// account being tracked, block ranges for processing, connection to an RPC endpoint,
// asset filters, and maps for both application-level and asset-level contexts.
type Ledger struct {
	accountFor  base.Address
	firstBlock  base.Blknum
	lastBlock   base.Blknum
	names       map[base.Address]types.Name
	testMode    bool
	asEther     bool
	noZero      bool
	reversed    bool
	useTraces   bool
	connection  *rpc.Connection
	assetFilter []base.Address
	theTx       *types.Transaction
	appContexts map[appContextKey]*appContext
}

// NewLedger returns a new empty Ledger struct
func NewLedger(conn *rpc.Connection, apps []types.Appearance, acctFor base.Address, fb, lb base.Blknum, asEther, testMode, noZero, useTraces, reversed bool, assetFilters *[]string) *Ledger {
	l := &Ledger{
		connection:  conn,
		accountFor:  acctFor,
		firstBlock:  fb,
		lastBlock:   lb,
		asEther:     asEther,
		testMode:    testMode,
		noZero:      noZero,
		reversed:    reversed,
		useTraces:   useTraces,
		appContexts: make(map[appContextKey]*appContext),
	}

	if assetFilters != nil {
		l.assetFilter = make([]base.Address, len(*assetFilters))
		for i, addr := range *assetFilters {
			l.assetFilter[i] = base.HexToAddress(addr)
		}
	} else {
		l.assetFilter = []base.Address{}
	}

	parts := types.Custom | types.Prefund | types.Regular
	l.names, _ = names.LoadNamesMap(conn.Chain, parts, []string{})
	if err := l.setContexts(apps); err != nil {
		fmt.Println("Error setting contexts:", err)
	}
	debugLedgerContexts(l.testMode, l.appContexts)

	return l
}

// assetOfInterest returns true if the asset filter is empty or the asset matches
func (l *Ledger) assetOfInterest(needle base.Address) bool {
	if len(l.assetFilter) == 0 {
		return true
	}

	for _, asset := range l.assetFilter {
		if asset.Hex() == needle.Hex() {
			return true
		}
	}

	return false
}

func (l *Ledger) getAppContextKey(bn base.Blknum, txid base.Txnum) appContextKey {
	return appContextKey(fmt.Sprintf("%09d-%05d", bn, txid))
}

func (l *Ledger) setContexts(apps []types.Appearance) error {
	for i := 0; i < len(apps); i++ {
		cur := base.Blknum(apps[i].BlockNumber)
		prev := base.Blknum(apps[base.Max(1, i)-1].BlockNumber)
		next := base.Blknum(apps[base.Min(i+1, len(apps)-1)].BlockNumber)
		key := l.getAppContextKey(base.Blknum(apps[i].BlockNumber), base.Txnum(apps[i].TransactionIndex))
		l.appContexts[key] = newAppContext(base.Blknum(prev), base.Blknum(cur), base.Blknum(next), i == 0, i == (len(apps)-1), l.reversed)
	}
	return nil
}

const maxTestingBlock = 17000000

func debugLedgerContexts[K ~string, T types.LedgerContexter](testMode bool, ctxs map[K]T) {
	if !testMode {
		return
	}

	keys := make([]K, 0, len(ctxs))
	for key := range ctxs {
		keys = append(keys, key)
	}

	sort.Slice(keys, func(i, j int) bool {
		return string(keys[i]) < string(keys[j])
	})

	logger.Info(strings.Repeat("-", 60))
	logger.Info(fmt.Sprintf("Contexts (%d)", len(keys)))
	for _, key := range keys {
		c := ctxs[key]
		if c.Cur() > maxTestingBlock {
			continue
		}
		msg := ""
		rr := c.Recon() &^ (types.First | types.Last)
		switch rr {
		case types.Genesis:
			msg = fmt.Sprintf(" %s", c.Recon().String()+"-diff")
		case types.DiffDiff:
			msg = fmt.Sprintf(" %s", c.Recon().String())
		case types.SameSame:
			msg = fmt.Sprintf(" %s", c.Recon().String())
		case types.DiffSame:
			msg = fmt.Sprintf(" %s", c.Recon().String())
		case types.SameDiff:
			msg = fmt.Sprintf(" %s", c.Recon().String())
		default:
			msg = fmt.Sprintf(" %s should not happen!", c.Recon().String())
		}
		logger.Info(fmt.Sprintf("%s: % 10d % 10d % 11d%s", key, c.Prev(), c.Cur(), c.Next(), msg))
	}
}
