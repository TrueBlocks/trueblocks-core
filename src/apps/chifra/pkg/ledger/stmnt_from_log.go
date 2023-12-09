package ledger

import (
	"fmt"
	"math/big"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

var transferTopic = base.HexToHash(
	"0xddf252ad1be2c89b69c2b068fc378daa952ba7f163c4a11628f55a4df523b3ef",
)

// getStatementFromLog returns a statement from a given log
func (l *Ledger) getStatementFromLog(conn *rpc.Connection, log *types.SimpleLog) (types.SimpleStatement, error) {
	if len(log.Topics) < 3 || log.Topics[0] != transferTopic {
		// TODO: Too short topics happens (sometimes) because the ABI says that the data is not
		// TODO: index, but it is or visa versa. In either case, we get the same topic0. We need to
		// TODO: attempt both with and without indexed parameters. See issues/1366.
		return types.SimpleStatement{}, nil
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

	// TODO: BOGUS PERF - WE HIT GETBALANCE THREE TIMES FOR EACH APPEARANCE. SPIN THROUGH ONCE
	// TODO: AND CACHE RESULTS IN MEMORY, BUT BE CAREFUL OF MULTIPLE LOGS PER BLOCK (OR TRANSACTION)
	key := l.ctxKey(log.BlockNumber, log.TransactionIndex)
	ctx := l.Contexts1[key]

	var err error
	pBal := new(big.Int)
	if pBal, err = conn.GetTokenBalanceAt(log.Address, l.AccountFor, fmt.Sprintf("0x%x", ctx.PrevBlock)); pBal == nil {
		return types.SimpleStatement{}, err
	}

	bBal := new(big.Int)
	if bBal, err = conn.GetTokenBalanceAt(log.Address, l.AccountFor, fmt.Sprintf("0x%x", ctx.CurBlock-1)); bBal == nil {
		return types.SimpleStatement{}, err
	}

	eBal := new(big.Int)
	if eBal, err = conn.GetTokenBalanceAt(log.Address, l.AccountFor, fmt.Sprintf("0x%x", ctx.CurBlock)); eBal == nil {
		return types.SimpleStatement{}, err
	}

	var amt *big.Int
	if amt, _ = new(big.Int).SetString(strings.Replace(log.Data, "0x", "", -1), 16); amt == nil {
		amt = big.NewInt(0)
	}

	s := types.SimpleStatement{
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

	ofInterest := false

	// Do not collapse, may be both
	if l.AccountFor == s.Sender {
		s.AmountOut = *amt
		ofInterest = true
	}

	// Do not collapse, may be both
	if l.AccountFor == s.Recipient {
		s.AmountIn = *amt
		ofInterest = true
	}

	if ofInterest {
		id := fmt.Sprintf("%d.%d.%d", s.BlockNumber, s.TransactionIndex, s.LogIndex)
		if !l.trialBalance("TOKENS", &s) {
			logger.Warn(colors.Yellow+"Transaction", id, "does not reconcile"+colors.Off)
		} else {
			logger.Progress(true, colors.Green+"Transaction", id, "reconciled"+colors.Off)
		}
	}

	return s, nil
}
