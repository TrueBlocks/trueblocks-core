package ledger

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/filter"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/ledger3"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/walk"
)

type AssetTransfer = types.Statement

func (r *Reconciler) GetAssetTransfers(pos *types.AppPosition, filter *filter.AppearanceFilter, trans *types.Transaction) ([]AssetTransfer, error) {
	if r.connection.Store != nil {
		statementGroup := &types.StatementGroup{
			BlockNumber:      trans.BlockNumber,
			TransactionIndex: trans.TransactionIndex,
			Address:          r.accountFor,
		}
		if err := r.connection.Store.Read(statementGroup); err == nil {
			return statementGroup.Statements, nil
		}
	}

	var err error
	results := make([]types.Statement, 0, 20)

	if ledger3.AssetOfInterest(r.assetFilter, base.FAKE_ETH_ADDRESS) {
		type AAA = types.Statement
		accountedFor := r.accountFor

		prevBal, _ := r.connection.GetBalanceAt(r.accountFor, pos.Prev)
		if trans.BlockNumber == 0 {
			prevBal = base.ZeroWei
		}

		begBal, _ := r.connection.GetBalanceAt(r.accountFor, trans.BlockNumber-1)
		if trans.BlockNumber == 0 {
			begBal = base.ZeroWei
		}

		endBal, _ := r.connection.GetBalanceAt(r.accountFor, trans.BlockNumber)

		at := AAA{
			AccountedFor:     accountedFor,
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
			at.AssetSymbol = "ETH"
		}

		if trans.To.IsZero() && trans.Receipt != nil && !trans.Receipt.ContractAddress.IsZero() {
			at.Recipient = trans.Receipt.ContractAddress
		}

		if r.useTraces {
			if traceStatements, err := r.getStatementsFromTraces(pos, trans, &at); err != nil {
				if !utils.IsFuzzing() {
					logger.Warn(colors.Yellow+"Statement at ", fmt.Sprintf("%d.%d", trans.BlockNumber, trans.TransactionIndex), " does not reconcile."+colors.Off)
				}
			} else {
				results = append(results, traceStatements...)
			}

		} else {
			// Do not collapse. A single transaction may have many movements of money
			if at.Sender == accountedFor {
				gasUsed := new(base.Wei)
				if trans.Receipt != nil {
					gasUsed.SetUint64(uint64(trans.Receipt.GasUsed))
				}
				gasPrice := new(base.Wei).SetUint64(uint64(trans.GasPrice))
				gasOut := new(base.Wei).Mul(gasUsed, gasPrice)
				at.AmountOut = trans.Value
				at.GasOut = *gasOut
			}

			// Do not collapse. A single transaction may have many movements of money
			if at.Recipient == accountedFor {
				if at.BlockNumber == 0 {
					at.PrefundIn = trans.Value
				} else {
					if trans.Rewards != nil {
						at.MinerBaseRewardIn = trans.Rewards.Block
						at.MinerNephewRewardIn = trans.Rewards.Nephew
						at.MinerTxFeeIn = trans.Rewards.TxFee
						at.MinerUncleRewardIn = trans.Rewards.Uncle
					} else {
						at.AmountIn = trans.Value
					}
				}
			}

			if !utils.IsFuzzing() {
				reconciled := r.trialBalance(pos, types.TrialBalEth, trans, &at)
				if !reconciled {
					logger.TestLog(!r.useTraces, "Trial balance failed for ", at.TransactionHash.Hex(), " need to decend into traces")
					if traceStatements, err := r.getStatementsFromTraces(pos, trans, &at); err != nil {
						logger.Warn(colors.Yellow+"Statement at ", fmt.Sprintf("%d.%d", trans.BlockNumber, trans.TransactionIndex), " does not reconcile."+colors.Off)
					} else {
						results = append(results, traceStatements...)
					}

				} else if at.IsMaterial() {
					results = append(results, at)
				}
			}
		}
	}

	if trans.Receipt != nil {
		if receiptStatements, err := r.getStatementsFromReceipt(pos, filter, trans); err == nil {
			results = append(results, receiptStatements...)
		}
	}

	allReconciled := true
	for _, statement := range results {
		if statement.IsMaterial() && !statement.Reconciled() {
			allReconciled = false
			break
		}
	}

	// TODO: BOGUS Turn on caching (remove the false below) for results once we get 100% coverage
	statementGroup := &types.StatementGroup{
		BlockNumber:      trans.BlockNumber,
		TransactionIndex: trans.TransactionIndex,
		Address:          r.accountFor,
		Statements:       results,
	}
	err = r.connection.Store.WriteToCache(statementGroup, walk.Cache_Statements, trans.Timestamp, allReconciled, false)
	return results, err
}
