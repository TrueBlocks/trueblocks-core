package ledger

import (
	"errors"
	"fmt"
	"math/big"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/identifiers"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/token"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/ethereum/go-ethereum"
	"github.com/ethereum/go-ethereum/common"
)

// Context is a struct to hold the context of a reconciliation (i.e., its previous and next blocks and whether they are different)
type Context struct {
	PrevBlock  base.Blknum
	CurBlock   base.Blknum
	NextBlock  base.Blknum
	IsPrevDiff bool
	IsNextDiff bool
	ReconType  string
}

// Ledger is a struct that holds the context for each appearance and, for each asset encountered, the previous reconcilation record for balance accounting
type Ledger struct {
	Chain      string
	AccountFor base.Address
	TestMode   bool
	Previous   map[base.Address]*types.SimpleStatement
	Contexts   map[string]Context
	AsEther    bool
	NoZero     bool
}

// NewLedger returns a new empty Ledger struct
func NewLedger(chain string, acctFor base.Address, asEther, testMode, noZero bool) *Ledger {
	return &Ledger{
		Chain:      chain,
		AccountFor: acctFor,
		TestMode:   testMode,
		Previous:   make(map[base.Address]*types.SimpleStatement),
		Contexts:   make(map[string]Context),
		AsEther:    asEther,
		NoZero:     noZero,
	}
}

// GetStatementsFromAppearance returns a list of statements from a given appearance
func (ledgers *Ledger) GetStatementsFromAppearance(chain string, acctFor base.Address, app *types.RawAppearance) (statements []types.SimpleStatement, err error) {
	var tx *types.SimpleTransaction
	if tx, err = rpcClient.GetTransactionByAppearance(chain, app, false); err != nil {
		return []types.SimpleStatement{}, err

	} else {
		if s := ledgers.GetStatementFromTransaction(tx); s != nil {
			if s.Sender == acctFor || s.Recipient == acctFor {
				add := !ledgers.NoZero || len(s.AmountNet().Bits()) != 0
				if add {
					statements = append(statements, *s)
					ledgers.Previous[s.AssetAddr] = s
				}
			}
		} else if err != nil {
			return []types.SimpleStatement{}, err
		}

		for _, log := range tx.Receipt.Logs {
			if s, err := ledgers.GetStatementFromLog(&log); s != nil {
				if s.Sender == acctFor || s.Recipient == acctFor {
					add := !ledgers.NoZero || len(s.AmountNet().Bits()) != 0
					if add {
						statements = append(statements, *s)
						ledgers.Previous[s.AssetAddr] = s
					}
				}
			} else if err != nil {
				return []types.SimpleStatement{}, err
			}
		}

		for _, statement := range statements {
			ledgers.PriceUsd(chain, &statement)
		}

		return
	}
}

// PriceUsd returns the price of the asset in USD
func (ledgers *Ledger) PriceUsd(chain string, statement *types.SimpleStatement) (err error) {
	// contract, err := token.GetContract(chain, statement.AssetAddr)
	uniswapV2 := "0xa478c2975ab1ea89e8196811f51a7b7ade33eb11"
	uniswapFactoryV2 := "0x5c69bee701ef814a2b6a3edd4b1652cb9cc5aa6f"
	getPair := "0xe6a43905"
	val1 := "0x000000000000000000000000a478c2975ab1ea89e8196811f51a7b7ade33eb11"
	getReservesSelector := "0x0902f1ac"
	val2 := "0x00000000000000000000000000000000000000000005c87e96b075ac37ad823e00000000000000000000000000000000000000000000011c1b48e046b82f1e740000000000000000000000000000000000000000000000000000000062d36ab4"
	logger.TestLog(true, "call to", uniswapFactoryV2, "at block", statement.BlockNumber, "at four-byte", getPair, "returned", val1)
	logger.TestLog(true, "call to", uniswapV2, "at block", statement.BlockNumber, "at four-byte", getReservesSelector, "returned", val2)
	// TEST[DATE|TIME] r1:                  0x6b175474e89094c44da98b954eedeac495271d0f
	// TEST[DATE|TIME] r2:                  0xc02aaa39b223fe8d0a0e5c4f27ead9083c756cc2
	// TEST[DATE|TIME] reserve1:            6.4673e+06
	// TEST[DATE|TIME] reserve2:            5779.86
	// TEST[DATE|TIME] priceOut:            1334.0296
	// TEST[DATE|TIME] reversed:            0
	return nil
}

// GetStatementFromTransaction returns a statement from a given transaction
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
		ledgers.TrialBalance("FLOW ETH", &ret)
		ledgers.TrialBalance("BALANCE ETH", &ret)
	} else {
		logger.TestLog(ledgers.TestMode, "Transaction", fmt.Sprintf("%d.%d", trans.BlockNumber, trans.TransactionIndex), "does not transfer value")
	}

	return &ret
}

// GetStatementFromLog returns a statement from a given log
func (ledgers *Ledger) GetStatementFromLog(log *types.SimpleLog) (r *types.SimpleStatement, err error) {
	if !rpcClient.IsTransferTopic(log.Topics[0].Hex()) || len(log.Topics) < 3 {
		// isn't a transfer, return silently
		return nil, nil
	}

	sym := log.Address.Hex()[:6]
	key := fmt.Sprintf("%09d-%05d", log.BlockNumber, log.TransactionIndex)
	ctx := ledgers.Contexts[key]

	pBal := new(big.Int)
	if pBal, err = token.GetBalanceAt(ledgers.Chain, log.Address, ledgers.AccountFor, fmt.Sprintf("0x%x", ctx.PrevBlock)); pBal == nil {
		return nil, err
	}

	bBal := new(big.Int)
	if bBal, err = token.GetBalanceAt(ledgers.Chain, log.Address, ledgers.AccountFor, fmt.Sprintf("0x%x", ctx.CurBlock-1)); bBal == nil {
		return nil, err
	}

	eBal := new(big.Int)
	if eBal, err = token.GetBalanceAt(ledgers.Chain, log.Address, ledgers.AccountFor, fmt.Sprintf("0x%x", ctx.CurBlock)); eBal == nil {
		return nil, err
	}

	b := strings.Replace(log.Data, "0x", "", -1)
	val, _ := new(big.Int).SetString(b, 16)
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
		ledgers.TrialBalance("FLOW TOKEN", &ret)
		ledgers.TrialBalance("BALANCE TOKEN", &ret)
	} else {
		logger.TestLog(ledgers.TestMode, "Log", log.LogIndex, "at", fmt.Sprintf("%d.%d", log.BlockNumber, log.TransactionIndex), "(a token transfer) is not relevant")
	}

	return &ret, nil
}

func (l *Ledger) SetContexts(chain string, txIds []identifiers.Identifier) error {
	for _, rng := range txIds {
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
				IsPrevDiff: prev != cur,
				IsNextDiff: cur != next,
				ReconType:  getReconType(prev != cur, cur != next),
			}
			l.Contexts[key] = ctext
		}
	}
	return nil
}

var FAKE_ETH_ADDRESS = base.HexToAddress("0xeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee")

// TrialBalance returns true of the reconciliation balances, false otherwise. It also prints the trial balance to the console.
func (l *Ledger) TrialBalance(msg string, r *types.SimpleStatement) bool {
	key := fmt.Sprintf("%09d-%05d", r.BlockNumber, r.TransactionIndex)
	ctx := l.Contexts[key]
	r.ReconciliationType = ctx.ReconType
	if r.AssetAddr == FAKE_ETH_ADDRESS {
		r.ReconciliationType += "-eth"
	} else {
		r.ReconciliationType += "-token"
	}

	if !l.TestMode {
		return r.Reconciled()
	}

	logger.TestLog(true, "===================================================")
	logger.TestLog(true, fmt.Sprintf("====> %s", msg))
	logger.TestLog(true, "===================================================")
	logger.TestLog(true, "ledger.blockNumber:    ", ctx.PrevBlock)
	logger.TestLog(true, "prevBlock:             ", ctx.PrevBlock)
	logger.TestLog(true, "transfer.blockNumber:  ", r.BlockNumber)
	logger.TestLog(true, "nextBlock:             ", ctx.NextBlock)
	logger.TestLog(true, "isPrevDiff:            ", ctx.PrevBlock != r.BlockNumber)
	logger.TestLog(true, "isNextDiff:            ", ctx.NextBlock != r.BlockNumber)
	logger.TestLog(true, "---------------------------------------------------")
	logger.TestLog(true, "Trial balance:")
	logger.TestLog(true, "   reconciliationType: ", r.ReconciliationType)
	logger.TestLog(true, "   accountedFor:       ", r.AccountedFor)
	logger.TestLog(true, "   sender:             ", r.Sender)
	logger.TestLog(true, "   recipient:          ", r.Recipient)
	logger.TestLog(true, "   assetAddr:          ", r.AssetAddr)
	logger.TestLog(true, "   assetSymbol:        ", r.AssetSymbol)
	logger.TestLog(true, "   decimals:           ", r.Decimals)
	logger.TestLog(true, "   prevAppBlk:         ", r.PrevAppBlk)
	logger.TestLog(true, "   hash:               ", r.TransactionHash)
	logger.TestLog(true, "   timestamp:          ", r.Timestamp)
	logger.TestLog(true, "   blockNumber:        ", r.BlockNumber)
	logger.TestLog(true, "   transactionIndex:   ", r.TransactionIndex)
	logger.TestLog(true, "   logIndex:           ", r.LogIndex)
	logger.TestLog(true, "   priceSource:        ", r.PriceSource)
	logger.TestLog(true, "   spotPrice:          ", r.SpotPrice)
	logger.TestLog(true, "   prevBal:            ", r.PrevBal.Text(10))
	logger.TestLog(true, "   begBal:             ", r.BegBal.Text(10))
	logger.TestLog(true, "   amountIn:           ", r.AmountIn.Text(10))
	logger.TestLog(true, "   internalIn:         ", r.InternalIn.Text(10))
	logger.TestLog(true, "   minerBaseRewardIn:  ", r.MinerBaseRewardIn.Text(10))
	logger.TestLog(true, "   minerNephewRewardIn:", r.MinerNephewRewardIn.Text(10))
	logger.TestLog(true, "   minerTxFeeIn:       ", r.MinerTxFeeIn.Text(10))
	logger.TestLog(true, "   minerUncleRewardIn: ", r.MinerUncleRewardIn.Text(10))
	logger.TestLog(true, "   prefundIn:          ", r.PrefundIn.Text(10))
	logger.TestLog(true, "   selfDestructIn:     ", r.SelfDestructIn.Text(10))
	logger.TestLog(true, "   totalIn:            ", r.TotalIn().Text(10))
	logger.TestLog(true, "   amountOut:          ", r.AmountOut.Text(10))
	logger.TestLog(true, "   internalOut:        ", r.InternalOut.Text(10))
	logger.TestLog(true, "   selfDestructOut:    ", r.SelfDestructOut.Text(10))
	logger.TestLog(true, "   gasOut:             ", r.GasOut.Text(10))
	logger.TestLog(true, "   totalOut:           ", r.TotalOut().Text(10))
	logger.TestLog(true, "   amountNet:          ", r.AmountNet().Text(10))
	logger.TestLog(true, "   endBal:             ", r.EndBal.Text(10))
	logger.TestLog(true, "   begBalDiff:         ", r.BegBalDiff().Text(10))
	logger.TestLog(true, "   endBalDiff:         ", r.EndBalDiff().Text(10))
	logger.TestLog(true, "   endBalCalc:         ", r.EndBalCalc().Text(10))
	logger.TestLog(true, "   trialBalance:       ", r.Reconciled())
	// Encoding            string         `json:"encoding"`
	// Signature           string         `json:"signature"`
	logger.TestLog(true, "---------------------------------------------------")

	return r.Reconciled()
}
