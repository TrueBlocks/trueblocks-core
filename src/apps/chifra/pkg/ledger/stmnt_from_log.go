package ledger

import (
	"fmt"
	"math/big"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/token"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// GetStatementFromLog returns a statement from a given log
func (l *Ledger) GetStatementFromLog(log *types.SimpleLog) (r *types.SimpleStatement, err error) {
	if !rpcClient.IsTransferTopic(log.Topics[0].Hex()) || len(log.Topics) < 3 {
		// isn't a transfer, return silently
		return nil, nil
	}

	sym := log.Address.Prefix(6)
	decimals := uint64(18)
	name := l.Names[log.Address]
	if name.Address == log.Address {
		if name.Symbol != "" {
			sym = name.Symbol
		}
		if name.Decimals != 0 {
			decimals = name.Decimals
		}
	}

	key := l.CtxKey(log.BlockNumber, log.TransactionIndex)
	ctx := l.Contexts[key]

	pBal := new(big.Int)
	if pBal, err = token.GetTokenBalanceAt(l.Chain, log.Address, l.AccountFor, fmt.Sprintf("0x%x", ctx.PrevBlock)); pBal == nil {
		return nil, err
	}

	bBal := new(big.Int)
	if bBal, err = token.GetTokenBalanceAt(l.Chain, log.Address, l.AccountFor, fmt.Sprintf("0x%x", ctx.CurBlock-1)); bBal == nil {
		return nil, err
	}

	eBal := new(big.Int)
	if eBal, err = token.GetTokenBalanceAt(l.Chain, log.Address, l.AccountFor, fmt.Sprintf("0x%x", ctx.CurBlock)); eBal == nil {
		return nil, err
	}

	b := strings.Replace(log.Data, "0x", "", -1)
	val := big.NewInt(0)
	if val, _ = new(big.Int).SetString(b, 16); val == nil {
		val = big.NewInt(0)
	}
	ret := types.SimpleStatement{
		AccountedFor:     l.AccountFor,
		Sender:           base.HexToAddress(log.Topics[1].Hex()),
		Recipient:        base.HexToAddress(log.Topics[2].Hex()),
		BlockNumber:      log.BlockNumber,
		TransactionIndex: log.TransactionIndex,
		LogIndex:         log.LogIndex,
		TransactionHash:  log.TransactionHash,
		Timestamp:        log.Timestamp,
		AssetAddr:        log.Address,
		AssetSymbol:      sym,
		Decimals:         decimals,
		SpotPrice:        0.0,
		PriceSource:      "not-priced",
		PrevAppBlk:       ctx.PrevBlock,
		PrevBal:          *pBal,
		BegBal:           *bBal,
		EndBal:           *eBal,
	}

	ofInterst := false
	if l.AccountFor == ret.Sender {
		ret.AmountOut = *val
		ofInterst = true
	}

	// Do not collapse, may be both (self-send)
	if l.AccountFor == ret.Recipient {
		ret.AmountIn = *val
		ofInterst = true
	}

	if ofInterst {
		if !l.TrialBalance("TOKENS", &ret) {
			logger.Warn("Transaction", fmt.Sprintf("%d.%d.%d", ret.BlockNumber, ret.TransactionIndex, ret.LogIndex), "does not reconcile")
		}
	}

	return &ret, nil
}
