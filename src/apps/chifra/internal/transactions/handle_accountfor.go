package transactionsPkg

import (
	"context"
	"errors"
	"fmt"
	"math/big"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/ethereum/go-ethereum"
	"github.com/ethereum/go-ethereum/common"
)

var zero = &big.Int{}

func (opts *TransactionsOptions) HandleAccounting() (err error) {
	chain := opts.Globals.Chain
	testMode := opts.Globals.TestMode
	ether := opts.Globals.Ether
	acctFor := base.HexToAddress(opts.AccountFor)

	ledgers := NewLedger(chain, base.HexToAddress(opts.AccountFor), ether, testMode)
	for _, rng := range opts.TransactionIds {
		txIds, err := rng.ResolveTxs(chain)
		if err != nil && !errors.Is(err, ethereum.NotFound) {
			return err
		}
		for i := 0; i < len(txIds); i++ {
			key := fmt.Sprintf("%09d-%05d", txIds[i].BlockNumber, txIds[i].TransactionIndex)
			prev := uint32(0)
			if txIds[i].BlockNumber > 0 {
				prev = txIds[i].BlockNumber - 1
			}
			next := txIds[i].BlockNumber + 1
			cur := txIds[i].BlockNumber
			getReconType := func(isPrevDiff bool, isNextDiff bool) (reconType string) {
				if isPrevDiff && isNextDiff {
					return "regular"
				} else if !isPrevDiff && !isNextDiff {
					return "both-not-diff"
				} else if isPrevDiff {
					return "prevDiff"
				} else if isNextDiff {
					return "nextDiff"
				} else {
					return "unknown"
				}
			}
			ctext := Context{
				PrevBlock:  base.Blknum(prev),
				CurBlock:   base.Blknum(cur),
				NextBlock:  base.Blknum(next),
				isPrevDiff: prev != cur,
				isNextDiff: cur != next,
				ReconType:  getReconType(prev != cur, cur != next),
			}
			ledgers.Contexts[key] = ctext
		}
	}

	ctx, cancel := context.WithCancel(context.Background())
	fetchData := func(modelChan chan types.Modeler[types.RawStatement], errorChan chan error) {
		for _, rng := range opts.TransactionIds {
			txIds, err := rng.ResolveTxs(chain)
			if err != nil && !errors.Is(err, ethereum.NotFound) {
				errorChan <- err
				cancel()
			}

			for _, app := range txIds {
				if statements, err := ledgers.GetStatementsFromAppearance(chain, acctFor, &app); err != nil {
				} else {
					for _, statement := range statements {
						if statement.AmountNet().Cmp(zero) != 0 {
							modelChan <- &statement
						}
					}
				}
			}
		}
	}

	extra := map[string]interface{}{
		"ether": opts.Globals.Ether,
	}
	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOptsWithExtra(extra))
}

// var zero = &big.Int{}

func (ledgers *Ledger) GetStatementsFromAppearance(chain string, acctFor base.Address, app *types.RawAppearance) (statements []types.SimpleStatement, err error) {
	if app == nil {
		return nil, nil
	}

	ret := []types.SimpleStatement{}
	if true { // app.IsToken() {
		if tx, err := rpcClient.GetTransactionByAppearance(chain, app, false); err != nil {
			return ret, err
		} else {
			if s := ledgers.GetStatementFromTransaction(tx); s != nil {
				if s.Sender == acctFor || s.Recipient == acctFor {
					ret = append(ret, *s)
					ledgers.Previous[s.AssetAddr] = *s
				}
			}

			for _, log := range tx.Receipt.Logs {
				if s := ledgers.GetStatementFromLog(&log); s != nil {
					if s.Sender == acctFor || s.Recipient == acctFor {
						ret = append(ret, *s)
						ledgers.Previous[s.AssetAddr] = *s
					}
				}
			}

			if len(ret) > 0 && ledgers.TestMode {
				fmt.Println("TIME ~ CLOCK - <INFO>  : Calling 0x")
				fmt.Println("TIME ~ CLOCK - <INFO>  : call to 0x")
			}
		}
	}

	return ret, nil
}

func (ledgers *Ledger) GetStatementFromTransaction(trans *types.SimpleTransaction) (statement *types.SimpleStatement) {
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
		AssetAddr:        FAKE_ETH_ADDRESS,
		AssetSymbol:      "WEI",
		Decimals:         18,
		SpotPrice:        1.0,
		PriceSource:      "uniswap",
		PrevAppBlk:       ctx.PrevBlock,
		PrevBal:          *pBal,
		BegBal:           *bBal,
		EndBal:           *eBal,
	}

	ofInterst := false
	if ledgers.AccountFor == ret.Sender {
		ret.AmountOut = trans.Value
		ret.GasOut = *gasOut
		ofInterst = true
	}
	// Do not collapse, may be both (self-send)
	if ledgers.AccountFor == ret.Recipient {
		ret.AmountIn = trans.Value
		ofInterst = true
	}

	if ledgers.AsEther {
		ret.AssetSymbol = "ETH"
	}

	if len(trans.Input) > 10 {
		ret.Encoding = trans.Input[:10]
	}

	if ofInterst {
		ledgers.TrialBalance("flows-top", &ret)
		ledgers.TrialBalance("balances-top", &ret)
	} else {
		logger.TestLog(ledgers.TestMode, "Transaction", fmt.Sprintf("%d.%d", trans.BlockNumber, trans.TransactionIndex), "does not transfer value")
	}

	return &ret
}

func (ledgers *Ledger) GetStatementFromLog(log *types.SimpleLog) (r *types.SimpleStatement) {
	if len(log.Topics) < 3 {
		return nil
	}

	key := fmt.Sprintf("%09d-%05d", log.BlockNumber, log.TransactionIndex)
	ctx := ledgers.Contexts[key]

	pBal, _ := rpcClient.GetBalanceAt(ledgers.Chain, common.HexToAddress(ledgers.AccountFor.Hex()), ctx.PrevBlock)
	bBal, _ := rpcClient.GetBalanceAt(ledgers.Chain, common.HexToAddress(ledgers.AccountFor.Hex()), ctx.CurBlock-1)
	eBal, _ := rpcClient.GetBalanceAt(ledgers.Chain, common.HexToAddress(ledgers.AccountFor.Hex()), ctx.CurBlock)

	sym := log.Address.Hex()[:6]
	if rpcClient.IsTransferTopic(log.Topics[0].Hex()) {
		val := new(big.Int).SetBytes([]byte(log.Data))
		ret := types.SimpleStatement{
			AccountedFor:     ledgers.AccountFor,
			Sender:           base.HexToAddress(log.Topics[1].Hex()),
			Recipient:        base.HexToAddress(log.Topics[2].Hex()),
			BlockNumber:      log.BlockNumber,
			TransactionIndex: log.TransactionIndex,
			LogIndex:         log.LogIndex,
			TransactionHash:  log.TransactionHash,
			Timestamp:        log.Timestamp,
			AssetAddr:        log.Address,
			AssetSymbol:      sym,
			Decimals:         18,
			SpotPrice:        1.0,
			PriceSource:      "uniswap",
			PrevAppBlk:       ctx.PrevBlock,
			PrevBal:          *pBal,
			BegBal:           *bBal,
			EndBal:           *eBal,
		}

		ofInterst := false
		if ledgers.AccountFor == ret.Sender {
			ret.AmountOut = *val
			ofInterst = true
		}
		// Do not collapse, may be both (self-send)
		if ledgers.AccountFor == ret.Recipient {
			ret.AmountIn = *val
			ofInterst = true
		}

		if ofInterst {
			ledgers.TrialBalance("flows-token", &ret)
			ledgers.TrialBalance("balances-token", &ret)
		} else {
			logger.TestLog(ledgers.TestMode, "Log", log.LogIndex, "at", fmt.Sprintf("%d.%d", log.BlockNumber, log.TransactionIndex), "(a token transfer) is not relevant")
		}

		return &ret
	}
	return nil
}

type Context struct {
	PrevBlock  base.Blknum
	CurBlock   base.Blknum
	NextBlock  base.Blknum
	isPrevDiff bool
	isNextDiff bool
	ReconType  string
}

type Ledger struct {
	Chain      string
	AccountFor base.Address
	TestMode   bool
	Previous   map[base.Address]types.SimpleStatement
	Contexts   map[string]Context
	AsEther    bool
}

func NewLedger(chain string, acctFor base.Address, asEther, testMode bool) *Ledger {
	return &Ledger{
		Chain:      chain,
		AccountFor: acctFor,
		TestMode:   testMode,
		Previous:   make(map[base.Address]types.SimpleStatement),
		Contexts:   make(map[string]Context),
		AsEther:    asEther,
	}
}

var FAKE_ETH_ADDRESS = base.HexToAddress("0xeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee")

func (l *Ledger) TrialBalance(msg string, r *types.SimpleStatement) {
	key := fmt.Sprintf("%09d-%05d", r.BlockNumber, r.TransactionIndex)
	ctx := l.Contexts[key]
	r.ReconciliationType = ctx.ReconType
	if r.AssetAddr == FAKE_ETH_ADDRESS {
		r.ReconciliationType += "-eth"
	} else {
		r.ReconciliationType += "-token"
	}

	logger.TestLog(true, "")
	logger.TestLog(true, "===================================================")
	logger.TestLog(true, "ledger.blockNumber:    ", ctx.PrevBlock)
	logger.TestLog(true, "prevBlock:             ", ctx.PrevBlock)
	logger.TestLog(true, "transfer.blockNumber:  ", r.BlockNumber)
	logger.TestLog(true, "nextBlock:             ", ctx.NextBlock)
	logger.TestLog(true, "isPrevDiff:            ", ctx.PrevBlock != r.BlockNumber)
	logger.TestLog(true, "isNextDiff:            ", ctx.NextBlock != r.BlockNumber)
	logger.TestLog(true, fmt.Sprintf("-------------%s-----------------------------", msg))
	logger.TestLog(true, "Trial balance:")
	logger.TestLog(true, r.ReconciliationType)
	logger.TestLog(true, "   accountedFor:       ", r.AccountedFor)
	logger.TestLog(true, "   assetAddr:          ", r.AssetAddr)
	logger.TestLog(true, "   assetSymbol:        ", r.AssetSymbol)
	logger.TestLog(true, "   hash:               ", r.TransactionHash)
	logger.TestLog(true, "   blockNumber:        ", r.BlockNumber)
	logger.TestLog(true, "   transactionIndex:   ", r.TransactionIndex)
	logger.TestLog(true, "   logIndex:           ", r.LogIndex)
	logger.TestLog(true, "   begBal:             ", r.BegBal.Text(10))
	logger.TestLog(true, "   amountIn:           ", r.AmountIn.Text(10))
	logger.TestLog(true, "   totalIn:            ", r.TotalIn().Text(10))
	logger.TestLog(true, "   amountNet:          ", r.AmountNet().Text(10))
	logger.TestLog(true, "   endBal:             ", r.EndBal.Text(10))
	logger.TestLog(true, "   trialBalance:       ", r.Reconciled())
	logger.TestLog(true, "---------------------------------------------------")
}
