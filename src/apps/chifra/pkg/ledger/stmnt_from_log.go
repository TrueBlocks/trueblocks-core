package ledger

import (
	"fmt"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/normalize"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/topics"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

// getStatementsFromLog returns a statement from a given log
func (l *Ledger) getStatementsFromLog(pos *types.AppPosition, trans *types.Transaction, logIn *types.Log) (types.Statement, error) {
	if logIn.Topics[0] != topics.TransferTopic {
		return types.Statement{}, nil
	}

	// TODO: BOGUS NOT DONE
	// This is an NFT, probably should not try to balance it
	// if len(logIn.Topics) == 4 {
	// 	// an ERC721 token transfer - same topic[0], different semantics
	// 	return types.Statement{}, nil
	// }

	if log, err := normalize.NormalizeTransferOrApproval(logIn); err != nil {
		return types.Statement{}, err

	} else {
		sym := log.Address.DefaultSymbol()
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
			AssetAddress:     log.Address,
			AssetSymbol:      sym,
			Decimals:         decimals,
			SpotPrice:        0.0,
			PriceSource:      "not-priced",
			AmountIn:         amountIn,
			AmountOut:        amountOut,
		}

		key := l.getAppBalancerKey(log.BlockNumber, log.TransactionIndex)
		var ctx *appBalancer
		var exists bool
		if ctx, exists = l.appBalancers[key]; !exists {
			return s, fmt.Errorf("no context for %s", key)
		}
		validatePosition(pos, ctx)

		if ofInterest {
			s.PostFirst = ctx.first
			s.PostLast = ctx.last

			var err error
			pBal := new(base.Wei)
			pBal, err = l.connection.GetBalanceAtToken(log.Address, l.accountFor, fmt.Sprintf("0x%x", ctx.Prev()))
			if err != nil || pBal == nil {
				return s, err
			}
			s.PrevBal = *pBal

			bBal := new(base.Wei)
			bBal, err = l.connection.GetBalanceAtToken(log.Address, l.accountFor, fmt.Sprintf("0x%x", ctx.Cur()-1))
			if err != nil || bBal == nil {
				return s, err
			}
			s.BegBal = *bBal

			eBal := new(base.Wei)
			eBal, err = l.connection.GetBalanceAtToken(log.Address, l.accountFor, fmt.Sprintf("0x%x", ctx.Cur()))
			if err != nil || eBal == nil {
				return s, err
			}
			s.EndBal = *eBal

			id := fmt.Sprintf(" %d.%d.%d", s.BlockNumber, s.TransactionIndex, s.LogIndex)
			t := types.TrialBalToken
			if len(log.Topics) == 4 {
				t = types.TrialBalNft
			}
			if !l.trialBalance(pos, t, trans, &s) {
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
