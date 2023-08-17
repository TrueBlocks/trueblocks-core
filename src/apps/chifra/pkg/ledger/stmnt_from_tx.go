package ledger

import (
	"math/big"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// GetStatementsFromTransaction returns a statement from a given transaction
func (l *Ledger) GetStatementsFromTransaction(conn *rpc.Connection, trans *types.SimpleTransaction) (statements []*types.SimpleStatement) {
	if false && conn.StoreReadable() {
		statementGroup := &types.SimpleStatementGroup{
			Address:          l.AccountFor,
			BlockNumber:      trans.BlockNumber,
			TransactionIndex: trans.TransactionIndex,
		}
		if err := conn.Store.Read(statementGroup, nil); err == nil {
			pointers := []*types.SimpleStatement{}
			for _, statement := range statementGroup.Statements {
				pointers = append(pointers, &statement)
			}
			return pointers
		}
	}

	// make room for our results
	statements = make([]*types.SimpleStatement, 0, 20) // a high estimate of the number of statements we'll need

	key := l.ctxKey(trans.BlockNumber, trans.TransactionIndex)
	ctx := l.Contexts[key]

	if l.assetOfInterest(base.FAKE_ETH_ADDRESS) {
		// TODO: We ignore errors in the next few lines, but we should not
		// TODO: performance - This greatly increases the number of times we call into eth_getBalance which is quite slow
		prevBal, _ := conn.GetBalanceAt(l.AccountFor, ctx.PrevBlock)
		if trans.BlockNumber == 0 {
			prevBal = new(big.Int)
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

		if !l.UseTraces && l.trialBalance("ETH", &ret) {
			if ret.MoneyMoved() {
				statements = append(statements, &ret)
			} else {
				logger.TestLog(true, "Tx reconciled with a zero value net amount. It's okay.")
			}
		} else {
			if !l.UseTraces {
				logger.TestLog(!l.UseTraces, "Trial balance failed for ", ret.TransactionHash.Hex(), "need to decend into traces")
			}
			if traceStatements := l.getStatementsFromTraces(conn, trans, &ret); len(traceStatements) == 0 {
				logger.Warn(l.TestMode, "Error getting statement from traces")
			} else {
				statements = append(statements, traceStatements...)
			}
		}
	}

	if trans.Receipt != nil {
		for _, log := range trans.Receipt.Logs {
			log := log
			if l.assetOfInterest(log.Address) && log.ContainsAddress(l.AccountFor) {
				if statement, err := l.getStatementFromLog(conn, &log); statement != nil {
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

	if false && l.Conn.StoreWritable() && l.Conn.EnabledMap["statements"] && base.IsFinal(l.Conn.LatestBlockTimestamp, trans.Timestamp) {
		objects := make([]types.SimpleStatement, len(statements))
		for _, ptr := range statements {
			objects = append(objects, *ptr)
		}
		statementGroup := &types.SimpleStatementGroup{
			Address:          l.AccountFor,
			BlockNumber:      trans.BlockNumber,
			TransactionIndex: trans.TransactionIndex,
			Statements:       objects,
		}
		_ = conn.Store.Write(statementGroup, nil)
		// logger.Info("Writing call results to the database...", results.Address, results.BlockNumber, call.Method.Encoding)
		// if err := call.Conn.Store.Write(results, nil); err != nil {
		// 	logger.Warn("Failed to write call results to the database", err) // report but don't fail
		// }
		// } else if !isFin {
		// 	logger.Info("Not caching result (not ripe)...", results.Address, results.BlockNumber, call.Method.Encoding)
	}

	return
}
