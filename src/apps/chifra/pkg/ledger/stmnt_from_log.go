package ledger

import (
	"fmt"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

var transferTopic = base.HexToHash(
	"0xddf252ad1be2c89b69c2b068fc378daa952ba7f163c4a11628f55a4df523b3ef",
)

var ErrNonIndexedTransfer = fmt.Errorf("non-indexed transfer")

// getStatementsFromLog returns a statement from a given log
func (l *Ledger) getStatementsFromLog(conn *rpc.Connection, logIn *types.Log) (types.Statement, error) {
	if logIn.Topics[0] != transferTopic {
		// Not a transfer
		return types.Statement{}, nil
	}

	if log, err := l.normalizeLog(logIn); err != nil {
		return types.Statement{}, err

	} else {
		sym := log.Address.Prefix(6)
		decimals := base.Value(18)
		name := l.Names[log.Address]
		if name.Address == log.Address {
			if name.Symbol != "" {
				sym = name.Symbol
			}
			if name.Decimals != 0 {
				decimals = base.Value(name.Decimals)
			}
		}

		sender := base.HexToAddress(log.Topics[1].Hex())
		recipient := base.HexToAddress(log.Topics[2].Hex())
		var amountIn, amountOut base.Wei
		var amt *base.Wei
		if amt, _ = new(base.Wei).SetString(strings.Replace(log.Data, "0x", "", -1), 16); amt == nil {
			amt = base.NewWei(0)
		}

		ofInterest := false

		// Do not collapse, may be both
		if l.AccountFor == sender {
			amountOut = *amt
			ofInterest = true
		}

		// Do not collapse, may be both
		if l.AccountFor == recipient {
			amountIn = *amt
			ofInterest = true
		}

		s := types.Statement{
			AccountedFor:     l.AccountFor,
			Sender:           sender,
			Recipient:        recipient,
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
			AmountIn:         amountIn,
			AmountOut:        amountOut,
		}

		key := l.ctxKey(log.BlockNumber, log.TransactionIndex, s.AccountedFor)
		ctx := l.Contexts[key]

		if ofInterest {
			var err error

			// Previous balance:
			var pBal *base.Wei
			if (ctx.ReconType & (types.SameSame | types.SameDiff | types.DiffSame)) != 0 {
				pBal, err = conn.GetBalanceAtToken(log.Address, l.AccountFor, fmt.Sprintf("0x%x", ctx.PrevBlock))
				if err != nil || pBal == nil {
					return s, err
				}
			} else {
				pBal, err = conn.GetBalanceAtToken(log.Address, l.AccountFor, fmt.Sprintf("0x%x", ctx.PrevBlock))
				if err != nil || pBal == nil {
					return s, err
				}
			}
			s.PrevBal = *pBal

			var bBal *base.Wei
			if (ctx.ReconType & (types.SameSame | types.SameDiff | types.DiffSame)) != 0 {
				bBal = pBal
			} else {
				bBal, err = conn.GetBalanceAtToken(log.Address, l.AccountFor, fmt.Sprintf("0x%x", ctx.CurBlock-1))
				if err != nil || bBal == nil {
					return s, err
				}
			}
			s.BegBal = *bBal

			eBal := new(base.Wei)
			eBal, err = conn.GetBalanceAtToken(log.Address, l.AccountFor, fmt.Sprintf("0x%x", ctx.CurBlock))
			if err != nil || eBal == nil {
				return s, err
			}
			s.EndBal = *eBal

			id := fmt.Sprintf(" %d.%d.%d", s.BlockNumber, s.TransactionIndex, s.LogIndex)
			if !l.trialBalance("token", &s) {
				if !utils.IsFuzzing() {
					logger.Warn(colors.Yellow+"Log statement at ", id, " does not reconcile."+colors.Off)
				}
			} else {
				if !utils.IsFuzzing() {
					logger.Progress(true, colors.Green+"Transaction", id, "reconciled       "+colors.Off)
				}
			}
		}

		return s, nil
	}
}
