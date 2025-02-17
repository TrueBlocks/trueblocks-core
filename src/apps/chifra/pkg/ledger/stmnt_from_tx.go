package ledger

import (
	"fmt"
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/filter"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/ledger2"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/walk"
)

// GetStatements returns a statement from a given transaction
func (l *Ledger) GetStatements(prev, next base.Blknum, filter *filter.AppearanceFilter, trans *types.Transaction) ([]types.Statement, error) {
	if os.Getenv("NEW_CODE") == "true" {
		r := ledger2.NewReconciler(l.connection, l.accountFor, l.names, l.asEther)
		return r.GetStatements(prev, next, filter, trans)

	} else {
		// We need this below...
		l.theTx = trans

		if l.connection.StoreReadable() {
			statementGroup := &types.StatementGroup{
				BlockNumber:      trans.BlockNumber,
				TransactionIndex: trans.TransactionIndex,
				Address:          l.accountFor,
			}
			if err := l.connection.Store.Read(statementGroup, nil); err == nil {
				return statementGroup.Statements, nil
			}
		}

		// make room for our results
		statements := make([]types.Statement, 0, 20) // a high estimate of the number of statements we'll need

		key := l.getAppBalancerKey(trans.BlockNumber, trans.TransactionIndex)
		var ctx *appBalancer
		var exists bool
		if ctx, exists = l.appBalancers[key]; !exists {
			debugContexts(l.testMode, l.appBalancers)
			return statements, fmt.Errorf("no context for %s", key)
		}

		if assetOfInterest(l.assetFilter, base.FAKE_ETH_ADDRESS) {
			// TODO: We ignore errors in the next few lines, but we should not
			prevBal, _ := l.connection.GetBalanceAt(l.accountFor, ctx.Prev())
			if trans.BlockNumber == 0 {
				prevBal = new(base.Wei)
			}
			begBal, _ := l.connection.GetBalanceAt(l.accountFor, ctx.Cur()-1)
			if trans.BlockNumber == 0 {
				begBal = new(base.Wei)
			}
			endBal, _ := l.connection.GetBalanceAt(l.accountFor, ctx.Cur())

			ret := types.Statement{
				AccountedFor:     l.accountFor,
				Sender:           trans.From,
				Recipient:        trans.To,
				BlockNumber:      trans.BlockNumber,
				TransactionIndex: trans.TransactionIndex,
				TransactionHash:  trans.Hash,
				LogIndex:         0,
				Timestamp:        trans.Timestamp,
				AssetAddress:     base.FAKE_ETH_ADDRESS,
				AssetSymbol:      "WEI",
				Decimals:         18,
				SpotPrice:        0.0,
				PriceSource:      "not-priced",
				PrevBal:          *prevBal,
				BegBal:           *begBal,
				EndBal:           *endBal,
				BlockNumberPrev:  ctx.Prev(),
				BlockNumberNext:  ctx.Next(),
			}
			ret.PostFirst = ctx.first
			ret.PostLast = ctx.last

			if trans.To.IsZero() && trans.Receipt != nil && !trans.Receipt.ContractAddress.IsZero() {
				ret.Recipient = trans.Receipt.ContractAddress
			}

			// Do not collapse. A single transaction may have many movements of money
			if l.accountFor == ret.Sender {
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
			if l.accountFor == ret.Recipient {
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

			if l.asEther {
				ret.AssetSymbol = "ETH"
			}

			if !l.useTraces && l.trialBalance(prev, next, types.TrialBalEth, &ret) {
				if ret.IsMaterial() {
					statements = append(statements, ret)
				} else {
					logger.TestLog(true, "Tx reconciled with a zero value net amount. It's okay.")
				}
			} else {
				if !l.useTraces {
					logger.TestLog(!l.useTraces, "Trial balance failed for ", ret.TransactionHash.Hex(), " need to decend into traces")
				}
				if traceStatements, err := l.getStatementsFromTraces(prev, next, trans, &ret); err != nil {
					if !utils.IsFuzzing() {
						logger.Warn(colors.Yellow+"Statement at ", fmt.Sprintf("%d.%d", trans.BlockNumber, trans.TransactionIndex), " does not reconcile."+colors.Off)
					}
				} else {
					statements = append(statements, traceStatements...)
				}
			}
		}

		if receiptStatements, err := l.getStatementsFromReceipt(prev, next, filter, trans.Receipt); err != nil {
			logger.Warn("Error getting statement from receipt", err)
		} else {
			statements = append(statements, receiptStatements...)
		}

		isFinal := base.IsFinal(l.connection.LatestBlockTimestamp, trans.Timestamp)
		isWritable := l.connection.StoreWritable()
		isEnabled := l.connection.EnabledMap[walk.Cache_Statements]
		// TODO: BOGUS Turn on caching for statements once we get 100% coverage
		if false && isFinal && isWritable && isEnabled {
			for _, statement := range statements {
				if statement.IsMaterial() && !statement.Reconciled() {
					return statements, nil
				}
			}
			statementGroup := &types.StatementGroup{
				BlockNumber:      trans.BlockNumber,
				TransactionIndex: trans.TransactionIndex,
				Address:          l.accountFor,
				Statements:       statements,
			}
			_ = l.connection.Store.Write(statementGroup, nil)
		}

		return statements, nil
	}
}
