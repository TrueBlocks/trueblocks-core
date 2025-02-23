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
func (l *Reconciler) GetStatements(pos *types.AppPosition, filter *filter.AppearanceFilter, trans *types.Transaction) ([]types.Statement, error) {
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
		r := ledger2.NewReconciler(l.connection, l.assetFilter, l.accountFor, l.names, l.asEther)
		if allStatements, err = r.GetStatements(pos, filter, trans); err != nil {
			return allStatements, err
		}

	} else {
		if ledger2.AssetOfInterest(l.assetFilter, base.FAKE_ETH_ADDRESS) {

			prevBal, _ := l.connection.GetBalanceAt(l.accountFor, pos.Prev)
			if trans.BlockNumber == 0 {
				prevBal = base.ZeroWei
			}

			begBal, _ := l.connection.GetBalanceAt(l.accountFor, trans.BlockNumber-1)
			if trans.BlockNumber == 0 {
				begBal = base.ZeroWei
			}

			endBal, _ := l.connection.GetBalanceAt(l.accountFor, trans.BlockNumber)

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
				BlockNumberPrev:  pos.Prev,
				BlockNumberNext:  pos.Next,
				PostFirst:        pos.First,
				PostLast:         pos.Last,
			}

			if l.asEther {
				s.AssetSymbol = "ETH"
			}

			if trans.To.IsZero() && trans.Receipt != nil && !trans.Receipt.ContractAddress.IsZero() {
				s.Recipient = trans.Receipt.ContractAddress
			}

			if l.useTraces {
				if traceStatements, err := l.getStatementsFromTraces(pos, trans, &s); err != nil {
					if !utils.IsFuzzing() {
						logger.Warn(colors.Yellow+"Statement at ", fmt.Sprintf("%d.%d", trans.BlockNumber, trans.TransactionIndex), " does not reconcile."+colors.Off)
					}
				} else {
					allStatements = append(allStatements, traceStatements...)
				}

			} else {
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
					}
				}

				if utils.IsFuzzing() {
					// do nothing

				} else {
					reconciled := l.trialBalance(pos, types.TrialBalEth, trans, &s)
					if !reconciled {
						logger.TestLog(!l.useTraces, "Trial balance failed for ", s.TransactionHash.Hex(), " need to decend into traces")
						if traceStatements, err := l.getStatementsFromTraces(pos, trans, &s); err != nil {
							logger.Warn(colors.Yellow+"Statement at ", fmt.Sprintf("%d.%d", trans.BlockNumber, trans.TransactionIndex), " does not reconcile."+colors.Off)
						} else {
							allStatements = append(allStatements, traceStatements...)
						}

					} else if s.IsMaterial() {
						allStatements = append(allStatements, s)
					}
				}
			}
		}

		if trans.Receipt != nil {
			if receiptStatements, err := l.getStatementsFromReceipt(pos, filter, trans); err == nil {
				allStatements = append(allStatements, receiptStatements...)
			}
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
