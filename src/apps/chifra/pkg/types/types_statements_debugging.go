package types

import (
	"fmt"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

func (s *Statement) DebugStatement(node *AppNode[Transaction]) {
	reportE := func(msg string, val *base.Wei) {
		isZero := func(val *base.Wei) bool {
			return val.Cmp(base.NewWei(0)) == 0
		}
		logger.TestLog(!isZero(val), msg, val.ToFloatString(18))
	}

	report2 := func(msg string, v1 *base.Wei, v2 *base.Wei) {
		s := ""
		if v1 != nil {
			s = v1.ToFloatString(18)
		}
		if v2 != nil {
			s += " (" + v2.ToFloatString(18) + ")"
		}
		logger.TestLog(true, msg, s)
	}

	reportL := func(msg string) {
		report2(msg, nil, nil)
	}

	report1 := func(msg string, val *base.Wei) {
		report2(msg, val, nil)
	}

	logger.TestLog(true, "===================================================")
	logger.TestLog(true, "Previous:              ", node.PrevBlock(), node.PrevTxId(), "(", node.IsFirstInBlock(), ")")
	logger.TestLog(true, "Current:               ", node.CurBlock(), node.CurTxId(), fmt.Sprintf("( %t )", node.CurBlock() == s.BlockNumber && node.CurTxId() == s.TransactionIndex))
	logger.TestLog(true, "Next:                  ", node.NextBlock(), node.NextTxId(), "(", node.IsLastInBlock(), ")")
	// logger.TestLog(true, "First, Last:           ", fmt.Sprintf("(%t, %t)", node.First(), node.Last()))
	logger.TestLog(true, "accountedFor:          ", s.AccountedFor)
	logger.TestLog(true, "sender:                ", s.Sender, " ==> ", s.Recipient)
	logger.TestLog(true, "asset:                 ", s.Asset, "("+s.Symbol+")", fmt.Sprintf("decimals: %d", s.Decimals))
	logger.TestLog(true, "hash:                  ", s.TransactionHash)
	logger.TestLog(true, "timestamp:             ", s.Timestamp)
	logger.TestLog(true, fmt.Sprintf("blockNumber:            %d.%d.%d", s.BlockNumber, s.TransactionIndex, s.LogIndex))
	logger.TestLog(true, "priceSource:           ", s.SpotPrice.String(), "("+s.PriceSource+")")
	reportL("---------------------------------------------------")
	logger.TestLog(true, "Trial balance:")
	report1("   prevBal:            ", &s.PrevBal)
	report2("   begBal:             ", &s.BegBal, s.EndBalDiff())
	report1("   totalIn:            ", s.TotalIn())
	report1("   totalOut:           ", s.TotalOut())
	report1("   amountNet:          ", s.AmountNet())
	reportL("                       =======================")
	report2("   endBal:             ", &s.EndBal, s.BegBalDiff())
	report1("   endBalCalc:         ", s.EndBalCalc())
	reportL("---------------------------------------------------")
	reportE("   amountIn:           ", &s.AmountIn)
	reportE("   internalIn:         ", &s.InternalIn)
	reportE("   minerBaseRewardIn:  ", &s.MinerBaseRewardIn)
	reportE("   minerNephewRewardIn:", &s.MinerNephewRewardIn)
	reportE("   minerTxFeeIn:       ", &s.MinerTxFeeIn)
	reportE("   minerUncleRewardIn: ", &s.MinerUncleRewardIn)
	reportE("   correctBegBalIn:    ", &s.CorrectBegBalIn)
	reportE("   correctAmountIn:    ", &s.CorrectAmountIn)
	reportE("   correctEndBalIn:    ", &s.CorrectEndBalIn)
	reportE("   prefundIn:          ", &s.PrefundIn)
	reportE("   selfDestructIn:     ", &s.SelfDestructIn)
	reportE("   amountOut:          ", &s.AmountOut)
	reportE("   internalOut:        ", &s.InternalOut)
	reportE("   correctBegBalOut:   ", &s.CorrectBegBalOut)
	reportE("   correctAmountOut:   ", &s.CorrectAmountOut)
	reportE("   correctEndBalOut:   ", &s.CorrectEndBalOut)
	reportE("   selfDestructOut:    ", &s.SelfDestructOut)
	reportE("   gasOut:             ", &s.GasOut)
	logger.TestLog(len(s.CorrectingReasons) > 0, "   correctingReasons:  ", strings.Trim(s.CorrectingReasons, ","))
	logger.TestLog(true, "   material:           ", s.IsMaterial())
	logger.TestLog(true, "   reconciled:         ", s.Reconciled())
	if !s.Reconciled() {
		logger.TestLog(true, " ^^ we need to fix this ^^")
	}
	logger.TestLog(true, "---------------------------------------------------")
}
