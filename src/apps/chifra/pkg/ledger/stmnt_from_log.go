package ledger

import (
	"fmt"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/topics"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

var ErrNonIndexedTransfer = fmt.Errorf("non-indexed transfer")

// getStatementsFromLog returns a statement from a given log
func (l *Ledger) getStatementsFromLog(conn *rpc.Connection, logIn *types.Log) (types.Statement, error) {
	if logIn.Topics[0] != topics.TransferTopic {
		// Not a transfer
		return types.Statement{}, nil
	}

	if log, err := l.normalizeLog(logIn); err != nil {
		return types.Statement{}, err

	} else {
		sym := log.Address.Prefix(6)
		decimals := base.Value(18)
		name := l.names[log.Address]
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
		if l.accountFor == sender {
			amountOut = *amt
			ofInterest = true
		}

		// Do not collapse, may be both
		if l.accountFor == recipient {
			amountIn = *amt
			ofInterest = true
		}

		s := types.Statement{
			AccountedFor:     l.accountFor,
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

		key := l.getAssetContextKey(log.BlockNumber, log.TransactionIndex, s.AccountedFor)
		var ctx *assetContext
		var exists bool
		if ctx, exists = l.assetContexts[key]; !exists {
			return s, fmt.Errorf("no context for %s", key)
		}

		if ofInterest {
			var err error

			// Previous balance:
			var pBal *base.Wei
			if (ctx.Recon() & (types.SameSame | types.SameDiff | types.DiffSame)) != 0 {
				pBal, err = conn.GetBalanceAtToken(log.Address, l.accountFor, fmt.Sprintf("0x%x", ctx.Prev()))
				if err != nil || pBal == nil {
					return s, err
				}
			} else {
				pBal, err = conn.GetBalanceAtToken(log.Address, l.accountFor, fmt.Sprintf("0x%x", ctx.Prev()))
				if err != nil || pBal == nil {
					return s, err
				}
			}
			s.PrevBal = *pBal

			var bBal *base.Wei
			if (ctx.Recon() & (types.SameSame | types.SameDiff | types.DiffSame)) != 0 {
				bBal = pBal
			} else {
				bBal, err = conn.GetBalanceAtToken(log.Address, l.accountFor, fmt.Sprintf("0x%x", ctx.Cur()-1))
				if err != nil || bBal == nil {
					return s, err
				}
			}
			s.BegBal = *bBal

			eBal := new(base.Wei)
			eBal, err = conn.GetBalanceAtToken(log.Address, l.accountFor, fmt.Sprintf("0x%x", ctx.Cur()))
			if err != nil || eBal == nil {
				return s, err
			}
			s.EndBal = *eBal

			id := fmt.Sprintf(" %d.%d.%d", s.BlockNumber, s.TransactionIndex, s.LogIndex)
			if !l.trialBalance(trialBalToken, &s) {
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
