package ledger1

import (
	"fmt"
	"os"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/filter"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/ledger4"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/normalize"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/topics"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/walk"
)

func (r *Reconciler) GetAssetTransfers(pos *types.AppPosition, filter *filter.AppearanceFilter, trans *types.Transaction) ([]ledger4.AssetTransfer, error) {
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

	if ledger4.AssetOfInterest(r.assetFilter, base.FAKE_ETH_ADDRESS) {
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
		receiptStatements := make([]types.Statement, 0, 20)
		for _, log := range trans.Receipt.Logs {
			if log.Topics[0] != topics.TransferTopic {
				continue
			}
			addrArray := []base.Address{r.accountFor}
			if filter.ApplyLogFilter(&log, addrArray) && ledger4.AssetOfInterest(r.assetFilter, log.Address) {
				normalized, err := normalize.NormalizeKnownLogs(&log)
				if err != nil {
					continue
				} else if normalized.IsNFT() {
					continue
				} else {
					sender := base.HexToAddress(normalized.Topics[1].Hex())
					recipient := base.HexToAddress(normalized.Topics[2].Hex())
					isSender, isRecipient := r.accountFor == sender, r.accountFor == recipient
					if utils.IsFuzzing() || (!isSender && !isRecipient) {
						continue
					}
					sym := normalized.Address.DefaultSymbol()
					decimals := base.Value(18)
					name := r.names[normalized.Address]
					if name.Address == normalized.Address {
						if name.Symbol != "" {
							sym = name.Symbol
						}
						if name.Decimals != 0 {
							decimals = base.Value(name.Decimals)
						}
					}
					var amountIn, amountOut base.Wei
					amt, _ := new(base.Wei).SetString(strings.Replace(normalized.Data, "0x", "", -1), 16)
					if amt == nil {
						amt = base.NewWei(0)
					}
					// Do not collapse, may be both
					if r.accountFor == sender {
						amountOut = *amt
					}
					// Do not collapse, may be both
					if r.accountFor == recipient {
						amountIn = *amt
					}
					s := types.Statement{
						AccountedFor:     r.accountFor,
						Sender:           sender,
						Recipient:        recipient,
						BlockNumber:      normalized.BlockNumber,
						TransactionIndex: normalized.TransactionIndex,
						LogIndex:         normalized.LogIndex,
						TransactionHash:  normalized.TransactionHash,
						Timestamp:        normalized.Timestamp,
						AssetAddress:     normalized.Address,
						AssetSymbol:      sym,
						Decimals:         decimals,
						SpotPrice:        0.0,
						PriceSource:      "not-priced",
						AmountIn:         amountIn,
						AmountOut:        amountOut,
						PostFirst:        pos.First,
						PostLast:         pos.Last,
					}
					pBal, err := r.connection.GetBalanceAtToken(normalized.Address, r.accountFor, pos.Prev)
					if err != nil || pBal == nil {
						continue
					}
					s.PrevBal = *pBal
					bBal, err := r.connection.GetBalanceAtToken(normalized.Address, r.accountFor, trans.BlockNumber-1)
					if err != nil || bBal == nil {
						continue
					}
					if trans.BlockNumber == 0 {
						bBal = base.NewWei(0)
					}
					s.BegBal = *bBal
					eBal, err := r.connection.GetBalanceAtToken(normalized.Address, r.accountFor, trans.BlockNumber)
					if err != nil || eBal == nil {
						continue
					}
					s.EndBal = *eBal
					reconciled := r.trialBalance(pos, types.TrialBalToken, trans, &s)
					if reconciled {
						id := fmt.Sprintf(" %d.%d.%d", s.BlockNumber, s.TransactionIndex, s.LogIndex)
						logger.Progress(true, colors.Green+"Transaction", id, "reconciled       "+colors.Off)
					} else {
						id := fmt.Sprintf(" %d.%d.%d", s.BlockNumber, s.TransactionIndex, s.LogIndex)
						if os.Getenv("TEST_MODE") != "true" {
							logger.Warn("Log statement at ", id, " does not reconcile.")
						}
					}
					if (s.Sender == r.accountFor || s.Recipient == r.accountFor) && s.IsMaterial() {
						receiptStatements = append(receiptStatements, s)
					}
				}
			}
		}
		results = append(results, receiptStatements...)
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
