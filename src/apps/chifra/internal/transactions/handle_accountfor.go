package transactionsPkg

import (
	"context"
	"errors"
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/ethereum/go-ethereum"
)

func (opts *TransactionsOptions) HandleAccounting() (err error) {
	chain := opts.Globals.Chain

	ctx, cancel := context.WithCancel(context.Background())
	fetchData := func(modelChan chan types.Modeler[types.RawReconciliation], errorChan chan error) {
		for _, rng := range opts.TransactionIds {
			txIds, err := rng.ResolveTxs(chain)
			if err != nil && !errors.Is(err, ethereum.NotFound) {
				errorChan <- err
				cancel()
			}

			for _, app := range txIds {
				if statements, err := opts.GetStatementsFromAppearance(chain, &app); err != nil {
				} else {
					for _, statement := range statements {
						modelChan <- &statement
					}
				}
			}
		}
	}

	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOpts())
}

func (opts *TransactionsOptions) GetStatementsFromAppearance(chain string, app *types.RawAppearance) (statements []types.SimpleReconciliation, err error) {
	if app == nil {
		return nil, nil
	}

	ret := []types.SimpleReconciliation{}
	if true { // app.IsToken() {
		if tx, err := rpcClient.GetTransactionByAppearance(chain, app, false); err != nil {
			return ret, err
		} else {
			if t := opts.GetTransferFromTransaction(tx); t != nil {
				s := opts.GetStatementFromTransfer(t)
				ret = append(ret, *s)
			}
			for _, log := range tx.Receipt.Logs {
				if t := opts.GetTransferFromLog(&log); t != nil {
					acctFor := base.HexToAddress(opts.AccountFor)
					if t.Sender == acctFor || t.Recipient == acctFor {
						s := opts.GetStatementFromTransfer(t)
						ret = append(ret, *s)
					}
				}
			}
			if opts.Globals.TestMode {
				fmt.Println("TEST[DATE|TIME] -------------pricing-------------------------------")
				fmt.Println("TIME ~ CLOCK - <INFO>  : Calling 0x")
				fmt.Println("TIME ~ CLOCK - <INFO>  : call to 0x")
			}
		}
	}

	return ret, nil
}

func (opts *TransactionsOptions) GetTransferFromLog(log *types.SimpleLog) (transfer *types.SimpleTransfer) {
	// logger.TestLog(true, log)
	if len(log.Topics) < 3 {
		return nil
	}
	if rpcClient.IsTransferTopic(log.Topics[0].Hex()) {
		return &types.SimpleTransfer{
			Sender:           base.HexToAddress(log.Topics[1].Hex()),
			Recipient:        base.HexToAddress(log.Topics[2].Hex()),
			BlockNumber:      log.BlockNumber,
			TransactionIndex: log.TransactionIndex,
			TransactionHash:  log.TransactionHash,
			LogIndex:         log.LogIndex,
			Timestamp:        log.Timestamp,
			Date:             log.Date,
			AssetAddr:        log.Address,
			Log:              log,
			Decimals:         18,
			PriceSource:      log.Topics[0].Hex(),
			// Amount:           base.NewFromBig(&log.Value),
		}
	}
	return nil
}

func (opts *TransactionsOptions) GetTransferFromTransaction(trans *types.SimpleTransaction) (transfer *types.SimpleTransfer) {
	// logger.TestLog(true, trans.Value.String())
	// if trans.Value.String() == "0" {
	// 	return nil
	// }

	ret := types.SimpleTransfer{
		Sender:           trans.From,
		Recipient:        trans.To,
		BlockNumber:      trans.BlockNumber,
		TransactionIndex: trans.TransactionIndex,
		TransactionHash:  trans.Hash,
		LogIndex:         0,
		Timestamp:        trans.Timestamp,
		Date:             trans.Date,
		AssetAddr:        base.HexToAddress("0xeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee"),
		Log:              nil,
		Decimals:         18,
		Amount:           base.NewFromBig(&trans.Value),
	}

	if len(trans.Input) > 10 {
		ret.Encoding = trans.Input[:10]
	}

	ret.AssetSymbol = "WEI"
	if opts.Globals.Ether {
		ret.AssetSymbol = "ETH"
	}

	return &ret
}

func (opts *TransactionsOptions) TrialBalance(msg string) {
	logger.TestLog(true, "ledger.blockNumber:     X")
	logger.TestLog(true, "prevBlock:              X")
	logger.TestLog(true, "transfer.blockNumber:   X")
	logger.TestLog(true, "nextBlock:              X")
	logger.TestLog(true, "isPrevDiff:             X")
	logger.TestLog(true, "isNextDiff:             X")
	logger.TestLog(true, fmt.Sprintf("-------------%s-----------------------------", msg))
	logger.TestLog(true, "Trial balance:")
	logger.TestLog(true, "")
	logger.TestLog(true, "   accountedFor:        X")
	logger.TestLog(true, "   assetAddr:           X")
	logger.TestLog(true, "   assetSymbol:         X")
	logger.TestLog(true, "   hash:                X")
	logger.TestLog(true, "   blockNumber:         X")
	logger.TestLog(true, "   transactionIndex:    X")
	logger.TestLog(true, "   logIndex:            X")
	logger.TestLog(true, "   begBal:              X")
	logger.TestLog(true, "   endBal:              X")
	logger.TestLog(true, "   trialBalance:        X")
	logger.TestLog(true, "---------------------------------------------------")
}

func (opts *TransactionsOptions) GetStatementFromTransfer(transfer *types.SimpleTransfer) (statement *types.SimpleReconciliation) {
	if len(transfer.PriceSource) == 0 {
		logger.TestLog(true, "")
		logger.TestLog(true, "===================================================")
		opts.TrialBalance("flows-top")
		opts.TrialBalance("balances-top")
	} else {
		logger.TestLog(true, "===================================================")
		opts.TrialBalance("flows-token")
		opts.TrialBalance("balances-token")
	}

	return &types.SimpleReconciliation{
		BlockNumber:         transfer.BlockNumber,
		TransactionIndex:    transfer.TransactionIndex,
		LogIndex:            transfer.LogIndex,
		TransactionHash:     transfer.TransactionHash,
		Timestamp:           transfer.Timestamp,
		Date:                utils.FormattedDate(transfer.Timestamp),
		AssetAddr:           transfer.AssetAddr,
		AssetSymbol:         transfer.AssetSymbol,
		Decimals:            transfer.Decimals,
		SpotPrice:           transfer.SpotPrice,
		PriceSource:         transfer.PriceSource,
		AccountedFor:        base.HexToAddress(opts.AccountFor),
		Sender:              transfer.Sender,
		Recipient:           transfer.Recipient,
		BegBal:              base.NewBigInt(12),
		AmountNet:           base.NewBigInt(13),
		EndBal:              base.NewBigInt(14),
		ReconciliationType:  "junk",
		Reconciled:          true,
		TotalIn:             base.NewBigInt(15),
		AmountIn:            transfer.Amount,
		InternalIn:          base.NewBigInt(17),
		SelfDestructIn:      base.NewBigInt(18),
		MinerBaseRewardIn:   base.NewBigInt(19),
		MinerNephewRewardIn: base.NewBigInt(20),
		MinerTxFeeIn:        base.NewBigInt(21),
		MinerUncleRewardIn:  base.NewBigInt(22),
		PrefundIn:           base.NewBigInt(24),
		TotalOut:            base.NewBigInt(25),
		AmountOut:           base.NewBigInt(26),
		InternalOut:         base.NewBigInt(27),
		SelfDestructOut:     base.NewBigInt(28),
		GasOut:              base.NewBigInt(29),
		TotalOutLessGas:     base.NewBigInt(30),
		PrevAppBlk:          transfer.BlockNumber - 1,
		PrevBal:             base.NewBigInt(31),
		BegBalDiff:          base.NewBigInt(32),
		EndBalDiff:          base.NewBigInt(33),
		EndBalCalc:          base.NewBigInt(34),
	}
}
