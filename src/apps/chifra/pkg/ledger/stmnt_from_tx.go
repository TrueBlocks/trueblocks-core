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

// GetTransfers returns a statement from a given transaction
func (r *Reconciler) GetTransfers(pos *types.AppPosition, filter *filter.AppearanceFilter, trans *types.Transaction) ([]types.Transfer, error) {
	r2 := ledger2.NewReconciler2(r.connection, r.assetFilter, r.accountFor, r.names, r.asEther)
	return r2.GetTransfers(pos, filter, trans)
}

// GetStatements returns a statement from a given transaction
func (r *Reconciler) GetStatements(pos *types.AppPosition, filter *filter.AppearanceFilter, trans *types.Transaction) ([]types.Statement, error) {
	if r.connection.StoreReadable() {
		statementGroup := &types.StatementGroup{
			BlockNumber:      trans.BlockNumber,
			TransactionIndex: trans.TransactionIndex,
			Address:          r.accountFor,
		}
		if err := r.connection.Store.Read(statementGroup, nil); err == nil {
			return statementGroup.Statements, nil
		}
	}

	var err error
	allStatements := make([]types.Statement, 0, 20)

	if os.Getenv("NEW_CODE") == "true" {
		r := ledger2.NewReconciler2(r.connection, r.assetFilter, r.accountFor, r.names, r.asEther)
		if allStatements, err = r.GetStatements(pos, filter, trans); err != nil {
			return allStatements, err
		}

	} else {
		if ledger2.AssetOfInterest(r.assetFilter, base.FAKE_ETH_ADDRESS) {

			prevBal, _ := r.connection.GetBalanceAt(r.accountFor, pos.Prev)
			if trans.BlockNumber == 0 {
				prevBal = base.ZeroWei
			}

			begBal, _ := r.connection.GetBalanceAt(r.accountFor, trans.BlockNumber-1)
			if trans.BlockNumber == 0 {
				begBal = base.ZeroWei
			}

			endBal, _ := r.connection.GetBalanceAt(r.accountFor, trans.BlockNumber)

			s := types.Statement{
				AccountedFor:     r.accountFor,
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
				PostFirst:        pos.First,
				PostLast:         pos.Last,
			}

			if r.asEther {
				s.AssetSymbol = "ETH"
			}

			if trans.To.IsZero() && trans.Receipt != nil && !trans.Receipt.ContractAddress.IsZero() {
				s.Recipient = trans.Receipt.ContractAddress
			}

			if r.useTraces {
				if traceStatements, err := r.getStatementsFromTraces(pos, trans, &s); err != nil {
					if !utils.IsFuzzing() {
						logger.Warn(colors.Yellow+"Statement at ", fmt.Sprintf("%d.%d", trans.BlockNumber, trans.TransactionIndex), " does not reconcile."+colors.Off)
					}
				} else {
					allStatements = append(allStatements, traceStatements...)
				}

			} else {
				// Do not collapse. A single transaction may have many movements of money
				if s.Sender == r.accountFor {
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
				if s.Recipient == r.accountFor {
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
					reconciled := r.trialBalance(pos, types.TrialBalEth, trans, &s)
					if !reconciled {
						logger.TestLog(!r.useTraces, "Trial balance failed for ", s.TransactionHash.Hex(), " need to decend into traces")
						if traceStatements, err := r.getStatementsFromTraces(pos, trans, &s); err != nil {
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
			if receiptStatements, err := r.getStatementsFromReceipt(pos, filter, trans); err == nil {
				allStatements = append(allStatements, receiptStatements...)
			}
		}
	}

	isFinal := base.IsFinal(r.connection.LatestBlockTimestamp, trans.Timestamp)
	isWritable := r.connection.StoreWritable()
	isEnabled := r.connection.EnabledMap[walk.Cache_Statements]
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
			Address:          r.accountFor,
			Statements:       allStatements,
		}
		_ = r.connection.Store.Write(statementGroup, nil)
	}

	return allStatements, nil
}
