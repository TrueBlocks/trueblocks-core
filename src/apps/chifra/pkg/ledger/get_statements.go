package ledger

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (r *Reconciler) GetStatements(node *types.AppNode[types.Transaction]) ([]types.Statement, error) {
	trans := node.Data()
	fail := func(n int) {
		logger.TestLog(true, fmt.Sprintf("~fail %d~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~", n))
	}

	debugHeader(node)

	results := make([]types.Statement, 0, 20)
	if types.IsAssetOfInterest(base.FAKE_ETH_ADDRESS, r.Opts.AssetFilters) {
		logger.TestLog(true, "ETH is of interest")
		if r.Opts.UseTraces {
			logger.TestLog(true, "Skipping top-level - going right to traces")
			if traces, err := r.Connection.GetTracesByTransactionHash(trans.Hash.Hex(), trans); err != nil {
				fail(1)
				return nil, err
			} else {
				if xfr, err := trans.ConvertTracesToTransfer(traces, r.Opts.AccountFor); err != nil {
					fail(2)
					logger.Error(err.Error())
				} else {
					stmt := r.ConvertToStatement(xfr, trans)
					logger.TestLog(true, "Fetched a single statement from traces")
					if _, err = r.trialBalance("traces", stmt, node, true); err != nil {
						fail(3)
						return nil, err
					} else {
						if stmt.IsMaterial() { // append even if not reconciled
							ReportProgress(stmt, true)
							logger.TestLog(true, "Statement is material - appending. reconciled:", stmt.Reconciled())
							results = append(results, *stmt)
						} else {
							logger.TestLog(true, "Statement was not material")
						}
					}
				}
			}
		} else {
			logger.TestLog(true, fmt.Sprintf("Attempting to reconcile at top level: %s-%s", base.FAKE_ETH_ADDRESS.Hex(), r.Opts.AccountFor.Hex()))
			if xfr, err := trans.ConvertToTransfer(r.Opts.AccountFor); err != nil {
				fail(4)
				return nil, err
			} else {
				stmt := r.ConvertToStatement(xfr, trans)
				if reconciled, err := r.trialBalance("top-level", stmt, node, false); err != nil {
					fail(5)
					return nil, err
				} else {
					if reconciled {
						if stmt.IsMaterial() {
							ReportProgress(stmt, false)
							logger.TestLog(true, "Statement is material and reconciled - appending")
							results = append(results, *stmt)
						} else {
							logger.TestLog(true, "Statement was not material")
						}
					} else {
						logger.TestLog(true, "Failed reconcile, decending into to traces")
						if traces, err := r.Connection.GetTracesByTransactionHash(trans.Hash.Hex(), trans); err != nil {
							fail(5)
							return nil, err
						} else {
							if xfr, err := trans.ConvertTracesToTransfer(traces, r.Opts.AccountFor); err != nil {
								fail(6)
								logger.Error(err.Error())
							} else {
								stmt := r.ConvertToStatement(xfr, trans)
								logger.TestLog(true, "Fetched a single statement from traces")
								if _, err = r.trialBalance("traces", stmt, node, true); err != nil {
									fail(7)
									return nil, err
								} else {
									if stmt.IsMaterial() { // append even if not reconciled
										ReportProgress(stmt, true)
										logger.TestLog(true, "Statement is material - appending. reconciled:", stmt.Reconciled())
										results = append(results, *stmt)
									} else {
										logger.TestLog(true, "Statement was not material")
									}
								}
							}
						}
					}
				}
			}
		}
	} else {
		logger.TestLog(true, "ETH is NOT of interest")
	}

	if trans.Receipt == nil {
		logger.TestLog(true, "Transaction receipt is nil. No log statements.")
	} else {
		logger.TestLog(true, "Extracting statements from logs.")
		if xfrs, err := trans.Receipt.ConvertToTranfers(r.Opts.AccountFor, r.Opts.AssetFilters, r.Opts.AppFilters); err != nil {
			fail(8)
			return nil, err

		} else {
			if len(xfrs) == 0 {
				logger.TestLog(true, "There were no statements generated from logs.")

			} else {
				logger.TestLog(true, len(xfrs), "statements generated from logs.")
				for i, xfr := range xfrs {
					stmt := r.ConvertToStatement(xfr, trans)
					if i == 0 {
						stmt.BegSentinel = true
					}
					if i == len(xfrs)-1 {
						stmt.EndSentinel = true
					}
					stmt.Symbol = stmt.Asset.DefaultSymbol()
					stmt.Decimals = base.Value(18)
					if name, found := r.Names[stmt.Asset]; found {
						if r.RemoveAirdrops && r.SkipAirdrop(stmt.Asset) {
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

					if _, err := r.trialBalance("token", stmt, node, true); err != nil {
						fail(9)
						logger.Error(err.Error())
						continue
					} else {
						// order matters - don't move
						if stmt.IsMaterial() { // append even if not reconciled
							ReportProgress(stmt, true)
							logger.TestLog(true, "Statement is material - appending. reconciled:", stmt.Reconciled())
							results = append(results, *stmt)
						} else {
							logger.TestLog(true, "Statement was not material")
						}
					}
				}
			}
		}
	}

	logger.TestLog(true, fmt.Sprintf("~~~ Leaving: %d.%d ~~~", node.CurBlock(), node.CurTxId()))
	logger.TestLog(true, "------------------------------------", len(results), "statements generated.")
	logger.TestLog(true, "")
	return results, nil
}
