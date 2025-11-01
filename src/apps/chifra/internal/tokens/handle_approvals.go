// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package tokensPkg

import (
	"context"
	"fmt"
	"sort"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/monitor"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/ranges"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/rpc/query"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/topics"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/utils"
)

const maxTestingBlock = 14000000

// approvalKey represents a unique approval event
type approvalKey struct {
	Owner   base.Address
	Spender base.Address
	Token   base.Address
}

// approvalInfo represents information about an approval event
type approvalInfo struct {
	Allowance *base.Wei
	LastTs    base.Timestamp
	LastBlock base.Blknum
	LastTxId  base.Txnum
	LastLogId base.Lognum
}

// approvalMap maps approval keys to their info
type approvalMap map[approvalKey]approvalInfo

func (opts *TokensOptions) HandleApprovals(rCtx *output.RenderCtx) error {
	chain := opts.Globals.Chain
	var firstBlock base.Blknum = 0
	var lastBlock = opts.Conn.GetLatestBlockNumber()

	if len(opts.BlockIds) > 0 && opts.BlockIds[0].End.Number != 0 {
		firstBlockNums, err := opts.BlockIds[0].ResolveBlocks(chain)
		if err != nil {
			return err
		}
		lastBlockNums, err := opts.BlockIds[len(opts.BlockIds)-1].ResolveBlocks(chain)
		if err != nil {
			return err
		}
		firstBlock = firstBlockNums[0]
		lastBlock = lastBlockNums[len(lastBlockNums)-1]
	}

	if base.IsTestMode() {
		lastBlock = min(lastBlock, maxTestingBlock)
		firstBlock = min(firstBlock, lastBlock-1)
	}

	block, _ := opts.Conn.GetBlockHeaderByNumber(lastBlock)
	lastTs := block.Timestamp

	approvals := make(approvalMap)

	fetchData := func(modelChan chan types.Modeler, errorChan chan error) {
		chain := opts.Globals.Chain
		filter := types.NewFilter(
			false,
			false,
			[]string{},
			ranges.BlockRange{First: firstBlock, Last: lastBlock},
			ranges.RecordRange{First: 0, Last: base.NOPOS},
		)

		monitorArray := make([]monitor.Monitor, 0, len(opts.Addrs))
		for _, addr := range opts.Addrs {
			if mon, err := monitor.NewMonitor(chain, base.HexToAddress(addr), false); err != nil {
				errorChan <- err
				rCtx.Cancel()
				return
			} else {
				_ = mon.ReadMonitorHeader() // so we know how many appearances there are
				defer mon.Close()
				monitorArray = append(monitorArray, mon)
			}
		}

		addrArray := make([]base.Address, 0, len(monitorArray))
		for _, mon := range monitorArray {
			addrArray = append(addrArray, mon.Address)
		}
		logFilter := rpc.NewLogFilter([]string{}, []string{topics.ApprovalTopic.Hex()})

		reportedCnt := 0
		for _, mon := range monitorArray {
			if sliceOfMaps, cnt, err := monitor.AsSliceOfItemMaps[[]*types.Log](&mon, filter, filter.Reversed); err != nil {
				errorChan <- err
				rCtx.Cancel()

			} else if cnt == 0 {
				errorChan <- fmt.Errorf("no blocks found for the query")
				continue

			} else {
				showProgress := opts.Globals.ShowProgress()
				bar := logger.NewBar(logger.BarOptions{
					Prefix:  mon.Address.Hex(),
					Enabled: showProgress,
					Total:   int64(cnt),
				})

				finished := false
				for _, thisMap := range sliceOfMaps {
					if rCtx.WasCanceled() {
						return
					}

					if finished {
						continue
					}

					for app := range thisMap {
						thisMap[app] = &[]*types.Log{}
					}

					iterFunc := func(app types.Appearance, value *[]*types.Log) error {
						if tx, err := opts.Conn.GetTransactionByAppearance(&app, false); err != nil {
							return err
						} else {
							passes := filter.PassesTxFilter(tx)
							if passes && tx.Receipt != nil {
								for _, log := range tx.Receipt.Logs {
									if filter.PassesLogFilter(&log, addrArray) && logFilter.PassesFilter(&log) {
										*value = append(*value, &log)
									}
								}
							}
							return nil
						}
					}

					// Set up and iterate over the map calling iterFunc for each appearance (PARALLEL)
					iterCtx, iterCancel := context.WithCancel(context.Background())
					defer iterCancel()
					errChan := make(chan error)
					go utils.IterateOverMap(iterCtx, errChan, thisMap, iterFunc)
					if stepErr := <-errChan; stepErr != nil {
						errorChan <- stepErr
						return
					}

					// Now safely collect all logs from all log slices
					items := make([]*types.Log, 0)
					for _, logSlice := range thisMap {
						if logSlice != nil && *logSlice != nil {
							items = append(items, *logSlice...)
						}
					}

					sort.Slice(items, func(i, j int) bool {
						// if false { // opts.Reversed {
						// 	i, j = j, i
						// }
						if items[i].BlockNumber == items[j].BlockNumber {
							if items[i].TransactionIndex == items[j].TransactionIndex {
								return items[i].LogIndex < items[j].LogIndex
							}
							return items[i].TransactionIndex < items[j].TransactionIndex
						}
						return items[i].BlockNumber < items[j].BlockNumber
					})

					for _, item := range items {
						var passes bool
						passes, finished = filter.PassesCountFilter()
						if passes {
							ownerAddr := base.HexToAddress(item.Topics[1].Hex())
							spenderAddr := base.HexToAddress(item.Topics[2].Hex())
							tokenAddr := item.Address
							currentAllowance, err := opts.getAllowanceAtBlock(tokenAddr, ownerAddr, spenderAddr, lastBlock)
							if err != nil {
								errChan <- err
								continue
							} else if currentAllowance == nil {
								continue
							} else if opts.NoZero && currentAllowance.IsZero() {
								continue
							}
							key := approvalKey{
								Owner:   ownerAddr,
								Spender: spenderAddr,
								Token:   tokenAddr,
							}

							// Check if the key exists and if the new item is later
							existingInfo, ok := approvals[key]
							isLater := false
							if !ok {
								isLater = true
							} else {
								if item.BlockNumber > existingInfo.LastBlock {
									isLater = true
								} else if item.BlockNumber == existingInfo.LastBlock {
									if item.TransactionIndex > existingInfo.LastTxId {
										isLater = true
									} else if item.TransactionIndex == existingInfo.LastTxId {
										if item.LogIndex > existingInfo.LastLogId {
											isLater = true
										}
									}
								}
							}

							if isLater {
								approvals[key] = approvalInfo{
									Allowance: currentAllowance,
									LastTs:    item.Timestamp,
									LastBlock: item.BlockNumber,
									LastTxId:  item.TransactionIndex,
									LastLogId: item.LogIndex,
								}
							}
							if bar != nil && showProgress {
								bar.Tick()
							} else {
								reportedCnt++
								logger.Progress(reportedCnt%5 == 0, "Found:", ownerAddr.Hex(), "-->", spenderAddr.Hex())
							}
						}
						if finished {
							break
						}
					}
				}
				bar.Finish(true /* newLine */)
			}

			apps := make([]approvalKey, 0, len(approvals))
			for key := range approvals {
				apps = append(apps, key)
			}

			sort.Slice(apps, func(i, j int) bool {
				if apps[i].Token == apps[j].Token {
					if apps[i].Spender == apps[j].Spender {
						if apps[i].Owner == apps[j].Owner {
							return approvals[apps[i]].LastBlock < approvals[apps[j]].LastBlock
						}
						return apps[i].Owner.LessThan(apps[j].Owner)
					}
					return apps[i].Spender.LessThan(apps[j].Spender)
				}
				return apps[i].Token.LessThan(apps[j].Token)
			})

			for _, key := range apps {
				modelChan <- &types.Approval{
					Owner:        key.Owner,
					Spender:      key.Spender,
					Token:        key.Token,
					Allowance:    *approvals[key].Allowance,
					BlockNumber:  lastBlock,
					Timestamp:    lastTs,
					LastAppBlock: approvals[key].LastBlock,
					LastAppTs:    approvals[key].LastTs,
					LastAppLogID: approvals[key].LastLogId,
					LastAppTxID:  approvals[key].LastTxId,
				}
			}
		}
	}

	extraOpts := map[string]any{
		"loadNames": true,
	}

	return output.StreamMany(rCtx, fetchData, opts.Globals.OutputOptsWithExtra(extraOpts))
}

func (opts *TokensOptions) getAllowanceAtBlock(tokenAddr, owner, spender base.Address, blockNum base.Blknum) (*base.Wei, error) {
	funcSig := "0xdd62ed3e"
	data := funcSig + owner.Pad32() + spender.Pad32()

	var blockParam string
	if blockNum == 0 {
		blockParam = "latest"
	} else {
		blockParam = fmt.Sprintf("0x%x", blockNum)
	}

	payloads := []query.BatchPayload{{
		Key: "allowance",
		Payload: &query.Payload{
			Method: "eth_call",
			Params: query.Params{
				map[string]any{
					"to":   tokenAddr.Hex(),
					"data": data,
				},
				blockParam,
			},
		},
	}}
	output, err := query.QueryBatch[string](opts.Conn.Chain, payloads)
	if err != nil {
		return base.NewWei(0), err
	}
	var allowance *base.Wei
	if output["allowance"] == nil {
		allowance = base.NewWei(0)
	} else {
		allowance = base.HexToWei(*output["allowance"])
	}
	return allowance, nil
}
