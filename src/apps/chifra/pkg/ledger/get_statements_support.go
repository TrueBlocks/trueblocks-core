package ledger

import (
	"fmt"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func ReportProgress(stmt *types.Statement, warn bool) {
	msg := fmt.Sprintf("Ether statement at % 9d.%d.%d %s %s", stmt.BlockNumber, stmt.TransactionIndex, stmt.LogIndex, stmt.Asset.Hex(), stmt.Holder.Hex())
	if !stmt.IsEth() {
		msg = fmt.Sprintf("Token statement at % 9d.%d.%d %s %s", stmt.BlockNumber, stmt.TransactionIndex, stmt.LogIndex, stmt.Asset.Hex(), stmt.Holder.Hex())
	}
	spacer := strings.Repeat(" ", 100-base.Min(100, len(msg)))
	if !stmt.Reconciled() {
		// logger.Progress(true, colors.Green+msg+" reconciled.", colors.Off, spacer)
		// } else {
		if warn {
			logger.Warn(msg+" did not reconcile.", spacer)
		}
	}
}

func debugHeader(a *types.AppNode[types.Transaction]) {
	logger.TestLog(true, "")
	logger.TestLog(true, "------------------------------------")
	logger.TestLog(true, fmt.Sprintf("~~~ Entering: %d.%d ~~~", a.CurBlock(), a.CurTxId()))
	// TODO: Better reporting?
	// logger.TestLog(true, "")
	// logger.TestLog(true, "------------------------------------")
	// // logger.TestLog(true, fmt.Sprintf("~~~ Entering: %d.%d ~~~", a.CurBlock(), a.CurTxId()))
	// logger.TestLog(true, "         prev        cur         next        isHead  isTail  isFirst isLast")
	// logger.TestLog(true, a.String())
	// logger.TestLog(true, "------------------------------------")
}

func (r *Reconciler) SkipAirdrop(addr base.Address) bool {
	_ = addr
	return false
	// if name, found := r.Names[addr]; !found {
	// 	return false
	// } else {
	// 	return name.IsAirdrop()
	// }
}
