package ledger

import (
	"fmt"
	"math/big"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/articulate"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// getStatementFromLog returns a statement from a given log
func (l *Ledger) getStatementFromLog(conn *rpc.Connection, log *types.SimpleLog) (r *types.SimpleStatement, err error) {
	if len(log.Topics) < 3 || log.Topics[0] != articulate.TransferTopic {
		// TODO: Too short topics happens (sometimes) because the ABI says that the data is not
		// TODO: index, but it is or visa versa. In either case, we get the same topic0. We need to
		// TODO: attempt both with and without indexed parameters. See issues/1366.
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

	key := l.ctxKey(log.BlockNumber, log.TransactionIndex)
	ctx := l.Contexts[key]

	pBal := new(big.Int)
	if pBal, err = conn.GetTokenBalanceAt(log.Address, l.AccountFor, fmt.Sprintf("0x%x", ctx.PrevBlock)); pBal == nil {
		return nil, err
	}

	bBal := new(big.Int)
	if bBal, err = conn.GetTokenBalanceAt(log.Address, l.AccountFor, fmt.Sprintf("0x%x", ctx.CurBlock-1)); bBal == nil {
		return nil, err
	}

	eBal := new(big.Int)
	if eBal, err = conn.GetTokenBalanceAt(log.Address, l.AccountFor, fmt.Sprintf("0x%x", ctx.CurBlock)); eBal == nil {
		return nil, err
	}

	b := strings.Replace(log.Data, "0x", "", -1)
	var val *big.Int
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
		if !l.trialBalance("TOKENS", &ret) {
			logger.Warn(colors.Yellow+"Transaction", fmt.Sprintf("%d.%d.%d", ret.BlockNumber, ret.TransactionIndex, ret.LogIndex), "does not reconcile"+colors.Off)
		} else {
			logger.Progress(true, colors.Green+"Transaction", fmt.Sprintf("%d.%d.%d", ret.BlockNumber, ret.TransactionIndex, ret.LogIndex), "reconciled"+colors.Off)
		}
	}

	return &ret, nil
}
