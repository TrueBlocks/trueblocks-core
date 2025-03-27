package ledger

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/names"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// TODO: Two things to note. (1) if balances were part of this structure, we could fill those
// TODO: balances in a concurrent way before spinning through the appearances. And (2) if we did that
// TODO: prior to doing the accounting, we could easily travers in reverse order.

// Ledger is a structure that carries enough information to complete a reconciliation
type Ledger struct {
	Chain       string
	AccountFor  base.Address
	FirstBlock  base.Blknum
	LastBlock   base.Blknum
	Names       map[base.Address]types.Name
	TestMode    bool
	Contexts    map[ledgerContextKey]*ledgerContext
	AsEther     bool
	NoZero      bool
	Reversed    bool
	UseTraces   bool
	Conn        *rpc.Connection
	assetFilter []base.Address
	theTx       *types.Transaction
}

// NewLedger returns a new empty Ledger struct
func NewLedger(conn *rpc.Connection, acctFor base.Address, fb, lb base.Blknum, asEther, testMode, noZero, useTraces, reversed bool, assetFilters *[]string) *Ledger {
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

// See issue #2791 - This is the code that used to generate extra traces to make reconciliation work
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
