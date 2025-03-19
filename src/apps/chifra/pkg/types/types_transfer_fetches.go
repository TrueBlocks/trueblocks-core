package types

import (
	"slices"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/topics"
)

func (trans *Transaction) FetchStatement(asEther bool, asset, holder base.Address) (*Statement, error) {
	sym := "WEI"
	if asEther {
		sym = "ETH"
	}
	to := trans.To
	if trans.To.IsZero() && trans.Receipt != nil && !trans.Receipt.ContractAddress.IsZero() {
		to = trans.Receipt.ContractAddress
	}

	stmt := &Statement{
		AccountedFor:     holder,
		Holder:           holder,
		Asset:            asset,
		Symbol:           sym,
		Decimals:         18,
		BlockNumber:      trans.BlockNumber,
		TransactionIndex: trans.TransactionIndex,
		TransactionHash:  trans.Hash,
		Timestamp:        trans.Timestamp,
		Sender:           trans.From,
		Recipient:        to,
		PriceSource:      "not-priced",
	}

	// logger.TestLog(true, "holder", holder, "sender", stmt.Sender)
	if stmt.Sender == holder {
		gasUsed := new(base.Wei)
		if trans.Receipt != nil {
			gasUsed.SetUint64(uint64(trans.Receipt.GasUsed))
		}
		gasPrice := new(base.Wei).SetUint64(uint64(trans.GasPrice))
		gasOut := new(base.Wei).Mul(gasUsed, gasPrice)
		stmt.AmountOut = trans.Value
		stmt.GasOut = *gasOut
	}

	// logger.TestLog(true, "holder", holder, "recipient", stmt.Recipient)
	if stmt.Recipient == holder {
		if stmt.BlockNumber == 0 {
			stmt.PrefundIn = trans.Value
		} else {
			if trans.Rewards != nil {
				stmt.MinerBaseRewardIn = trans.Rewards.Block
				stmt.MinerNephewRewardIn = trans.Rewards.Nephew
				stmt.MinerTxFeeIn = trans.Rewards.TxFee
				stmt.MinerUncleRewardIn = trans.Rewards.Uncle
			} else {
				stmt.AmountIn = trans.Value
			}
		}
	}

	return stmt, nil
}

func (trans *Transaction) FetchStatementFromTraces(traces []Trace, accountedFor base.Address, asEther bool) (*Statement, error) {
	if stmt, err := trans.FetchStatement(asEther, base.FAKE_ETH_ADDRESS, accountedFor); err != nil {
		return nil, err

	} else {
		for i, trace := range traces {
			// the first trace is identical to the transaction itself, so we can skip it
			if i > 0 {
				// updates statement in place...
				if err := trace.UpdateStatement(stmt); err != nil {
					return nil, err
				}
			}
		}
		return stmt, nil
	}
}

func (t *Trace) UpdateStatement(stmt *Statement) error {
	// delegate calls are not included in the transaction's gas cost, so we skip them
	if t.Action.CallType == "delegatecall" && t.Action.To != stmt.AccountedFor {
		return nil
	}

	plusEq := func(a1, a2 *base.Wei) base.Wei {
		return *a1.Add(a1, a2)
	}

	// Do not collapse, more than one of these can be true at the same time
	if t.Action.From == stmt.AccountedFor {
		stmt.InternalOut = plusEq(&stmt.InternalOut, &t.Action.Value)
		stmt.Sender = t.Action.From
		if t.Action.To.IsZero() {
			if t.Result != nil {
				stmt.Recipient = t.Result.Address
			}
		} else {
			stmt.Recipient = t.Action.To
		}
	}

	if t.Action.To == stmt.AccountedFor {
		stmt.InternalIn = plusEq(&stmt.InternalIn, &t.Action.Value)
		stmt.Sender = t.Action.From
		stmt.Recipient = t.Action.To
	}

	if t.Action.SelfDestructed == stmt.AccountedFor {
		stmt.SelfDestructOut = plusEq(&stmt.SelfDestructOut, &t.Action.Balance)
		stmt.Sender = t.Action.SelfDestructed
		if stmt.Sender.IsZero() {
			stmt.Sender = t.Action.Address
		}
		stmt.Recipient = t.Action.RefundAddress
	}

	if t.Action.RefundAddress == stmt.AccountedFor {
		stmt.SelfDestructIn = plusEq(&stmt.SelfDestructIn, &t.Action.Balance)
		stmt.Sender = t.Action.SelfDestructed
		if stmt.Sender.IsZero() {
			stmt.Sender = t.Action.Address
		}
		stmt.Recipient = t.Action.RefundAddress
	}

	if t.Action.Address == stmt.AccountedFor && !t.Action.RefundAddress.IsZero() {
		stmt.SelfDestructOut = plusEq(&stmt.SelfDestructOut, &t.Action.Balance)
		// self destructed send
		stmt.Sender = t.Action.Address
		stmt.Recipient = t.Action.RefundAddress
	}

	if t.Result != nil {
		if t.Result.Address == stmt.AccountedFor {
			stmt.InternalIn = plusEq(&stmt.InternalIn, &t.Action.Value)
			stmt.Sender = t.Action.From
			stmt.Recipient = t.Result.Address
		}
	}

	return nil
}

func (s *Receipt) FetchStatements(accountedFor base.Address, assetFilters []base.Address, appFilter *AppearanceFilter) ([]Statement, error) {
	statements := make([]Statement, 0, 20)
	for _, log := range s.Logs {
		isTransfer := log.Topics[0] == topics.TransferTopic
		isOfIterest := IsAssetOfInterest(log.Address, assetFilters)
		passesFilter := appFilter.ApplyLogFilter(&log, []base.Address{accountedFor})
		if isTransfer && isOfIterest && passesFilter {
			if stmt, err := log.FetchStatement(accountedFor); err != nil {
				// TODO: silent fail?
				continue
			} else if stmt == nil {
				continue
			} else {
				statements = append(statements, *stmt)
			}
		}
	}
	if len(statements) > 0 {
		statements[0].BegSentinel = true
		statements[len(statements)-1].EndSentinel = true
	}
	return statements, nil
}

func (log *Log) FetchStatement(accountedFor base.Address) (*Statement, error) {
	if normalized, err := NormalizeKnownLogs(log); err != nil {
		return nil, err
	} else if normalized.IsNFT() {
		return nil, nil
	} else {
		sender := base.HexToAddress(normalized.Topics[1].Hex())
		recipient := base.HexToAddress(normalized.Topics[2].Hex())
		isSender, isRecipient := accountedFor == sender, accountedFor == recipient
		if !isSender && !isRecipient {
			return nil, nil
		}

		var amountIn, amountOut base.Wei
		amount, _ := new(base.Wei).SetString(strings.ReplaceAll(normalized.Data, "0x", ""), 16)
		if amount == nil {
			amount = base.ZeroWei
		}
		if accountedFor == sender {
			amountOut = *amount
		}
		if accountedFor == recipient {
			amountIn = *amount
		}
		stmt := &Statement{
			AccountedFor:     accountedFor,
			Holder:           accountedFor,
			Sender:           sender,
			Recipient:        recipient,
			BlockNumber:      normalized.BlockNumber,
			TransactionIndex: normalized.TransactionIndex,
			LogIndex:         normalized.LogIndex,
			TransactionHash:  normalized.TransactionHash,
			Timestamp:        normalized.Timestamp,
			Asset:            normalized.Address,
			PriceSource:      "not-priced",
			AmountIn:         amountIn,
			AmountOut:        amountOut,
			Decimals:         18,
		}
		return stmt, nil
	}
}

// ---------------------------------------------------------
func IsAssetOfInterest(needle base.Address, filters []base.Address) bool {
	if len(filters) == 0 {
		return true
	}
	return slices.ContainsFunc(filters, func(asset base.Address) bool {
		return asset.Hex() == needle.Hex()
	})
}
