package types

import (
	"slices"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/topics"
)

// ------------------------------------------------------------------------------------------
func (trans *Transaction) ToTransfer(holder base.Address) (*Transfer, error) {
	to := trans.To
	if trans.To.IsZero() && trans.Receipt != nil && !trans.Receipt.ContractAddress.IsZero() {
		to = trans.Receipt.ContractAddress
	}

	xfr := &Transfer{
		Transaction:      trans,
		Log:              nil,
		BlockNumber:      trans.BlockNumber,
		TransactionIndex: trans.TransactionIndex,
		LogIndex:         0,
		Recipient:        to,
		Sender:           trans.From,
		Holder:           holder,
		Asset:            base.FAKE_ETH_ADDRESS,
		Decimals:         18,
	}

	isSender, isRecipient := xfr.Sender == holder, xfr.Recipient == holder
	if isSender {
		gasUsed := new(base.Wei)
		if trans.Receipt != nil {
			gasUsed.SetUint64(uint64(trans.Receipt.GasUsed))
		}
		gasPrice := new(base.Wei).SetUint64(uint64(trans.GasPrice))
		gasOut := new(base.Wei).Mul(gasUsed, gasPrice)
		xfr.AmountOut = trans.Value
		xfr.GasOut = *gasOut
	}

	if isRecipient {
		if xfr.BlockNumber == 0 {
			xfr.PrefundIn = trans.Value
		} else {
			if trans.Rewards != nil {
				xfr.MinerBaseRewardIn = trans.Rewards.Block
				xfr.MinerNephewRewardIn = trans.Rewards.Nephew
				xfr.MinerTxFeeIn = trans.Rewards.TxFee
				xfr.MinerUncleRewardIn = trans.Rewards.Uncle
			} else {
				xfr.AmountIn = trans.Value
			}
		}
	}

	return xfr, nil
}

// ------------------------------------------------------------------------------------------
func (trans *Transaction) TracesToTransfer(traces []Trace, holder base.Address) (*Transfer, error) {
	if xfr, err := trans.ToTransfer(holder); err != nil {
		return nil, err

	} else {
		wasModified := false
		for i, trace := range traces {
			// skip over the first trace, we've already gotten its values from the transaction
			if i > 0 {
				if modified, err := trace.updateTransfer(xfr); err != nil {
					return nil, err
				} else {
					wasModified = wasModified || modified
					// logger.TestLog(modified, fmt.Sprintf("tx: %d.%d trace: %d of %d modifed transfer", trans.BlockNumber, trans.TransactionIndex, i, len(traces)))
				}
			}
		}
		return xfr, nil
	}
}

// ------------------------------------------------------------------------------------------
func (t *Trace) updateTransfer(xfr *Transfer) (bool, error) {
	modified := false

	// delegate calls are not included in the transaction's gas cost, so we skip them
	if t.Action.CallType == "delegatecall" && t.Action.To != xfr.Holder {
		return modified, nil
	}

	plusEq := func(a1, a2 *base.Wei) base.Wei {
		return *a1.Add(a1, a2)
	}

	// Do not collapse, more than one of these can be true at the same time
	if t.Action.From == xfr.Holder {
		xfr.InternalOut = plusEq(&xfr.InternalOut, &t.Action.Value)
		xfr.Sender = t.Action.From
		if t.Action.To.IsZero() {
			if t.Result != nil {
				xfr.Recipient = t.Result.Address
			}
		} else {
			xfr.Recipient = t.Action.To
		}
		modified = true
	}

	if t.Action.To == xfr.Holder {
		xfr.InternalIn = plusEq(&xfr.InternalIn, &t.Action.Value)
		xfr.Sender = t.Action.From
		xfr.Recipient = t.Action.To
		modified = true
	}

	if t.Action.SelfDestructed == xfr.Holder {
		xfr.SelfDestructOut = plusEq(&xfr.SelfDestructOut, &t.Action.Balance)
		xfr.Sender = t.Action.SelfDestructed
		if xfr.Sender.IsZero() {
			xfr.Sender = t.Action.Address
		}
		xfr.Recipient = t.Action.RefundAddress
		modified = true
	}

	if t.Action.RefundAddress == xfr.Holder {
		xfr.SelfDestructIn = plusEq(&xfr.SelfDestructIn, &t.Action.Balance)
		xfr.Sender = t.Action.SelfDestructed
		if xfr.Sender.IsZero() {
			xfr.Sender = t.Action.Address
		}
		xfr.Recipient = t.Action.RefundAddress
		modified = true
	}

	if t.Action.Address == xfr.Holder && !t.Action.RefundAddress.IsZero() {
		xfr.SelfDestructOut = plusEq(&xfr.SelfDestructOut, &t.Action.Balance)
		xfr.Sender = t.Action.Address
		xfr.Recipient = t.Action.RefundAddress
		modified = true
	}

	if t.Result != nil {
		if t.Result.Address == xfr.Holder {
			xfr.InternalIn = plusEq(&xfr.InternalIn, &t.Action.Value)
			xfr.Sender = t.Action.From
			xfr.Recipient = t.Result.Address
			modified = true
		}
	}

	return modified, nil
}

// ------------------------------------------------------------------------------------------
func (s *Receipt) ToTranfers(holder base.Address, assetFilters []base.Address, appFilter *AppearanceFilter) ([]*Transfer, error) {
	xfrs := make([]*Transfer, 0, 20)

	for _, log := range s.Logs {
		if len(log.Topics) > 0 {
			isTransfer := log.Topics[0] == topics.TransferTopic
			isOfIterest := IsAssetOfInterest(log.Address, assetFilters)
			passesFilter := appFilter.PassesLogFilter(&log, []base.Address{holder})
			if isTransfer && isOfIterest && passesFilter {
				if xfr, err := log.toTransfer(holder); err != nil {
					return nil, err

				} else if xfr == nil {
					continue

				} else {
					xfrs = append(xfrs, xfr)
				}
			}
		}
	}

	return xfrs, nil
}

// ------------------------------------------------------------------------------------------
func (log *Log) toTransfer(holder base.Address) (*Transfer, error) {
	if normalized, err := NormalizeKnownLogs(log); err != nil {
		return nil, err

	} else if normalized.IsNFT() {
		return nil, nil

	} else {
		sender := base.HexToAddress(normalized.Topics[1].Hex())
		recipient := base.HexToAddress(normalized.Topics[2].Hex())
		isSender, isRecipient := holder == sender, holder == recipient
		if !isSender && !isRecipient {
			return nil, nil
		}

		var amountIn, amountOut base.Wei
		amount, _ := new(base.Wei).SetString(strings.ReplaceAll(normalized.Data, "0x", ""), 16)
		if amount == nil {
			amount = base.ZeroWei
		}

		// can be both
		if isSender {
			amountOut = *amount
		}

		// can be both
		if isRecipient {
			amountIn = *amount
		}

		xfr := &Transfer{
			Log:              normalized,
			BlockNumber:      normalized.BlockNumber,
			TransactionIndex: normalized.TransactionIndex,
			LogIndex:         normalized.LogIndex,
			Sender:           sender,
			Recipient:        recipient,
			Asset:            normalized.Address,
			Holder:           holder,
			AmountIn:         amountIn,
			AmountOut:        amountOut,
			Decimals:         18,
		}
		return xfr, nil
	}
}

// ------------------------------------------------------------------------------------------
func IsAssetOfInterest(needle base.Address, filters []base.Address) bool {
	if len(filters) == 0 {
		return true
	}
	return slices.ContainsFunc(filters, func(asset base.Address) bool {
		return asset.Hex() == needle.Hex()
	})
}
