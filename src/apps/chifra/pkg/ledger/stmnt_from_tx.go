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
func (l *Ledger) GetStatements(pos *types.AppPosition, filter *filter.AppearanceFilter, trans *types.Transaction) ([]types.Statement, error) {
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

	var err error
	allStatements := make([]types.Statement, 0, 20)

	if os.Getenv("NEW_CODE") == "true" {
		r := ledger2.NewReconciler(l.connection, l.accountFor, l.names, l.asEther)
		if allStatements, err = r.GetStatements(pos, filter, trans); err != nil {
			return allStatements, err
		}

	} else {
		key := l.getAppBalancerKey(trans.BlockNumber, trans.TransactionIndex)
		var ctx *appBalancer
		var exists bool
		if ctx, exists = l.appBalancers[key]; !exists {
			debugContexts(l.testMode, l.appBalancers)
			return allStatements, fmt.Errorf("balancer not found for key %s", key)
		}

		if assetOfInterest(l.assetFilter, base.FAKE_ETH_ADDRESS) {
			validatePosition(pos, ctx)

			prevBal, _ := l.connection.GetBalanceAt(l.accountFor, ctx.Prev())
			if trans.BlockNumber == 0 {
				prevBal = new(base.Wei)
			}

			begBal, _ := l.connection.GetBalanceAt(l.accountFor, ctx.Cur()-1)
			if trans.BlockNumber == 0 {
				begBal = new(base.Wei)
			}

			endBal, _ := l.connection.GetBalanceAt(l.accountFor, ctx.Cur())

			s := types.Statement{
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
				PostFirst:        ctx.first,
				PostLast:         ctx.last,
			}

			if trans.To.IsZero() && trans.Receipt != nil && !trans.Receipt.ContractAddress.IsZero() {
				s.Recipient = trans.Receipt.ContractAddress
			}

			// Do not collapse. A single transaction may have many movements of money
			if s.Sender == l.accountFor {
				gasUsed := new(base.Wei)
				if trans.Receipt != nil {
					gasUsed.SetUint64(uint64(trans.Receipt.GasUsed))
				}
				gasPrice := new(base.Wei).SetUint64(uint64(trans.GasPrice))
				gasOut := new(base.Wei).Mul(gasUsed, gasPrice)

				s.AmountOut = trans.Value
				s.GasOut = *gasOut
			}

			// Do not collapse. A single transaction may have many movements of money
			if s.Recipient == l.accountFor {
				if s.BlockNumber == 0 {
					s.PrefundIn = trans.Value
				} else {
					if trans.Rewards != nil {
						s.MinerBaseRewardIn = trans.Rewards.Block
						s.MinerNephewRewardIn = trans.Rewards.Nephew
						s.MinerTxFeeIn = trans.Rewards.TxFee
						s.MinerUncleRewardIn = trans.Rewards.Uncle
					} else {
						s.AmountIn = trans.Value
					}
					// TODO: BOGUS PERF - WHAT ABOUT WITHDRAWALS?
				}
			}

			if l.asEther {
				s.AssetSymbol = "ETH"
			}

			if !l.useTraces && l.trialBalance(pos, types.TrialBalEth, trans, &s) {
				if s.IsMaterial() {
					allStatements = append(allStatements, s)
				} else {
					logger.TestLog(true, "Tx reconciled with a zero value net amount. It's okay.")
				}
			} else {
				if !l.useTraces {
					logger.TestLog(!l.useTraces, "Trial balance failed for ", s.TransactionHash.Hex(), " need to decend into traces")
				}
				if traceStatements, err := l.getStatementsFromTraces(pos, trans, &s); err != nil {
					if !utils.IsFuzzing() {
						logger.Warn(colors.Yellow+"Statement at ", fmt.Sprintf("%d.%d", trans.BlockNumber, trans.TransactionIndex), " does not reconcile."+colors.Off)
					}
				} else {
					allStatements = append(allStatements, traceStatements...)
				}
			}
		}

		if receiptStatements, err := l.getStatementsFromReceipt(pos, filter, trans, trans.Receipt); err == nil {
			allStatements = append(allStatements, receiptStatements...)
		}
	}

	isFinal := base.IsFinal(l.connection.LatestBlockTimestamp, trans.Timestamp)
	isWritable := l.connection.StoreWritable()
	isEnabled := l.connection.EnabledMap[walk.Cache_Statements]
	// TODO: BOGUS Turn on caching for allStatements once we get 100% coverage
	if false && isFinal && isWritable && isEnabled {
		for _, statement := range allStatements {
			if statement.IsMaterial() && !statement.Reconciled() {
				return allStatements, nil
			}
		}
		statementGroup := &types.StatementGroup{
			BlockNumber:      trans.BlockNumber,
			TransactionIndex: trans.TransactionIndex,
			Address:          l.accountFor,
			Statements:       allStatements,
		}
		_ = l.connection.Store.Write(statementGroup, nil)
	}

	return allStatements, nil
}

func validatePosition(pos *types.AppPosition, ctx *appBalancer) {
	if !utils.IsFuzzing() {
		if pos.Prev != ctx.Prev() {
			logger.Error("GetStatements: pos.Prev != ctx.Prev()", pos.Prev, ctx.Prev())
		}
		if pos.Next != ctx.Next() {
			logger.Error("GetStatements: pos.Next != ctx.Next()", pos.Next, ctx.Next())
		}
		if pos.First != ctx.first {
			logger.Error("GetStatements: pos.First != ctx.first", pos.First, ctx.first)
		}
		if pos.Last != ctx.last {
			logger.Error("GetStatements: pos.Last != ctx.last", pos.Last, ctx.last)
		}
	}
}
