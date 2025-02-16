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
	accountFor   base.Address
	firstBlock   base.Blknum
	lastBlock    base.Blknum
	names        map[base.Address]types.Name
	testMode     bool
	asEther      bool
	noZero       bool
	reversed     bool
	useTraces    bool
	connection   *rpc.Connection
	assetFilter  []base.Address
	theTx        *types.Transaction
	appBalancers map[appBalancerKey]*appBalancer
}

// NewLedger returns a new empty Ledger struct
func NewLedger(conn *rpc.Connection, apps []types.Appearance, acctFor base.Address, fb, lb base.Blknum, asEther, testMode, noZero, useTraces, reversed bool, assetFilters *[]string) *Ledger {
	l := &Ledger{
		connection:   conn,
		accountFor:   acctFor,
		firstBlock:   fb,
		lastBlock:    lb,
		asEther:      asEther,
		testMode:     testMode,
		noZero:       noZero,
		reversed:     reversed,
		useTraces:    useTraces,
		appBalancers: make(map[appBalancerKey]*appBalancer),
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
	for index := 0; index < len(apps); index++ {
		curApp := apps[index]
		cur := base.Blknum(curApp.BlockNumber)
		prev := base.Blknum(apps[base.Max(1, index)-1].BlockNumber)
		next := base.Blknum(apps[base.Min(index+1, len(apps)-1)].BlockNumber)
		appKey := l.getAppBalancerKey(base.Blknum(curApp.BlockNumber), base.Txnum(curApp.TransactionIndex))
		l.appBalancers[appKey] = newAppBalancer(base.Blknum(prev), base.Blknum(cur), base.Blknum(next), index == 0, index == (len(apps)-1), l.reversed)
	}
	debugContexts(l.testMode, l.appBalancers)

	return l
}

// assetOfInterest returns true if the asset filter is empty or the asset matches
func assetOfInterest(filters []base.Address, needle base.Address) bool {
	if len(filters) == 0 {
		return true
	}

	for _, asset := range filters {
		if asset.Hex() == needle.Hex() {
			return true
		}
	}

	return false
}

func (l *Ledger) getAppBalancerKey(bn base.Blknum, txid base.Txnum) appBalancerKey {
	return appBalancerKey(fmt.Sprintf("%09d-%05d", bn, txid))
}

const maxTestingBlock = 17000000

func keyToString[K ~string | base.Address](k K) string {
	switch v := any(k).(type) {
	case appBalancerKey:
		return string(v)
	case base.Address:
		return v.Hex()
	default:
		logger.Fatal("keyToString: unknown type", v)
		return ""
	}
}

func debugContexts[K ~string | base.Address, T Balancer](testMode bool, ctxs map[K]T) {
	if !testMode {
		return
	}

	keys := make([]K, 0, len(ctxs))
	for key := range ctxs {
		keys = append(keys, key)
	}

	sort.Slice(keys, func(i, j int) bool {
		return keyToString(keys[i]) < keyToString(keys[j])
	})

	var whichLabel string
	var isAppearance bool
	if len(keys) > 0 {
		switch any(keys[0]).(type) {
		case appBalancerKey:
			whichLabel = "Appearance"
			isAppearance = true
		case base.Address:
			whichLabel = "Asset"
			isAppearance = false
		default:
			whichLabel = "Unknown"
			isAppearance = false
		}
	} else {
		whichLabel = "Contexts"
	}

	logger.Info(strings.Repeat("-", 60))
	logger.Info(fmt.Sprintf("%s Contexts (%d)", whichLabel, len(keys)))
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
		if isAppearance {
			logger.Info(fmt.Sprintf("%s: %10d %10d %11d%s", keyToString(key), c.Prev(), c.Cur(), c.Next(), msg))
		} else {
			logger.Info(fmt.Sprintf("%s: %10d", keyToString(key), c.Cur()))
		}
	}
}
