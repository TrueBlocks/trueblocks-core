package ledger

import (
	"fmt"
	"math/big"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/ethereum/go-ethereum/common"
)

// GetStatementFromTransaction returns a statement from a given transaction
func (ledgers *Ledger) GetStatementsFromTransaction(trans *types.SimpleTransaction) (statements []*types.SimpleStatement) {
	statements = make([]*types.SimpleStatement, 0, 20) // a high estimate of the number of statements we'll need
	// TODO: BOGUS - DO WE HAVE ANY TESTS WHERE IS_ERROR IS USED TO NOT BALANCE A TRANSACTION? DOES MONEYMOVED MAKE THAT NOT NECESSARY?
	// if trans.IsError {
	// 	return nil
	// }

	key := fmt.Sprintf("%09d-%05d", trans.BlockNumber, trans.TransactionIndex)
	ctx := ledgers.Contexts[key]

	pBal, _ := rpcClient.GetBalanceAt(ledgers.Chain, common.HexToAddress(ledgers.AccountFor.Hex()), ctx.PrevBlock)
	bBal, _ := rpcClient.GetBalanceAt(ledgers.Chain, common.HexToAddress(ledgers.AccountFor.Hex()), ctx.CurBlock-1)
	eBal, _ := rpcClient.GetBalanceAt(ledgers.Chain, common.HexToAddress(ledgers.AccountFor.Hex()), ctx.CurBlock)

	gasUsed := new(big.Int).SetUint64(trans.Receipt.GasUsed)
	gasPrice := new(big.Int).SetUint64(trans.GasPrice)
	gasOut := new(big.Int).Mul(gasUsed, gasPrice)

	ret := types.SimpleStatement{
		AccountedFor:     ledgers.AccountFor,
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
		PrevBal:          *pBal,
		BegBal:           *bBal,
		EndBal:           *eBal,
	}

	if trans.To.IsZero() && !trans.Receipt.ContractAddress.IsZero() {
		ret.Recipient = trans.Receipt.ContractAddress
	}

	if ledgers.AccountFor == ret.Sender {
		ret.AmountOut = trans.Value
		ret.GasOut = *gasOut
	}

	// Do not collapse, may be both (self-send)
	if ledgers.AccountFor == ret.Recipient {
		ret.AmountIn = trans.Value
	}

	if ledgers.AsEther {
		ret.AssetSymbol = "ETH"
	}

	if !ledgers.UseTraces && ledgers.TrialBalance("ETH", &ret) {
		if ret.MoneyMoved() {
			statements = append(statements, &ret)
		} else {
			logger.TestLog(true, "Tx reconciled with a zero value net amount. It's okay.")
		}
	} else {
		logger.TestLog(!ledgers.UseTraces, "Trial balance failed for ", ret.TransactionHash.Hex(), "need to decend into traces")
		if traceStatements := ledgers.GetStatementsFromTraces(trans, &ret); len(traceStatements) == 0 {
			logger.Warn(ledgers.TestMode, "Error getting statement from traces")
		} else {
			statements = append(statements, traceStatements...)
		}
	}

	for _, log := range trans.Receipt.Logs {
		if s, err := ledgers.GetStatementFromLog(&log); s != nil {
			if s.Sender == ledgers.AccountFor || s.Recipient == ledgers.AccountFor {
				add := !ledgers.NoZero || s.MoneyMoved()
				if add {
					statements = append(statements, s)
				}
			}
		} else if err != nil {
			logger.Warn(ledgers.TestMode, "Error getting statement from log: ", err)
		}
	}

	return
}
