package uniq

import (
	"fmt"
	"sync"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// AddMiner adds the miner address (for use with post-merge)
func AddMiner(chain string, miner base.Address, bn base.Blknum, addrMap AddressBooleanMap) (err error) {
	addAddressToMaps(miner, bn, types.BlockReward, addrMap)
	return nil
}

// UniqFromWithdrawals extracts addresses from an array of receipts
func UniqFromWithdrawals(chain string, withdrawals []types.Withdrawal, bn base.Blknum, addrMap AddressBooleanMap) (err error) {
	for _, withdrawal := range withdrawals {
		addAddressToMaps(withdrawal.Address, bn, types.WithdrawalAmt, addrMap)
	}
	return nil
}

// UniqFromReceipts extracts addresses from an array of receipts
func UniqFromReceipts(chain string, receipts []types.Receipt, addrMap AddressBooleanMap) (err error) {
	for _, receipt := range receipts {
		addAddressToMaps(receipt.ContractAddress, receipt.BlockNumber, receipt.TransactionIndex, addrMap)
		if err := uniqFromLogs(chain, receipt.Logs, addrMap); err != nil {
			return err
		}
	}
	return nil
}

// uniqFromLogs extracts addresses from the logs
func uniqFromLogs(chain string, logs []types.Log, addrMap AddressBooleanMap) (err error) {
	_ = chain // linter
	for _, log := range logs {
		for _, topic := range log.Topics {
			str := string(topic.Hex()[2:])
			if addr, ok := IsImplicitAddress(str); ok {
				addAddressToMaps(addr, log.BlockNumber, log.TransactionIndex, addrMap)
			}
		}

		if len(log.Data) > 2 {
			inputData := log.Data[2:]
			for i := 0; i < len(inputData)/64; i++ {
				str := string(inputData[i*64 : (i+1)*64])
				if addr, ok := IsImplicitAddress(str); ok {
					addAddressToMaps(addr, log.BlockNumber, log.TransactionIndex, addrMap)
				}
			}
		}
	}

	return
}

// UniqFromTraces extracts addresses from traces
func UniqFromTraces(chain string, traces []types.Trace, addrMap AddressBooleanMap) (err error) {
	conn := rpc.TempConnection(chain)

	for _, trace := range traces {
		bn := base.Blknum(trace.BlockNumber)
		txid := trace.TransactionIndex

		addAddressToMaps(trace.Action.From, bn, txid, addrMap)
		addAddressToMaps(trace.Action.To, bn, txid, addrMap)

		if trace.TraceType == "call" {
			// If it's a call, get the to and from, we're done

		} else if trace.TraceType == "reward" {
			if trace.Action.RewardType == "block" {
				author := trace.Action.Author
				fakeId := types.BlockReward
				if author.IsPrecompile() {
					author = base.SentinelAddr
					fakeId = types.MisconfigReward
				}
				addAddressToMaps(author, bn, fakeId, addrMap)

			} else if trace.Action.RewardType == "uncle" {
				author := trace.Action.Author
				fakeId := types.UncleReward
				if author.IsPrecompile() {
					author = base.SentinelAddr
					fakeId = types.MisconfigReward
				}
				addAddressToMaps(author, bn, fakeId, addrMap)

			} else if trace.Action.RewardType == "external" {
				// This only happens in xDai as far as we know...
				addAddressToMaps(trace.Action.Author, bn, types.ExternalReward, addrMap)

			} else {
				logger.Warn(fmt.Sprintf("Unknown reward type %s for trace: %d.%d.%d", trace.Action.RewardType, trace.BlockNumber, trace.TransactionIndex, trace.TraceIndex))
				return err
			}

		} else if trace.TraceType == "suicide" {
			// add the contract that died, and where it sent it's money
			addAddressToMaps(trace.Action.RefundAddress, bn, txid, addrMap)
			addAddressToMaps(trace.Action.Address, bn, txid, addrMap)

		} else if trace.TraceType == "create" {
			if trace.Result != nil {
				// may be both...record the self-destruct instead of the creation since we can only report on one
				addAddressToMaps(trace.Result.Address, bn, txid, addrMap)
			}

			// If it's a top level trace, then the call data is the init,
			// so to match with TrueBlocks, we just parse init
			if len(trace.TraceAddress) == 0 {
				if len(trace.Action.Init) > 10 {
					initData := trace.Action.Init[10:]
					for i := 0; i < len(initData)/64; i++ {
						str := string(initData[i*64 : (i+1)*64])
						if addr, ok := IsImplicitAddress(str); ok {
							addAddressToMaps(addr, bn, txid, addrMap)
						}
					}
				}
			}

			// Handle contract creations that may have errored out
			if trace.Action.To.IsZero() {
				if trace.Result != nil && trace.Result.Address.IsZero() {
					if trace.Error != "" {
						if receipt, err := conn.GetReceiptNoTimestamp(bn, txid); err == nil {
							addAddressToMaps(receipt.ContractAddress, bn, txid, addrMap)
						}
					}
				}
			}

		} else {
			if len(trace.TraceType) > 0 && trace.BlockNumber != 0 {
				logger.Warn(fmt.Sprintf("Unknown trace type %s for trace: %d.%d.%d", trace.TraceType, trace.BlockNumber, trace.TransactionIndex, trace.TraceIndex))
			}
			return err
		}

		// Try to get addresses from the input data
		if len(trace.Action.Input) > 10 {
			inputData := trace.Action.Input[10:]
			for i := 0; i < len(inputData)/64; i++ {
				str := string(inputData[i*64 : (i+1)*64])
				if addr, ok := IsImplicitAddress(str); ok {
					addAddressToMaps(addr, bn, txid, addrMap)
				}
			}
		}

		// Parse output of trace
		if trace.Result != nil && len(trace.Result.Output) > 2 {
			outputData := trace.Result.Output[2:]
			for i := 0; i < len(outputData)/64; i++ {
				str := string(outputData[i*64 : (i+1)*64])
				if addr, ok := IsImplicitAddress(str); ok {
					addAddressToMaps(addr, bn, txid, addrMap)
				}
			}
		}
	}

	return
}

var mapSync sync.Mutex

// addAddressToMaps helps keep track of appearances for an address. An appearance is inserted into `appsMap`
// if we've never seen this appearance before. `appsMap` is used to build the appearance table when writing the
// chunk. `addrMap` helps eliminate duplicates and is used to build the address table when writing the chunk.
// Precompiles are ignored. If the given address string does not start with a lead `0x`, it is normalized.
func addAddressToMaps(address base.Address, bn base.Blknum, txid base.Txnum, addrMap AddressBooleanMap) {
	if address.IsPrecompile() {
		return
	}
	mapSync.Lock()
	defer mapSync.Unlock()
	addrMap.Insert(address.Hex(), bn, txid)
}
