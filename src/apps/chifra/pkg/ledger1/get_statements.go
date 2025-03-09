package ledger1

import (
	"fmt"
	"os"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/ledger10"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/normalize"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/topics"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (r *Reconciler1) GetStatements1(pos *types.AppPosition, trans *types.Transaction) ([]types.Statement, error) {
	results := make([]types.Statement, 0, 20)
	if ledger10.AssetOfInterest(r.opts.AssetFilters, base.FAKE_ETH_ADDRESS) {
		var err error
		reconciled := false
		if !r.opts.UseTraces {
			stmt := r.getStatementFromTransaction(trans)
			if reconciled, err = r.trialBalance(pos, trans, stmt); err != nil {
				return nil, err
			} else {
				if reconciled && stmt.IsMaterial() {
					results = append(results, *stmt)
				}
			}
		}

		if r.opts.UseTraces || !reconciled {
			if stmt, err := r.getStatementFromTraces(trans); err != nil {
				logger.Warn(colors.Yellow+"Statement at ", fmt.Sprintf("%d.%d", trans.BlockNumber, trans.TransactionIndex), " does not reconcile."+colors.Off)
			} else {
				if _, err = r.trialBalance(pos, trans, stmt); err != nil {
					return nil, err
				} else {
					if stmt.IsMaterial() {
						results = append(results, *stmt)
					}
				}
			}
		}
	}

	if trans.Receipt != nil {
		if statements, err := r.getStatementsFromLogs(trans.Receipt.Logs); err != nil {
			return nil, err
		} else {
			receiptStatements := make([]types.Statement, 0, len(statements))
			for _, stmt := range statements {
				if reconciled, err := r.trialBalance(pos, trans, &stmt); err != nil {
					// TODO: Silent fail?
					continue
				} else {
					if reconciled {
						id := fmt.Sprintf(" %d.%d.%d", stmt.BlockNumber, stmt.TransactionIndex, stmt.LogIndex)
						logger.Progress(true, colors.Green+"Transaction", id, "reconciled       "+colors.Off)
					} else {
						if os.Getenv("TEST_MODE") != "true" {
							id := fmt.Sprintf(" %d.%d.%d", stmt.BlockNumber, stmt.TransactionIndex, stmt.LogIndex)
							logger.Warn("Log statement at ", id, " does not reconcile.")
						}
					}
					// order matters
					if stmt.IsMaterial() {
						receiptStatements = append(receiptStatements, stmt)
					}
				}
			}
			results = append(results, receiptStatements...)
		}
	}
	return results, nil
}

func (r *Reconciler1) NewStatement(trans *types.Transaction) *types.Statement {
	sym := "WEI"
	if r.opts.AsEther {
		sym = "ETH"
	}
	to := trans.To
	if trans.To.IsZero() && trans.Receipt != nil && !trans.Receipt.ContractAddress.IsZero() {
		to = trans.Receipt.ContractAddress
	}

	return &types.Statement{
		AccountedFor:     r.opts.AccountFor,
		Sender:           trans.From,
		Recipient:        to,
		BlockNumber:      trans.BlockNumber,
		TransactionIndex: trans.TransactionIndex,
		TransactionHash:  trans.Hash,
		Timestamp:        trans.Timestamp,
		Asset:            base.FAKE_ETH_ADDRESS,
		Symbol:           sym,
		Decimals:         18,
		PriceSource:      "not-priced",
	}
}

func (r *Reconciler1) getStatementFromTransaction(trans *types.Transaction) *types.Statement {
	stmt := r.NewStatement(trans)
	if stmt.Sender == r.opts.AccountFor {
		gasUsed := new(base.Wei)
		if trans.Receipt != nil {
			gasUsed.SetUint64(uint64(trans.Receipt.GasUsed))
		}
		gasPrice := new(base.Wei).SetUint64(uint64(trans.GasPrice))
		gasOut := new(base.Wei).Mul(gasUsed, gasPrice)
		stmt.AmountOut = trans.Value
		stmt.GasOut = *gasOut
	}
	if stmt.Recipient == r.opts.AccountFor {
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

func (r *Reconciler1) getStatementFromTraces(trans *types.Transaction) (*types.Statement, error) {
	if traces, err := r.opts.Connection.GetTracesByTransactionHash(trans.Hash.Hex(), trans); err != nil {
		return nil, err

	} else {
		stmt := r.getStatementFromTransaction(trans)
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
}

func (r *Reconciler1) getStatementsFromLogs(logs []types.Log) ([]types.Statement, error) {
	statements := make([]types.Statement, 0, 20)
	for _, log := range logs {
		pass1 := log.Topics[0] == topics.TransferTopic
		if !pass1 {
			continue
		}

		pass2 := r.opts.AppFilters.ApplyLogFilter(&log, []base.Address{r.opts.AccountFor})
		if !pass2 {
			continue
		}

		pass3 := ledger10.AssetOfInterest(r.opts.AssetFilters, log.Address)
		if !pass3 {
			continue
		}

		normalized, err := normalize.NormalizeKnownLogs(&log)
		if err != nil {
			// TODO: silent fail?
			continue

		} else if normalized.IsNFT() {
			continue

		} else {
			sender := base.HexToAddress(normalized.Topics[1].Hex())
			recipient := base.HexToAddress(normalized.Topics[2].Hex())
			isSender, isRecipient := r.opts.AccountFor == sender, r.opts.AccountFor == recipient
			if !isSender && !isRecipient {
				continue
			}

			var amountIn, amountOut base.Wei
			amount, _ := new(base.Wei).SetString(strings.ReplaceAll(normalized.Data, "0x", ""), 16)
			if amount == nil {
				amount = base.ZeroWei
			}
			if r.opts.AccountFor == sender {
				amountOut = *amount
			}
			if r.opts.AccountFor == recipient {
				amountIn = *amount
			}
			stmt := &types.Statement{
				AccountedFor:     r.opts.AccountFor,
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
			}

			name := r.names[stmt.Asset]
			stmt.Symbol = stmt.Asset.DefaultSymbol()
			stmt.Decimals = base.Value(18)
			if name.Address == stmt.Asset {
				if name.Symbol != "" {
					stmt.Symbol = name.Symbol
				}
				if name.Decimals != 0 {
					stmt.Decimals = base.Value(name.Decimals)
				}
			}
			statements = append(statements, *stmt)
		}
	}
	return statements, nil
}
