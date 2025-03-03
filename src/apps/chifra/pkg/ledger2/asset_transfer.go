package ledger2

import (
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/filter"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/ledger3"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/normalize"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/topics"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/walk"
)

// AssetTransfer is a raw movement of an asset derived from logs, traces, or transaction details.
type AssetTransfer = types.Statement

// GetAssetTransfers parses a single Transaction and returns a slice of AssetTransfer
// by checking the transaction's Value, its Logs for ERC20 events, and an optional Traces field.
func (r *Reconciler2) GetAssetTransfers(pos *types.AppPosition, filter *filter.AppearanceFilter, trans *types.Transaction) ([]AssetTransfer, error) {
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

	var results []AssetTransfer
	if ledger3.AssetOfInterest(r.assetFilter, base.FAKE_ETH_ADDRESS) {
		type AAA = AssetTransfer
		accountedFor := r.accountFor
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
			PostAssetType:    types.TrialBalEth,
			PostFirst:        pos.First,
			PostLast:         pos.Last,
		}

		if r.asEther {
			at.AssetSymbol = "ETH"
		}

		if trans.To.IsZero() && trans.Receipt != nil && !trans.Receipt.ContractAddress.IsZero() {
			at.Recipient = trans.Receipt.ContractAddress
		}

		if false { // r.useTraces {
			// if traceStatements, err := r.getStatementsFromTraces(pos, trans, &at); err != nil {
			// 	if !utils.IsFuzzing() {
			// 		logger.Warn(colors.Yellow+"Statement at ", fmt.Sprintf("%d.%d", trans.BlockNumber, trans.TransactionIndex), " does not reconcile."+colors.Off)
			// 	}
			// } else {
			// 	allStatements = append(allStatements, traceStatements...)
			// }

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
				/*
					reconciled := r.trialBalance(pos, types.TrialBalEth, trans, &at)
					if !reconciled {
						logger.TestLog(!r.useTraces, "Trial balance failed for ", at.TransactionHash.Hex(), " need to decend into traces")
						if traceStatements, err := r.getStatementsFromTraces(pos, trans, &at); err != nil {
							logger.Warn(colors.Yellow+"Statement at ", fmt.Sprintf("%d.%d", trans.BlockNumber, trans.TransactionIndex), " does not reconcile."+colors.Off)
						} else {
							results = append(results, traceStatements...)
						}

					} else
				*/
				if at.IsMaterial() {
					results = append(results, at)
				}
			}
		}
	}

	// 2) Parse logs for ERC20 or other asset transfers. This is a simplified example
	// that checks if a log looks like an ERC20 Transfer. Real logic should decode topics, etc.
	if trans.Receipt != nil {
		for i, lg := range trans.Receipt.Logs {
			if log, err := normalize.NormalizeKnownLogs(&lg); err != nil {
				continue

			} else if log.IsNFT() {
				continue

			} else {
				addrArray := []base.Address{r.LedgerBook.AccountedFor}
				if !filter.ApplyLogFilter(log, addrArray) || !ledger3.AssetOfInterest(r.assetFilter, log.Address) {
					continue
				}

				if log.Topics[0] != topics.TransferTopic {
					continue
				}

				fromAddr := base.HexToAddress(log.Topics[1].Hex())
				toAddr := base.HexToAddress(log.Topics[2].Hex())
				var amount *base.Wei
				if amount, _ = new(base.Wei).SetString(strings.Replace(log.Data, "0x", "", i), 16); amount == nil {
					amount = base.NewWei(0)
				}

				ofInterest := false

				// Do not collapse, may be both
				amountOut := *base.ZeroWei
				if r.LedgerBook.AccountedFor == fromAddr {
					amountOut = *amount
					ofInterest = true
				}

				// Do not collapse, may be both
				amountIn := *base.ZeroWei
				if r.LedgerBook.AccountedFor == toAddr {
					amountIn = *amount
					ofInterest = true
				}

				if ofInterest {

					sym := log.Address.DefaultSymbol()
					decimals := base.Value(18)
					name := r.names[log.Address]
					if name.Address == log.Address {
						if name.Symbol != "" {
							sym = name.Symbol
						}
						if name.Decimals != 0 {
							decimals = base.Value(name.Decimals)
						}
					}

					// The contract address is in log.Address, which typically is the ERC20 token.
					at := AssetTransfer{
						AccountedFor:     r.LedgerBook.AccountedFor,
						BlockNumber:      trans.BlockNumber,
						TransactionIndex: trans.TransactionIndex,
						TransactionHash:  trans.Hash,
						Timestamp:        trans.Timestamp,
						AssetAddress:     lg.Address,
						AssetSymbol:      sym,
						AmountIn:         amountIn,
						AmountOut:        amountOut,
						Decimals:         decimals,
						LogIndex:         lg.LogIndex,
						Sender:           fromAddr,
						Recipient:        toAddr,
						PostAssetType:    types.TrialBalToken,
						PostFirst:        pos.First,
						PostLast:         pos.Last,
					}

					if at.IsMaterial() {
						results = append(results, at)
					}
				}
			}
		}
	}

	// // 3) If you have internal traces, parse them for self-destructs, internal calls, etc.
	// // For each trace that shows a value transfer from A to B, create an AssetTransfer.
	// for i, tr := range trans.Traces {
	// 	// Example check if it's a simple call with a value transfer
	// 	if strings.EqualFold(tr.Type, "call") && tr.ValueWei > 0 {
	// 		// We treat it as a native coin movement
	// 		xf := AssetTransfer{
	// 			BlockNumber:      trans.BlockNumber,
	// 			TransactionIndex: trans.TransactionIndex,
	// 			AssetAddress:        "0x0",
	// 			Amount:           *base.NewWei(tr.ValueWei),
	// 			Index:            traceIndexToString(i),
	// 			FromAddress:      tr.From,
	// 			ToAddress:        tr.To,
	// 		}
	// 		results = append(results, xf)
	// 	}
	// 	// Additional logic for selfdestruct or create, etc.
	// }

	// TODO: BOGUS Turn on caching (remove the false below) for results once we get 100% coverage
	statementGroup := &types.StatementGroup{
		BlockNumber:      trans.BlockNumber,
		TransactionIndex: trans.TransactionIndex,
		Address:          r.accountFor,
		Statements:       results,
	}
	err := r.connection.Store.WriteToCache(statementGroup, walk.Cache_Statements, trans.Timestamp, false)
	return results, err
}

// func (l *Reconciler2) getStatementsFromTraces(pos *types.AppPosition, trans *types.Transaction) ([]types.Statement, error) {
// 	_ = pos
// 	statements := make([]types.Statement, 0, 20) // a high estimate of the number of statements we'll need

// 	ret := types.Statement{} // *s
// 	// clear all the internal accounting values. Keeps AmountIn, AmountOut and GasOut because
// 	// those are at the top level (both the transaction itself and trace '0' have them). We
// 	// skip trace '0' because it's the same as the transaction.
// 	// ret.AmountIn.SetUint64(0)
// 	ret.InternalIn.SetUint64(0)
// 	ret.MinerBaseRewardIn.SetUint64(0)
// 	ret.MinerNephewRewardIn.SetUint64(0)
// 	ret.MinerTxFeeIn.SetUint64(0)
// 	ret.MinerUncleRewardIn.SetUint64(0)
// 	ret.CorrectingIn.SetUint64(0)
// 	ret.PrefundIn.SetUint64(0)
// 	ret.SelfDestructIn.SetUint64(0)

// 	// ret.AmountOut.SetUint64(0)
// 	// ret.GasOut.SetUint64(0)
// 	ret.InternalOut.SetUint64(0)
// 	ret.CorrectingOut.SetUint64(0)
// 	ret.SelfDestructOut.SetUint64(0)

// 	if traces, err := l.connection.GetTracesByTransactionHash(trans.Hash.Hex(), trans); err != nil {
// 		return statements, err

// 	} else {
// 		// These values accumulate...so we use += instead of =
// 		for i, trace := range traces {
// 			if i == 0 {
// 				// the first trace is identical to the transaction itself, so we can skip it
// 				continue
// 			}

// 			if trace.Action.CallType == "delegatecall" && trace.Action.To != l.LedgerBook.AccountedFor {
// 				// delegate calls are not included in the transaction's gas cost, so we skip them
// 				continue
// 			}

// 			plusEq := func(a1, a2 *base.Wei) base.Wei {
// 				return *a1.Add(a1, a2)
// 			}

// 			// Do not collapse, more than one of these can be true at the same time
// 			if trace.Action.From == l.LedgerBook.AccountedFor {
// 				ret.InternalOut = plusEq(&ret.InternalOut, &trace.Action.Value)
// 				ret.Sender = trace.Action.From
// 				if trace.Action.To.IsZero() {
// 					if trace.Result != nil {
// 						ret.Recipient = trace.Result.Address
// 					}
// 				} else {
// 					ret.Recipient = trace.Action.To
// 				}
// 			}

// 			if trace.Action.To == l.LedgerBook.AccountedFor {
// 				ret.InternalIn = plusEq(&ret.InternalIn, &trace.Action.Value)
// 				ret.Sender = trace.Action.From
// 				ret.Recipient = trace.Action.To
// 			}

// 			if trace.Action.SelfDestructed == l.LedgerBook.AccountedFor {
// 				ret.SelfDestructOut = plusEq(&ret.SelfDestructOut, &trace.Action.Balance)
// 				ret.Sender = trace.Action.SelfDestructed
// 				if ret.Sender.IsZero() {
// 					ret.Sender = trace.Action.Address
// 				}
// 				ret.Recipient = trace.Action.RefundAddress
// 			}

// 			if trace.Action.RefundAddress == l.LedgerBook.AccountedFor {
// 				ret.SelfDestructIn = plusEq(&ret.SelfDestructIn, &trace.Action.Balance)
// 				ret.Sender = trace.Action.SelfDestructed
// 				if ret.Sender.IsZero() {
// 					ret.Sender = trace.Action.Address
// 				}
// 				ret.Recipient = trace.Action.RefundAddress
// 			}

// 			if trace.Action.Address == l.LedgerBook.AccountedFor && !trace.Action.RefundAddress.IsZero() {
// 				ret.SelfDestructOut = plusEq(&ret.SelfDestructOut, &trace.Action.Balance)
// 				// self destructed send
// 				ret.Sender = trace.Action.Address
// 				ret.Recipient = trace.Action.RefundAddress
// 			}

// 			if trace.Result != nil {
// 				if trace.Result.Address == l.LedgerBook.AccountedFor {
// 					ret.InternalIn = plusEq(&ret.InternalIn, &trace.Action.Value)
// 					ret.Sender = trace.Action.From
// 					ret.Recipient = trace.Result.Address
// 				}
// 			}
// 		}
// 	}

// 	if utils.IsFuzzing() {
// 		statements = append(statements, ret)
// 		return statements, nil
// 	}

// 	// reconciled := l.trialBalance(pos, types.TrialBalTraceEth, trans, &ret)
// 	// if !reconciled {
// 	// 	statements = append(statements, ret)
// 	// 	return statements, nil
// 	// }

// 	if ret.IsMaterial() {
// 		statements = append(statements, ret)
// 		// } else {
// 		// 	logger.TestLog(true, "Tx reconciled with a zero value net amount. It's okay.")
// 	}

// 	return statements, nil
// }
