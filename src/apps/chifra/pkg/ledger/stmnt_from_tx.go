package ledger

import (
	"math/big"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// GetStatementsFromTransaction returns a statement from a given transaction
func (l *Ledger) GetStatementsFromTransaction(trans *types.SimpleTransaction) (statements []*types.SimpleStatement) {
	// make room for our results
	statements = make([]*types.SimpleStatement, 0, 20) // a high estimate of the number of statements we'll need

	key := l.CtxKey(trans.BlockNumber, trans.TransactionIndex)
	ctx := l.Contexts[key]

	if l.AssetOfInterest(base.FAKE_ETH_ADDRESS) {
		// TODO: We ignore errors in the next few lines, but we should not
		// TODO: performance - This greatly increases the number of times we call into eth_getBalance which is quite slow
		prevBal, _ := l.Conn.GetBalanceAt(l.AccountFor, ctx.PrevBlock)
		if trans.BlockNumber == 0 {
			prevBal = new(big.Int)
		}
		begBal, _ := l.Conn.GetBalanceAt(l.AccountFor, ctx.CurBlock-1)
		endBal, _ := l.Conn.GetBalanceAt(l.AccountFor, ctx.CurBlock)

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
			PrevAppBlk:       ctx.PrevBlock,
			PrevBal:          *prevBal,
			BegBal:           *begBal,
			EndBal:           *endBal,
		}

		if trans.To.IsZero() && trans.Receipt != nil && !trans.Receipt.ContractAddress.IsZero() {
			ret.Recipient = trans.Receipt.ContractAddress
		}

		// Do not collapse. A single transaction may have many movements of money
		if l.AccountFor == ret.Sender {
			gasUsed := new(big.Int)
			if trans.Receipt != nil {
				gasUsed.SetUint64(trans.Receipt.GasUsed)
			}
			gasPrice := new(big.Int).SetUint64(trans.GasPrice)
			gasOut := new(big.Int).Mul(gasUsed, gasPrice)

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
			}
		}

		if l.AsEther {
			ret.AssetSymbol = "ETH"
		}

		if !l.UseTraces && l.TrialBalance("ETH", &ret) {
			if ret.MoneyMoved() {
				statements = append(statements, &ret)
			} else {
				logger.TestLog(true, "Tx reconciled with a zero value net amount. It's okay.")
			}
		} else {
			if !l.UseTraces {
				logger.TestLog(!l.UseTraces, "Trial balance failed for ", ret.TransactionHash.Hex(), "need to decend into traces")
			}
			if traceStatements := l.GetStatementsFromTraces(trans, &ret); len(traceStatements) == 0 {
				logger.Warn(l.TestMode, "Error getting statement from traces")
			} else {
				statements = append(statements, traceStatements...)
			}
		}
	}

	if trans.Receipt != nil {
		for _, log := range trans.Receipt.Logs {
			if l.AssetOfInterest(log.Address) {
				if statement, err := l.GetStatementFromLog(&log); statement != nil {
					if statement.Sender == l.AccountFor || statement.Recipient == l.AccountFor {
						add := !l.NoZero || statement.MoneyMoved()
						if add {
							statements = append(statements, statement)
						}
					}
				} else if err != nil {
					logger.Warn(l.TestMode, "Error getting statement from log: ", err)
				}
			}
		}
	}

	return
}
