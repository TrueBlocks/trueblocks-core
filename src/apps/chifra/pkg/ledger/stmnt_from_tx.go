package ledger

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/filter"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// GetStatements returns a statement from a given transaction
func (l *Ledger) GetStatements(conn *rpc.Connection, filter *filter.AppearanceFilter, trans *types.SimpleTransaction) ([]types.SimpleStatement, error) {
	// We need this below...
	l.theTx = trans

	if false && conn.StoreReadable() {
		statementGroup := &types.SimpleStatementGroup{
			Address:          l.AccountFor,
			BlockNumber:      trans.BlockNumber,
			TransactionIndex: trans.TransactionIndex,
		}
		if err := conn.Store.Read(statementGroup, nil); err == nil {
			return statementGroup.Statements, nil
		}
	}

	// make room for our results
	statements := make([]types.SimpleStatement, 0, 20) // a high estimate of the number of statements we'll need

	key := l.ctxKey(trans.BlockNumber, trans.TransactionIndex)
	ctx := l.Contexts[key]

	if l.assetOfInterest(base.FAKE_ETH_ADDRESS) {
		// TODO: We ignore errors in the next few lines, but we should not
		// TODO: BOGUS PERF - This greatly increases the number of times we call into eth_getBalance which is quite slow
		prevBal, _ := conn.GetBalanceAt(l.AccountFor, ctx.PrevBlock)
		if trans.BlockNumber == 0 {
			prevBal = new(base.Wei)
		}
		begBal, _ := conn.GetBalanceAt(l.AccountFor, ctx.CurBlock-1)
		endBal, _ := conn.GetBalanceAt(l.AccountFor, ctx.CurBlock)

		ret := types.SimpleStatement{
			AccountedFor:     l.AccountFor,
			Sender:           trans.From,
			Recipient:        trans.To,
			BlockNumber:      trans.BlockNumber,
			TransactionIndex: trans.TransactionIndex,
			TransactionHash:  trans.Hash,
			LogIndex:         0,
			Timestamp:        trans.Timestamp,
			AssetAddr:        base.FAKE_ETH_ADDRESS,
			AssetSymbol:      "WEI",
			Decimals:         18,
			SpotPrice:        0.0,
			PriceSource:      "not-priced",
			PrevBal:          *prevBal,
			BegBal:           *begBal,
			EndBal:           *endBal,
			ReconType:        ctx.ReconType,
		}

		if trans.To.IsZero() && trans.Receipt != nil && !trans.Receipt.ContractAddress.IsZero() {
			ret.Recipient = trans.Receipt.ContractAddress
		}

		// Do not collapse. A single transaction may have many movements of money
		if l.AccountFor == ret.Sender {
			gasUsed := new(base.Wei)
			if trans.Receipt != nil {
				gasUsed.SetUint64(trans.Receipt.GasUsed)
			}
			gasPrice := new(base.Wei).SetUint64(trans.GasPrice)
			gasOut := new(base.Wei).Mul(gasUsed, gasPrice)

			ret.AmountOut = (base.Wei)(trans.Value)
			ret.GasOut = *gasOut
		}

		// Do not collapse. A single transaction may have many movements of money
		if l.AccountFor == ret.Recipient {
			if ret.BlockNumber == 0 {
				ret.PrefundIn = (base.Wei)(trans.Value)
			} else {
				if trans.Rewards != nil {
					ret.MinerBaseRewardIn = trans.Rewards.Block
					ret.MinerNephewRewardIn = trans.Rewards.Nephew
					ret.MinerTxFeeIn = trans.Rewards.TxFee
					ret.MinerUncleRewardIn = trans.Rewards.Uncle
				} else {
					ret.AmountIn = (base.Wei)(trans.Value)
				}
				// TODO: BOGUS PERF - WHAT ABOUT WITHDRAWALS?
			}
		}

		if l.AsEther {
			ret.AssetSymbol = "ETH"
		}

		if !l.UseTraces && l.trialBalance("eth", &ret) {
			if ret.IsMaterial() {
				statements = append(statements, ret)
			} else {
				logger.TestLog(true, "Tx reconciled with a zero value net amount. It's okay.")
			}
		} else {
			if !l.UseTraces {
				logger.TestLog(!l.UseTraces, "Trial balance failed for ", ret.TransactionHash.Hex(), "need to decend into traces")
			}
			if traceStatements, err := l.getStatementsFromTraces(conn, trans, &ret); err != nil {
				logger.Warn(colors.Yellow+"Statement at ", fmt.Sprintf("%d.%d", trans.BlockNumber, trans.TransactionIndex), " does not reconcile."+colors.Off)
			} else {
				statements = append(statements, traceStatements...)
			}
		}
	}

	if receiptStatements, err := l.getStatementsFromReceipt(conn, filter, trans.Receipt); err != nil {
		logger.Warn(l.TestMode, "Error getting statement from receipt")
	} else {
		statements = append(statements, receiptStatements...)
	}

	if false && l.Conn.StoreWritable() && l.Conn.EnabledMap["statements"] && base.IsFinal(l.Conn.LatestBlockTimestamp, trans.Timestamp) {
		statementGroup := &types.SimpleStatementGroup{
			Address:          l.AccountFor,
			BlockNumber:      trans.BlockNumber,
			TransactionIndex: trans.TransactionIndex,
			Statements:       statements,
		}
		_ = conn.Store.Write(statementGroup, nil)
	}

	return statements, nil
}
