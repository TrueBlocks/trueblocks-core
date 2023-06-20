package ledger

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// Ledger is a struct that holds the context for each appearance and, for each asset encountered, the previous reconcilation record for balance accounting
type Ledger struct {
	Chain      string
	AccountFor base.Address
	TestMode   bool
	Previous   map[base.Address]*types.SimpleStatement
	Contexts   map[string]LedgerContext
	AsEther    bool
	NoZero     bool
	UseTraces  bool
}

// NewLedger returns a new empty Ledger struct
func NewLedger(chain string, acctFor base.Address, asEther, testMode, noZero, useTraces bool) *Ledger {
	return &Ledger{
		Chain:      chain,
		AccountFor: acctFor,
		Previous:   make(map[base.Address]*types.SimpleStatement),
		Contexts:   make(map[string]LedgerContext),
		AsEther:    asEther,
		TestMode:   testMode,
		NoZero:     noZero,
		UseTraces:  useTraces,
	}
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
	logger.TestLog(true, "   prefundIn:          ", r.PrefundIn.Text(10))
	logger.TestLog(true, "   selfDestructIn:     ", r.SelfDestructIn.Text(10))
	logger.TestLog(true, "   totalIn:            ", r.TotalIn().Text(10))
	logger.TestLog(true, "   amountOut:          ", r.AmountOut.Text(10))
	logger.TestLog(true, "   internalOut:        ", r.InternalOut.Text(10))
	logger.TestLog(true, "   selfDestructOut:    ", r.SelfDestructOut.Text(10))
	logger.TestLog(true, "   gasOut:             ", r.GasOut.Text(10))
	logger.TestLog(true, "   totalOut:           ", r.TotalOut().Text(10))
	logger.TestLog(true, "   amountNet:          ", r.AmountNet().Text(10))
	logger.TestLog(true, "   endBal:             ", r.EndBal.Text(10))
	logger.TestLog(true, "   begBalDiff:         ", r.BegBalDiff().Text(10))
	logger.TestLog(true, "   endBalDiff:         ", r.EndBalDiff().Text(10))
	logger.TestLog(true, "   endBalCalc:         ", r.EndBalCalc().Text(10))
	logger.TestLog(true, "   trialBalance:       ", r.Reconciled())
	// Encoding            string         `json:"encoding"`
	// Signature           string         `json:"signature"`
	logger.TestLog(true, "---------------------------------------------------")
}
