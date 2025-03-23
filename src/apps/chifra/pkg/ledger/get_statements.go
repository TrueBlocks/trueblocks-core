package ledger

import (
	"fmt"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/walk"
)

func (r *Reconciler) GetStatements(node *types.AppNode[types.Transaction]) ([]types.Statement, error) {
	trans := node.Data()
	statementGroup := &types.StatementGroup{
		Address:          r.Opts.AccountFor,
		BlockNumber:      trans.BlockNumber,
		TransactionIndex: trans.TransactionIndex,
	}
	if err := r.Connection.ReadFromCache(statementGroup); err == nil {
		return statementGroup.Statements, nil
	}

	logger.TestLog(true, "")
	logger.TestLog(true, "------------------------------------")
	logger.TestLog(true, fmt.Sprintf("~~~ Entering: %d.%d ~~~", trans.BlockNumber, trans.TransactionIndex))

	debugFail := func(n int) {
		logger.TestLog(true, fmt.Sprintf("~fail %d~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~", n))
	}

	// First Pass: Fetch unreconciled transfers
	logger.TestLog(true, "First pass: Fetching unreconciled transfers")
	ethTransfers, tokenTransfers, balances, err := r.getTransfersInternal([]*types.Transaction{trans}, true)
	if err != nil {
		debugFail(1)
		logger.TestLog(true, "Error getting unreconciled transfers:", err)
		return nil, err
	}
	logger.TestLog(true, "Fetched", len(ethTransfers)+len(tokenTransfers), "unreconciled transfers")

	results := make([]types.Statement, 0, len(ethTransfers)+len(tokenTransfers))

	// Second Pass: Reconcile transfers
	logger.TestLog(true, "Second pass: Reconciling transfers")

	// Handle ETH transfer if present
	if len(ethTransfers) > 0 {
		ethXfr := ethTransfers[0]
		if ethXfr != nil {
			logger.TestLog(true, "Processing ETH transfer")
			stmt := ethXfr.ToStatement(trans, r.Opts.AsEther)
			// stmt.Symbol = "ETH" // Assuming ETH has a fixed symbol
			stmt.Decimals = 18
			logger.TestLog(true, "Converted ETH transfer to statement")

			if r.Opts.UseTraces {
				logger.TestLog(true, "Using trace-based ETH transfer")
				if _, err := r.trialBalance("traces", stmt, node, true, balances); err != nil {
					debugFail(2)
					logger.TestLog(true, "Error in trialBalance for traces:", err)
					return nil, err
				} else {
					if stmt.IsMaterial() { // Append even if not reconciled
						ReportProgress(stmt, true)
						logger.TestLog(true, "Trace-based statement is material - appending. reconciled:", stmt.Reconciled())
						results = append(results, *stmt)
					} else {
						logger.TestLog(true, "Trace-based statement was not material")
					}
				}
			} else {
				logger.TestLog(true, fmt.Sprintf("Attempting to reconcile top-level ETH transfer: %s-%s", base.FAKE_ETH_ADDRESS.Hex(), r.Opts.AccountFor.Hex()))
				reconciled, err := r.trialBalance("top-level", stmt, node, false, balances)
				if err != nil {
					debugFail(3)
					logger.TestLog(true, "Error in trialBalance for top-level:", err)
					return nil, err
				} else if reconciled {
					if stmt.IsMaterial() {
						ReportProgress(stmt, false)
						logger.TestLog(true, "Top-level statement is material and reconciled - appending")
						results = append(results, *stmt)
					} else {
						logger.TestLog(true, "Top-level statement was not material")
					}
				} else {
					logger.TestLog(true, "Top-level ETH transfer debugFailed to reconcile, falling back to traces")
					if traces, err := r.Connection.GetTracesByTransactionHash(trans.Hash.Hex(), trans); err != nil {
						debugFail(4)
						logger.TestLog(true, "Error getting traces:", err)
						return nil, err
					} else {
						if traceXfr, err := trans.TracesToTransfer(traces, r.Opts.AccountFor); err != nil {
							debugFail(5)
							logger.TestLog(true, "Error converting traces to transfer:", err)
							return nil, err
						} else {
							traceStmt := traceXfr.ToStatement(trans, r.Opts.AsEther)
							// traceStmt.Symbol = "ETH"
							traceStmt.Decimals = 18
							logger.TestLog(true, "Fetched and converted trace-based ETH transfer to statement")
							if _, err = r.trialBalance("traces", traceStmt, node, true, balances); err != nil {
								debugFail(6)
								logger.TestLog(true, "Error in trialBalance for traces:", err)
								return nil, err
							} else {
								if traceStmt.IsMaterial() { // Append even if not reconciled
									ReportProgress(traceStmt, true)
									logger.TestLog(true, "Trace-based statement is material - appending. reconciled:", traceStmt.Reconciled())
									results = append(results, *traceStmt)
								} else {
									logger.TestLog(true, "Trace-based statement was not material")
								}
							}
						}
					}
				}
			}
		} else {
			logger.TestLog(true, "No ETH transfer found")
		}
	}

	// Handle log transfers
	if len(tokenTransfers) == 0 {
		logger.TestLog(true, "No log transfers to process")
	} else {
		logger.TestLog(true, "Processing", len(tokenTransfers), "log transfers")
		for i, xfr := range tokenTransfers {
			stmt := xfr.ToStatement(trans, r.Opts.AsEther)
			if i == 0 {
				stmt.BegSentinel = true
				logger.TestLog(true, "Set BegSentinel for first log transfer")
			}
			if i == len(tokenTransfers)-1 {
				stmt.EndSentinel = true
				logger.TestLog(true, "Set EndSentinel for last log transfer")
			}
			stmt.Symbol = stmt.Asset.DefaultSymbol()
			stmt.Decimals = base.Value(18)
			if name, found := r.Names[stmt.Asset]; found {
				if r.RemoveAirdrops && name.IsAirdrop() {
					logger.TestLog(true, "Removing airdrop for", stmt.Asset, "at block", stmt.BlockNumber, ".", stmt.TransactionIndex)
					continue
				}
				if name.Symbol != "" {
					stmt.Symbol = name.Symbol
				}
				if name.Decimals != 0 {
					stmt.Decimals = base.Value(name.Decimals)
				}
			}
			logger.TestLog(true, "Converted log transfer to statement for asset:", stmt.Asset.Hex())

			if _, err := r.trialBalance("token", stmt, node, true, balances); err != nil {
				debugFail(7)
				logger.TestLog(true, "Error in trialBalance for token:", err)
				continue // Continue processing other transfers as in original
			} else {
				if stmt.IsMaterial() { // Append even if not reconciled
					ReportProgress(stmt, true)
					logger.TestLog(true, "Log statement is material - appending. reconciled:", stmt.Reconciled())
					results = append(results, *stmt)
				} else {
					logger.TestLog(true, "Log statement was not material")
				}
			}
		}
	}

	logger.TestLog(true, fmt.Sprintf("~~~ Leaving: %d.%d ~~~", node.CurBlock(), node.CurTxId()))
	logger.TestLog(true, "------------------------------------", len(results), "statements generated.")
	logger.TestLog(true, "")

	// we don't want to cache filtered results
	if !r.HasFilters() {
		statementGroup.Statements = results
		err = r.Connection.WriteToCache(statementGroup, walk.Cache_Statements, trans.Timestamp)
		return results, err
	}
	return results, nil
}

func ReportProgress(stmt *types.Statement, warn bool) {
	msg := fmt.Sprintf("Ether statement at % 9d.%d.%d %s %s", stmt.BlockNumber, stmt.TransactionIndex, stmt.LogIndex, stmt.Asset.Hex(), stmt.Holder.Hex())
	if !stmt.IsEth() {
		msg = fmt.Sprintf("Token statement at % 9d.%d.%d %s %s", stmt.BlockNumber, stmt.TransactionIndex, stmt.LogIndex, stmt.Asset.Hex(), stmt.Holder.Hex())
	}
	spacer := strings.Repeat(" ", 100-base.Min(100, len(msg)))
	if !stmt.Reconciled() {
		// logger.Progress(true, colors.Green+msg+" reconciled.", colors.Off, spacer)
		// } else {
		if warn {
			logger.Warn(msg+" did not reconcile.", spacer)
		}
	}
}
