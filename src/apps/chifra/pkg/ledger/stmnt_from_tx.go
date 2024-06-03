package ledger

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/filter"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/walk"
)

// GetStatements returns a statement from a given transaction
func (l *Ledger) GetStatements(conn *rpc.Connection, filter *filter.AppearanceFilter, trans *types.Transaction) ([]types.Statement, error) {
	// We need this below...
	l.theTx = trans

	if false && conn.StoreReadable() {
		statementGroup := &types.StatementGroup{
			Address:          l.AccountFor,
			BlockNumber:      trans.BlockNumber,
			TransactionIndex: trans.TransactionIndex,
		}
		if err := conn.Store.Read(statementGroup, nil); err == nil {
			return statementGroup.Statements, nil
		}
	}

	// make room for our results
	statements := make([]types.Statement, 0, 20) // a high estimate of the number of statements we'll need

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

		ret := types.Statement{
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
				gasUsed.SetUint64(uint64(trans.Receipt.GasUsed))
			}
			gasPrice := new(base.Wei).SetUint64(uint64(trans.GasPrice))
			gasOut := new(base.Wei).Mul(gasUsed, gasPrice)

			ret.AmountOut = trans.Value
			ret.GasOut = *gasOut
		}

		// Do not collapse. A single transaction may have many movements of money
		if l.AccountFor == ret.Recipient {
			if ret.BlockNumber == 0 {
				ret.PrefundIn = trans.Value
			} else {
				if trans.Rewards != nil {
					ret.MinerBaseRewardIn = trans.Rewards.Block
					ret.MinerNephewRewardIn = trans.Rewards.Nephew
					ret.MinerTxFeeIn = trans.Rewards.TxFee
					ret.MinerUncleRewardIn = trans.Rewards.Uncle
				} else {
					ret.AmountIn = trans.Value
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
				if !utils.IsFuzzing() {
					logger.Warn(colors.Yellow+"Statement at ", fmt.Sprintf("%d.%d", trans.BlockNumber, trans.TransactionIndex), " does not reconcile."+colors.Off)
				}
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

	isFinal := base.IsFinal(conn.LatestBlockTimestamp, trans.Timestamp)
	if false && isFinal && conn.StoreWritable() && conn.EnabledMap[walk.Cache_Statements] {
		statementGroup := &types.StatementGroup{
			Address:          l.AccountFor,
			BlockNumber:      trans.BlockNumber,
			TransactionIndex: trans.TransactionIndex,
			Statements:       statements,
		}
		_ = conn.Store.Write(statementGroup, nil)
	}

	return statements, nil
}
