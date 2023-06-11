package transactionsPkg

import (
	"context"
	"errors"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
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
				if statements, err := opts.GetStatementsByAppearance(chain, &app); err != nil {
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

func (opts *TransactionsOptions) GetStatementsByAppearance(chain string, app *types.RawAppearance) (statements []types.SimpleReconciliation, err error) {
	if app == nil {
		return nil, nil
	}

	ret := []types.SimpleReconciliation{}
	if true { // app.IsToken() {
		if tx, err := rpcClient.GetTransactionByAppearance(chain, app, false); err != nil {
			return ret, err
		} else {
			t := opts.TransactionToTransfer(tx)
			s := types.SimpleReconciliation{
				BlockNumber:         t.BlockNumber,
				TransactionIndex:    t.TransactionIndex,
				LogIndex:            t.LogIndex,
				TransactionHash:     t.TransactionHash,
				Timestamp:           t.Timestamp,
				Date:                t.Date,
				AssetAddr:           t.AssetAddr,
				AssetSymbol:         t.AssetSymbol,
				Decimals:            t.Decimals,
				SpotPrice:           t.SpotPrice,
				PriceSource:         t.PriceSource,
				AccountedFor:        base.HexToAddress(opts.AccountFor),
				Sender:              t.Sender,
				Recipient:           t.Recipient,
				BegBal:              base.NewBigInt(12),
				AmountNet:           base.NewBigInt(13),
				EndBal:              base.NewBigInt(14),
				ReconciliationType:  "junk",
				Reconciled:          true,
				TotalIn:             base.NewBigInt(15),
				AmountIn:            t.Amount,
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
				PrevAppBlk:          tx.BlockNumber - 1,
				PrevBal:             base.NewBigInt(31),
				BegBalDiff:          base.NewBigInt(32),
				EndBalDiff:          base.NewBigInt(33),
				EndBalCalc:          base.NewBigInt(34),
			}
			ret = append(ret, s)
		}
	}

	return ret, nil
}

func (opts *TransactionsOptions) TransactionToTransfer(trans *types.SimpleTransaction) (transfer *types.SimpleTransfer) {
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
