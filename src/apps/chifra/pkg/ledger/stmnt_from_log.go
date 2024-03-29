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

var ErrNonIndexedTransfer = fmt.Errorf("non-indexed transfer")

// getStatementsFromLog returns a statement from a given log
func (l *Ledger) getStatementsFromLog(conn *rpc.Connection, logIn *types.SimpleLog) (types.SimpleStatement, error) {
	if logIn.Topics[0] != transferTopic {
		// Not a transfer
		return types.SimpleStatement{}, nil
	}

	if log, err := l.normalizeTransfer(logIn); err != nil {
		return types.SimpleStatement{}, err

	} else {
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

		sender := base.HexToAddress(log.Topics[1].Hex())
		recipient := base.HexToAddress(log.Topics[2].Hex())
		var amountIn, amountOut big.Int
		var amt *big.Int
		if amt, _ = new(big.Int).SetString(strings.Replace(log.Data, "0x", "", -1), 16); amt == nil {
			amt = big.NewInt(0)
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

		s := types.SimpleStatement{
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

		// TODO: BOGUS PERF - WE HIT GETBALANCE THREE TIMES FOR EACH APPEARANCE. SPIN THROUGH ONCE
		// TODO: AND CACHE RESULTS IN MEMORY, BUT BE CAREFUL OF MULTIPLE LOGS PER BLOCK (OR TRANSACTION)
		key := l.ctxKey(log.BlockNumber, log.TransactionIndex)
		ctx := l.Contexts[key]

		if ofInterest {
			var err error
			pBal := new(big.Int)
			if pBal, err = conn.GetBalanceAtToken(log.Address, l.AccountFor, fmt.Sprintf("0x%x", ctx.PrevBlock)); pBal == nil {
				return s, err
			}
			s.PrevBal = *pBal

			bBal := new(big.Int)
			if bBal, err = conn.GetBalanceAtToken(log.Address, l.AccountFor, fmt.Sprintf("0x%x", ctx.CurBlock-1)); bBal == nil {
				return s, err
			}
			s.BegBal = *bBal

			eBal := new(big.Int)
			if eBal, err = conn.GetBalanceAtToken(log.Address, l.AccountFor, fmt.Sprintf("0x%x", ctx.CurBlock)); eBal == nil {
				return s, err
			}
			s.EndBal = *eBal

			id := fmt.Sprintf("%d.%d.%d", s.BlockNumber, s.TransactionIndex, s.LogIndex)
			if !l.trialBalance("token", &s) {
				logger.Warn(colors.Yellow+"Transaction", id, "does not reconcile"+colors.Off)
			} else {
				logger.Progress(true, colors.Green+"Transaction", id, "reconciled"+colors.Off)
			}
		}

		return s, nil
	}
}

func (l *Ledger) normalizeTransfer(log *types.SimpleLog) (*types.SimpleLog, error) {
	if len(log.Topics) < 3 {
		// Transfer(address _from, address _to, uint256 _tokenId) - no indexed topics
		// Transfer(address indexed _from, address indexed _to, uint256 _value) - two indexed topics
		// Transfer(address indexed _from, address indexed _to, uint256 indexed _tokenId) - three indexed topics
		// TODO: This may be a transfer. Returning here is wrong. What this means is that
		// TODO:the some of the transfer's data is not indexed. Too short topics happens
		// TODO: (sometimes) because the ABI says that the data is not index, but it is
		// TODO: or visa versa. In either case, we get the same topic0. We need to
		// TODO: attempt both with and without indexed parameters. See issues/1366.
		// TODO: We could fix this and call back in recursively...
		return nil, ErrNonIndexedTransfer
	}

	return log, nil
}
