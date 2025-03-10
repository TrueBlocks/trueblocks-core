package types

import "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"

func (trans *Transaction) StatementFromTransaction(asEther bool, asset, holder base.Address) *Statement {
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
