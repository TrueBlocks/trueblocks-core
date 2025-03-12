package types

import "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"

func (trans *Transaction) GetStatementFromTransaction(asEther bool, asset, holder base.Address) *Statement {
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

	return stmt
}

func (trans *Transaction) GetStatementFromTraces(traces []Trace, accountedFor base.Address, asEther bool) (*Statement, error) {
	stmt := trans.GetStatementFromTransaction(asEther, base.FAKE_ETH_ADDRESS, accountedFor)
	for i, trace := range traces {
		if i == 0 {
			// the first trace is identical to the transaction itself, so we can skip it
			continue
		}

		if trace.Action.CallType == "delegatecall" && trace.Action.To != stmt.AccountedFor {
			// delegate calls are not included in the transaction's gas cost, so we skip them
			continue
		}

		plusEq := func(a1, a2 *base.Wei) base.Wei {
			return *a1.Add(a1, a2)
		}

		// Do not collapse, more than one of these can be true at the same time
		if trace.Action.From == stmt.AccountedFor {
			stmt.InternalOut = plusEq(&stmt.InternalOut, &trace.Action.Value)
			stmt.Sender = trace.Action.From
			if trace.Action.To.IsZero() {
				if trace.Result != nil {
					stmt.Recipient = trace.Result.Address
				}
			} else {
				stmt.Recipient = trace.Action.To
			}
		}

		if trace.Action.To == stmt.AccountedFor {
			stmt.InternalIn = plusEq(&stmt.InternalIn, &trace.Action.Value)
			stmt.Sender = trace.Action.From
			stmt.Recipient = trace.Action.To
		}

		if trace.Action.SelfDestructed == stmt.AccountedFor {
			stmt.SelfDestructOut = plusEq(&stmt.SelfDestructOut, &trace.Action.Balance)
			stmt.Sender = trace.Action.SelfDestructed
			if stmt.Sender.IsZero() {
				stmt.Sender = trace.Action.Address
			}
			stmt.Recipient = trace.Action.RefundAddress
		}

		if trace.Action.RefundAddress == stmt.AccountedFor {
			stmt.SelfDestructIn = plusEq(&stmt.SelfDestructIn, &trace.Action.Balance)
			stmt.Sender = trace.Action.SelfDestructed
			if stmt.Sender.IsZero() {
				stmt.Sender = trace.Action.Address
			}
			stmt.Recipient = trace.Action.RefundAddress
		}

		if trace.Action.Address == stmt.AccountedFor && !trace.Action.RefundAddress.IsZero() {
			stmt.SelfDestructOut = plusEq(&stmt.SelfDestructOut, &trace.Action.Balance)
			// self destructed send
			stmt.Sender = trace.Action.Address
			stmt.Recipient = trace.Action.RefundAddress
		}

		if trace.Result != nil {
			if trace.Result.Address == stmt.AccountedFor {
				stmt.InternalIn = plusEq(&stmt.InternalIn, &trace.Action.Value)
				stmt.Sender = trace.Action.From
				stmt.Recipient = trace.Result.Address
			}
		}
	}
	return stmt, nil
}
