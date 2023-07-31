package ledger

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/names"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// TODO: Two things to note. (1) if balances were part of this structure, we could fill those
// TODO: balances in a concurrent way before spinning through the appearances. And (2) if we did that
// TODO: prior to doing the accounting, we could easily travers in reverse order.

// Ledger is a structure that carries enough information to complate a reconciliation
type Ledger struct {
	Chain       string
	AccountFor  base.Address
	FirstBlock  base.Blknum
	LastBlock   base.Blknum
	Names       map[base.Address]types.SimpleName
	TestMode    bool
	Contexts    map[string]LedgerContext
	AsEther     bool
	NoZero      bool
	UseTraces   bool
	AssetFilter *[]base.Address
	Tx          *types.SimpleTransaction
	RpcOptions  *rpcClient.Options
}

// NewLedger returns a new empty Ledger struct
func NewLedger(chain string, acctFor base.Address, fb, lb base.Blknum, asEther, testMode, noZero, useTraces bool, assetFilters *[]string) *Ledger {
	l := &Ledger{
		Chain:      chain,
		AccountFor: acctFor,
		FirstBlock: fb,
		LastBlock:  lb,
		Contexts:   make(map[string]LedgerContext),
		AsEther:    asEther,
		TestMode:   testMode,
		NoZero:     noZero,
		UseTraces:  useTraces,
	}
	if assetFilters != nil {
		assets := make([]base.Address, 0, len(*assetFilters))
		for _, addr := range *assetFilters {
			assets = append(assets, base.HexToAddress(addr))
		}
		l.AssetFilter = &assets
	}

	parts := names.Custom | names.Prefund | names.Regular
	l.Names, _ = names.LoadNamesMap(chain, parts, []string{})

	return l
}

// AssetOfInterest returns true if the asset filter is empty or the asset matches
func (l *Ledger) AssetOfInterest(needle base.Address) bool {
	if l.AssetFilter == nil || len(*l.AssetFilter) == 0 {
		return true
	}

	for _, asset := range *l.AssetFilter {
		if asset.Hex() == needle.Hex() {
			return true
		}
	}

	return false
}

func Report(r *types.SimpleStatement, ctx LedgerContext, msg string) {
	logger.TestLog(true, "===================================================")
	logger.TestLog(true, fmt.Sprintf("====> %s", msg))
	logger.TestLog(true, "===================================================")
	logger.TestLog(true, "ledger.blockNumber:    ", ctx.PrevBlock)
	logger.TestLog(true, "prevBlock:             ", ctx.PrevBlock)
	logger.TestLog(true, "transfer.blockNumber:  ", r.BlockNumber)
	logger.TestLog(true, "nextBlock:             ", ctx.NextBlock)
	logger.TestLog(true, "isPrevDiff:            ", ctx.PrevBlock != r.BlockNumber)
	logger.TestLog(true, "isNextDiff:            ", ctx.NextBlock != r.BlockNumber)
	logger.TestLog(true, "---------------------------------------------------")
	logger.TestLog(true, "Trial balance:")
	logger.TestLog(true, "   reconciliationType: ", r.ReconciliationType)
	logger.TestLog(true, "   accountedFor:       ", r.AccountedFor)
	logger.TestLog(true, "   sender:             ", r.Sender)
	logger.TestLog(true, "   recipient:          ", r.Recipient)
	logger.TestLog(true, "   assetAddr:          ", r.AssetAddr)
	logger.TestLog(true, "   assetSymbol:        ", r.AssetSymbol)
	logger.TestLog(true, "   decimals:           ", r.Decimals)
	logger.TestLog(true, "   prevAppBlk:         ", r.PrevAppBlk)
	logger.TestLog(true, "   hash:               ", r.TransactionHash)
	logger.TestLog(true, "   timestamp:          ", r.Timestamp)
	logger.TestLog(true, "   blockNumber:        ", r.BlockNumber)
	logger.TestLog(true, "   transactionIndex:   ", r.TransactionIndex)
	logger.TestLog(true, "   logIndex:           ", r.LogIndex)
	logger.TestLog(true, "   priceSource:        ", r.PriceSource)
	logger.TestLog(true, "   spotPrice:          ", r.SpotPrice)
	logger.TestLog(true, "   prevBal:            ", r.PrevBal.Text(10))
	logger.TestLog(true, "   begBal:             ", r.BegBal.Text(10))
	logger.TestLog(true, "   amountIn:           ", r.AmountIn.Text(10))
	logger.TestLog(true, "   internalIn:         ", r.InternalIn.Text(10))
	logger.TestLog(true, "   minerBaseRewardIn:  ", r.MinerBaseRewardIn.Text(10))
	logger.TestLog(true, "   minerNephewRewardIn:", r.MinerNephewRewardIn.Text(10))
	logger.TestLog(true, "   minerTxFeeIn:       ", r.MinerTxFeeIn.Text(10))
	logger.TestLog(true, "   minerUncleRewardIn: ", r.MinerUncleRewardIn.Text(10))
	logger.TestLog(true, "   correctingIn:       ", r.CorrectingIn.Text(10))
	logger.TestLog(true, "   prefundIn:          ", r.PrefundIn.Text(10))
	logger.TestLog(true, "   selfDestructIn:     ", r.SelfDestructIn.Text(10))
	logger.TestLog(true, "   totalIn:            ", r.TotalIn().Text(10))
	logger.TestLog(true, "   amountOut:          ", r.AmountOut.Text(10))
	logger.TestLog(true, "   internalOut:        ", r.InternalOut.Text(10))
	logger.TestLog(true, "   correctingOut:      ", r.CorrectingOut.Text(10))
	logger.TestLog(true, "   selfDestructOut:    ", r.SelfDestructOut.Text(10))
	logger.TestLog(true, "   gasOut:             ", r.GasOut.Text(10))
	logger.TestLog(true, "   totalOut:           ", r.TotalOut().Text(10))
	logger.TestLog(true, "   amountNet:          ", r.AmountNet().Text(10))
	logger.TestLog(true, "   endBal:             ", r.EndBal.Text(10))
	logger.TestLog(true, "   begBalDiff:         ", r.BegBalDiff().Text(10))
	logger.TestLog(true, "   endBalDiff:         ", r.EndBalDiff().Text(10))
	logger.TestLog(true, "   endBalCalc:         ", r.EndBalCalc().Text(10))
	logger.TestLog(true, "   correctingReason:   ", r.CorrectingReason)
	logger.TestLog(true, "   moneyMoved:         ", r.MoneyMoved())
	logger.TestLog(true, "   trialBalance:       ", r.Reconciled())
	logger.TestLog(true, "---------------------------------------------------")
}
