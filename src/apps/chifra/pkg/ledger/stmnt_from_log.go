package ledger

import (
	"fmt"
	"os"
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
func (r *Reconciler) getStatementsFromLog(pos *types.AppPosition, trans *types.Transaction, logIn *types.Log) (types.Statement, error) {
	if logIn.Topics[0] != topics.TransferTopic {
		return types.Statement{}, nil
	}

	if log, err := normalize.NormalizeKnownLogs(logIn); err != nil {
		return types.Statement{}, err

	} else if log.IsNFT() {
		return types.Statement{}, nil

	} else {
		sender := base.HexToAddress(log.Topics[1].Hex())
		recipient := base.HexToAddress(log.Topics[2].Hex())

		isSender, isRecipient := r.accountFor == sender, r.accountFor == recipient
		if utils.IsFuzzing() || (!isSender && !isRecipient) {
			return types.Statement{}, err
		}

		sym := log.Address.DefaultSymbol()
		decimals := base.Value(18)
		name := r.names[log.Address]
		if name.Address == log.Address {
			if name.Symbol != "" {
				sym = name.Symbol
			}
			if name.Decimals != 0 {
				decimals = base.Value(name.Decimals)
			}
		}

		var amountIn, amountOut base.Wei
		var amt *base.Wei
		if amt, _ = new(base.Wei).SetString(strings.Replace(log.Data, "0x", "", -1), 16); amt == nil {
			amt = base.NewWei(0)
		}

		// Do not collapse, may be both
		if r.accountFor == sender {
			amountOut = *amt
		}
		// Do not collapse, may be both
		if r.accountFor == recipient {
			amountIn = *amt
		}

		s := types.Statement{
			AccountedFor:     r.accountFor,
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
			PostFirst:        pos.First,
			PostLast:         pos.Last,
		}

		pBal, err := r.connection.GetBalanceAtToken(log.Address, r.accountFor, pos.Prev)
		if err != nil || pBal == nil {
			return s, err
		}
		s.PrevBal = *pBal

		bBal, err := r.connection.GetBalanceAtToken(log.Address, r.accountFor, trans.BlockNumber-1)
		if err != nil || bBal == nil {
			return s, err
		}
		s.BegBal = *bBal

		eBal, err := r.connection.GetBalanceAtToken(log.Address, r.accountFor, trans.BlockNumber)
		if err != nil || eBal == nil {
			return s, err
		}
		s.EndBal = *eBal

		reconciled := r.trialBalance(pos, types.TrialBalToken, trans, &s)
		if reconciled {
			id := fmt.Sprintf(" %d.%d.%d", s.BlockNumber, s.TransactionIndex, s.LogIndex)
			logger.Progress(true, colors.Green+"Transaction", id, "reconciled       "+colors.Off)
		} else {
			id := fmt.Sprintf(" %d.%d.%d", s.BlockNumber, s.TransactionIndex, s.LogIndex)
			if os.Getenv("TEST_MODE") != "true" {
				logger.Warn("Log statement at ", id, " does not reconcile.")
			}
		}
		return s, nil
	}
}
