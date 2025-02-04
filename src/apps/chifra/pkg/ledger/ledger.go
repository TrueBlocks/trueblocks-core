/*
appContext
appContextKey
appContexts
appCtx
appKey
newAppContext
getAppContextKey

assetContext
assetContextKey
assetContexts
assetCtx
assetKey
newAssetContext
getAssetContextKey

getOrCreateAssetContext
setContexts
*/
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

type appContextKey string
type assetContextKey appContextKey

// Ledger is a structure that carries enough information to complate a reconciliation
type Ledger struct {
	Chain         string
	AccountFor    base.Address
	FirstBlock    base.Blknum
	LastBlock     base.Blknum
	Names         map[base.Address]types.Name
	TestMode      bool
	AsEther       bool
	NoZero        bool
	Reversed      bool
	UseTraces     bool
	Conn          *rpc.Connection
	assetFilter   []base.Address
	theTx         *types.Transaction
	appContexts   map[appContextKey]*appContext
	assetContexts map[assetContextKey]*assetContext
}

// NewLedger returns a new empty Ledger struct
func NewLedger(conn *rpc.Connection, apps []types.Appearance, acctFor base.Address, fb, lb base.Blknum, asEther, testMode, noZero, useTraces, reversed bool, assetFilters *[]string) *Ledger {
	l := &Ledger{
		Conn:          conn,
		AccountFor:    acctFor,
		FirstBlock:    fb,
		LastBlock:     lb,
		AsEther:       asEther,
		TestMode:      testMode,
		NoZero:        noZero,
		Reversed:      reversed,
		UseTraces:     useTraces,
		appContexts:   make(map[appContextKey]*appContext),
		assetContexts: make(map[assetContextKey]*assetContext),
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
	l.Names, _ = names.LoadNamesMap(conn.Chain, parts, []string{})

	for index := 0; index < len(apps); index++ {
		curApp := apps[index]
		cur := base.Blknum(curApp.BlockNumber)
		prev := base.Blknum(apps[base.Max(1, index)-1].BlockNumber)
		next := base.Blknum(apps[base.Min(index+1, len(apps)-1)].BlockNumber)
		appKey := l.getAppContextKey(base.Blknum(curApp.BlockNumber), base.Txnum(curApp.TransactionIndex))
		l.appContexts[appKey] = newAppContext(base.Blknum(prev), base.Blknum(cur), base.Blknum(next), index == 0, index == (len(apps)-1), l.Reversed)
	}
	debugLedgerContexts(l.TestMode, l.appContexts)

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

// See issue #2791 - This is the code that used to generate extra traces to make reconcilation work
// (or, at least, similar code in `chifra export` generated these traces.
// bool isSuicide = trace.action.selfDestructed != "";
// bool isCreation = trace.result.address != "";
// // do not colapse
// if (isCreation) {
//   displayAsTrace(opt, trace);
//   displayAsCreation(opt, trace);
// }
// // do not colapse
// if (isSuicide) {
//   displayAsSuicide(opt, trace);
// }
// // do not colapse
// if (!isCreation && !isSuicide) {
//   displayAsTrace(opt, trace);
// }
//--------------------------------------------------------------
// bool displayAsCreation(COptions* opt, const CTrace& trace) {
//     if (trace.result.address == "")
//         return false;
//     CTrace copy = trace;
//     copy.action.from = "0x0";
//     copy.action.to = trace.result.address;
//     copy.action.callType = "creation";
//     copy.action.value = trace.action.value;
//     if (copy.traceAddress.size() == 0)
//         copy.traceAddress.push_back("null");
//     copy.traceAddress.push_back("s");
//     copy.transactionHash = uint_2_Hex(trace.blockNumber * 100000 + trace.transactionIndex);
//     copy.action.input = trace.action.input;
//     return displayAsTrace(opt, copy);
// }
// //--------------------------------------------------------------
// bool displayAsSuicide(COptions* opt, const CTrace& trace) {
//     if (trace.action.refundAddress == "")
//         return false;
//     CTrace copy = trace;
//     copy.action.from = trace.action.selfDestructed;
//     copy.action.to = trace.action.refundAddress;
//     copy.action.callType = "suicide";
//     copy.action.value = trace.action.balance;
//     copy.traceAddress.push_back("s");
//     copy.transactionHash = uint_2_Hex(trace.blockNumber * 100000 + trace.transactionIndex);
//     copy.action.input = "0x";
//     return displayAsTrace(opt, copy);
// }
// //--------------------------------------------------------------
// bool displayAsTrace(COptions* opt, const CTrace& trace) {
//     bool isText = (expContext().exportFmt & (TXT1 | CSV1));
//     if (isText) {
//         cout << trim(trace.Format(expContext().fmtMap["format"]), '\t') << endl;
//     } else {
//         if (!opt->firstOut)
//             cout << ",";
//         cout << "  ";
//         indent();
//         trace.toJson(cout);
//         unindent();
//         opt->firstOut = false;
//     }
//     return true;
// }

func (l *Ledger) getAppContextKey(bn base.Blknum, txid base.Txnum) appContextKey {
	return appContextKey(fmt.Sprintf("%09d-%05d", bn, txid))
}

func (l *Ledger) getAssetContextKey(bn base.Blknum, txid base.Txnum, assetAddr base.Address) assetContextKey {
	return assetContextKey(fmt.Sprintf("%s-%09d-%05d", assetAddr.Hex(), bn, txid))
}

// getOrCreateAssetContext returns the asset-specific ledger context for the given block number, transaction index, and asset address.
// If it does not exist in the assetContexts map, it creates a new record based on the legacy context.
func (l *Ledger) getOrCreateAssetContext(bn base.Blknum, txid base.Txnum, assetAddr base.Address) *assetContext {
	assetKey := l.getAssetContextKey(bn, txid, assetAddr)
	if ctx, exists := l.assetContexts[assetKey]; exists {
		return ctx
	}

	appKey := l.getAppContextKey(bn, txid)
	appCtx, exists := l.appContexts[appKey]
	if !exists {
		logger.Warn("This should never happen in getOrCreateAssetContext")
		appCtx = newAppContext(bn, bn, bn, false, false, l.Reversed)
		l.appContexts[appKey] = appCtx
	}

	assetCtx := newAssetContext(appCtx.PrevBlock, appCtx.CurBlock, appCtx.NextBlock, false, false, l.Reversed, assetAddr)
	l.assetContexts[assetKey] = assetCtx
	return assetCtx
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
