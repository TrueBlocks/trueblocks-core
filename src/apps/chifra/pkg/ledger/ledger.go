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

type ledgerContextKey string

// Ledger is a structure that carries enough information to complate a reconciliation
type Ledger struct {
	Chain              string
	AccountFor         base.Address
	FirstBlock         base.Blknum
	LastBlock          base.Blknum
	Names              map[base.Address]types.Name
	TestMode           bool
	Contexts           map[ledgerContextKey]*ledgerContext
	AsEther            bool
	NoZero             bool
	Reversed           bool
	UseTraces          bool
	Conn               *rpc.Connection
	assetFilter        []base.Address
	theTx              *types.Transaction
	localTokenBalances map[string]*base.Wei
}

// NewLedger returns a new empty Ledger struct
func NewLedger(conn *rpc.Connection, apps []types.Appearance, acctFor base.Address, fb, lb base.Blknum, asEther, testMode, noZero, useTraces, reversed bool, assetFilters *[]string) *Ledger {
	l := &Ledger{
		Conn:       conn,
		AccountFor: acctFor,
		FirstBlock: fb,
		LastBlock:  lb,
		Contexts:   make(map[ledgerContextKey]*ledgerContext),
		AsEther:    asEther,
		TestMode:   testMode,
		NoZero:     noZero,
		Reversed:   reversed,
		UseTraces:  useTraces,
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

	l.localTokenBalances = make(map[string]*base.Wei)
	l.setContexts(apps)

	return l
}

func (l *Ledger) localTokenKey(block base.Blknum, token base.Address) string {
	return fmt.Sprintf("%d-%s", block, token.Hex())
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

// func (l *Ledger) getOrCreateContext(bn base.Blknum, txid base.Txnum, assetAddr base.Address) *ledgerContext {
// 	key := l.ctxKey(bn, txid, assetAddr)
//
// 	if ctx, exists := l.Contexts[key]; exists {
// 		return ctx
// 	}
//
// 	// Create and store the context on-demand
// 	ctx := newLedgerContext(bn, bn, bn, false, false, l.Reversed)
// 	l.Contexts[key] = ctx
//
// 	return ctx
// }

func (l *Ledger) ctxKey(bn base.Blknum, txid base.Txnum, assetAddr base.Address) ledgerContextKey {
	_ = assetAddr
	return ledgerContextKey(fmt.Sprintf("%09d-%05d", bn, txid))
}

// setContexts visits the list of appearances and notes the block numbers of the next and previous
// appearance's and if they are the same or different. Because balances are only available per block,
// we must know this information to be able to calculate the correct post-tx balance.
func (l *Ledger) setContexts(apps []types.Appearance) error {
	for index := 0; index < len(apps); index++ {
		curApp := apps[index]

		cur := base.Blknum(curApp.BlockNumber)
		prev := base.Blknum(apps[base.Max(1, index)-1].BlockNumber)
		next := base.Blknum(apps[base.Min(index+1, len(apps)-1)].BlockNumber)
		key := l.ctxKey(base.Blknum(curApp.BlockNumber), base.Txnum(curApp.TransactionIndex), base.ZeroAddr)

		l.Contexts[key] = newLedgerContext(base.Blknum(prev), base.Blknum(cur), base.Blknum(next), index == 0, index == (len(apps)-1), l.Reversed)
	}

	l.debugContext()
	return nil
}

const maxTestingBlock = 17000000

func (l *Ledger) debugContext() {
	if !l.TestMode {
		return
	}

	keys := make([]ledgerContextKey, 0, len(l.Contexts))
	for key := range l.Contexts {
		keys = append(keys, key)
	}

	sort.Slice(keys, func(i, j int) bool {
		return string(keys[i]) < string(keys[j])
	})

	logger.Info(strings.Repeat("-", 60))
	logger.Info(fmt.Sprintf("Contexts (%d)", len(keys)))
	for _, key := range keys {
		c := l.Contexts[key]
		if c.CurBlock > maxTestingBlock {
			continue
		}
		msg := ""
		rr := c.ReconType &^ (types.First | types.Last)
		switch rr {
		case types.Genesis:
			msg = fmt.Sprintf(" %s", c.ReconType.String()+"-diff")
		case types.DiffDiff:
			msg = fmt.Sprintf(" %s", c.ReconType.String())
		case types.SameSame:
			msg = fmt.Sprintf(" %s", c.ReconType.String())
		case types.DiffSame:
			msg = fmt.Sprintf(" %s", c.ReconType.String())
		case types.SameDiff:
			msg = fmt.Sprintf(" %s", c.ReconType.String())
		default:
			msg = fmt.Sprintf(" %s should not happen!", c.ReconType.String())
		}
		logger.Info(fmt.Sprintf("%s: % 10d % 10d % 11d%s", key, c.PrevBlock, c.CurBlock, c.NextBlock, msg))
	}
}
