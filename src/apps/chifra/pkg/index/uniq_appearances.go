package index

import (
	"fmt"
	"strings"
	"sync"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/ethereum/go-ethereum/common/hexutil"
)

// UniqFromReceipts extracts addresses from an array of receipts
func UniqFromReceipts(chain string, receipts []types.SimpleReceipt, addrMap AddressBooleanMap) (err error) {
	for _, receipt := range receipts {
		created := receipt.ContractAddress
		addAddressToMaps(created.Hex(), receipt.BlockNumber, receipt.TransactionIndex, addrMap)
		if err := UniqFromLogs(chain, receipt.Logs, addrMap); err != nil {
			return err
		}
	}
	return nil
}

// UniqFromLogs extracts addresses from the logs
func UniqFromLogs(chain string, logs []types.SimpleLog, addrMap AddressBooleanMap) (err error) {
	for _, log := range logs {
		log := log
		for _, topic := range log.Topics {
			str := string(topic.Hex()[2:])
			if IsImplicitAddress(str) {
				addAddressToMaps(str, log.BlockNumber, log.TransactionIndex, addrMap)
			}
		}

		if len(log.Data) > 2 {
			inputData := log.Data[2:]
			for i := 0; i < len(inputData)/64; i++ {
				str := string(inputData[i*64 : (i+1)*64])
				if IsImplicitAddress(str) {
					addAddressToMaps(str, log.BlockNumber, log.TransactionIndex, addrMap)
				}
			}
		}
	}

	return
}

// UniqFromTraces extracts addresses from traces
func UniqFromTraces(chain string, traces []types.SimpleTrace, addrMap AddressBooleanMap) (err error) {
	conn := rpc.TempConnection(chain)

	for _, trace := range traces {
		trace := trace
		bn := base.Blknum(trace.BlockNumber)
		txid := uint64(trace.TransactionIndex)

		from := trace.Action.From.Hex()
		addAddressToMaps(from, bn, txid, addrMap)

		to := trace.Action.To.Hex()
		addAddressToMaps(to, bn, txid, addrMap)

		if trace.TraceType == "call" {
			// If it's a call, get the to and from, we're done

		} else if trace.TraceType == "reward" {
			if trace.Action.RewardType == "block" {
				author := trace.Action.Author.Hex()
				a := base.HexToAddress(author)
				if a.IsZero() {
					// Early clients allowed misconfigured miner settings with address
					// 0x0 (reward got burned). We enter a false record with a false tx_id
					// to account for this.
					author = "0xdeaddeaddeaddeaddeaddeaddeaddeaddeaddead"
					addAddressToMaps(author, bn, 99997, addrMap)

				} else {
					addAddressToMaps(author, bn, 99999, addrMap)

				}

			} else if trace.Action.RewardType == "uncle" {
				author := trace.Action.Author.Hex()
				a := base.HexToAddress(author)
				if a.IsZero() {
					// Early clients allowed misconfigured miner settings with address
					// 0x0 (reward got burned). We enter a false record with a false tx_id
					// to account for this.
					author = "0xdeaddeaddeaddeaddeaddeaddeaddeaddeaddead"
					addAddressToMaps(author, bn, 99998, addrMap)

				} else {
					addAddressToMaps(author, bn, 99998, addrMap)

				}

			} else if trace.Action.RewardType == "external" {
				// This only happens in xDai as far as we know...
				author := trace.Action.Author.Hex()
				addAddressToMaps(author, bn, 99996, addrMap)

			} else {
				fmt.Println("Unknown reward type", trace.Action.RewardType)
				return err
			}

		} else if trace.TraceType == "suicide" {
			// add the contract that died, and where it sent it's money
			refundAddress := trace.Action.RefundAddress.Hex()
			addAddressToMaps(refundAddress, bn, txid, addrMap)

			address := trace.Action.Address.Hex()
			addAddressToMaps(address, bn, txid, addrMap)

		} else if trace.TraceType == "create" {
			if trace.Result != nil {
				// may be both...record the self-destruct instead of the creation since we can only report on one
				address := trace.Result.Address.Hex()
				addAddressToMaps(address, bn, txid, addrMap)
			}

			// If it's a top level trace, then the call data is the init,
			// so to match with TrueBlocks, we just parse init
			if len(trace.TraceAddress) == 0 {
				if len(trace.Action.Init) > 10 {
					initData := trace.Action.Init[10:]
					for i := 0; i < len(initData)/64; i++ {
						str := string(initData[i*64 : (i+1)*64])
						if IsImplicitAddress(str) {
							addAddressToMaps(str, bn, txid, addrMap)
						}
					}
				}
			}

			// Handle contract creations that may have errored out
			if trace.Action.To.IsZero() {
				if trace.Result != nil && trace.Result.Address.IsZero() {
					if trace.Error != "" {
						receipt, err := conn.GetReceiptNoTimestamp(bn, txid)
						if err != nil {
							msg := fmt.Sprintf("rpcCall failed at block %d, tx %d hash %s err %s", bn, txid, trace.TransactionHash, err)
							logger.Warn(colors.Red, msg, colors.Off)
							// TODO: This is possibly an error in Erigon - remove it when they fix this issue:
							// TODO: https://github.com/ledgerwatch/erigon/issues/6956. It may require a
							// TODO: full resync. Yes, the problem appears to be this specific. The follow
							// TODO: hack (which tries to correct the problem) may well not work, but
							// TODO: the hope is that these will have already been picked up by the traces.
							// TODO: When fixed, we need to re-scrape from block 16,600,000. This map (which was
							// TODO: retrieved from Nethermind) tries to repair the missing data by marking (for
							// TODO: each transaction) any smart contracts created.
							fixMap := map[string]string{
								"16616983-242": "0x6784d7583cf2528daa270b555a4cb5376648488f",
								"16618181-146": "0x86494c70df6d3416bb4f536a82533b6120c52cde",
								"16618196-18":  "0x40d7b756557d9f7a5655ff70b3253a07f714807a",
								"16620128-12":  "0xb8fb9a557d19d5266f1ba1724445ee2436e3c626",
								"16620182-35":  "0x708bf2bf05492a5644787c134cf8a64e82fa4c52",
								"16621080-107": "0x23c84318fb83ee62e059679cddb3914c923da871",
								"16623590-179": "0xe88d3857676adf23d8324231eabee6ac390f666e",
								"16623602-106": "0x473a0524a25c252bc65a023c8b8476b1eb6ac805",
								"16626181-115": "0x010d9eb886f5b1a0fbef58bca722079e9ac75275",
								"16627272-125": "0xdfd76821bebdbe589f74d311dff4f5859995cda4",
								"16628102-66":  "0xddec22d76cfb1aded71c2f7b64ff768d207d615d",
							}
							key := fmt.Sprintf("%d-%d", bn, txid)
							msg = err.Error()
							if msg != "empty hex string" {
								// not the error we're looking for
								return err
							}

							if len(fixMap[key]) > 0 {
								// both are true - the error is `empty hex string` and we have a fix
								msg = fmt.Sprintf("Corrected %d, tx %d adds %s", bn, txid, fixMap[key])
								logger.Warn(colors.Red, msg, colors.Off)
								addAddressToMaps(fixMap[key], bn, txid, addrMap)
							}

						} else {
							addr := hexutil.Encode(receipt.ContractAddress.Bytes())
							addAddressToMaps(addr, bn, txid, addrMap)
						}
					}
				}
			}

		} else {
			fmt.Println("Unknown trace type", trace.TraceType)
			return err
		}

		// Try to get addresses from the input data
		if len(trace.Action.Input) > 10 {
			inputData := trace.Action.Input[10:]
			for i := 0; i < len(inputData)/64; i++ {
				str := string(inputData[i*64 : (i+1)*64])
				if IsImplicitAddress(str) {
					addAddressToMaps(str, bn, txid, addrMap)
				}
			}
		}

		// Parse output of trace
		if trace.Result != nil && len(trace.Result.Output) > 2 {
			outputData := trace.Result.Output[2:]
			for i := 0; i < len(outputData)/64; i++ {
				str := string(outputData[i*64 : (i+1)*64])
				if IsImplicitAddress(str) {
					addAddressToMaps(str, bn, txid, addrMap)
				}
			}
		}
	}

	return
}

var mapSync sync.Mutex

// addAddressToMaps help keep track of appearances for an address. An appearance is inserted into `appsMap`
// if we've never seen this appearance before. `appsMap` is used to build the appearance table when writing the
// chunk. `addrMap` helps eliminate duplicates and is used to build the address table when writing the chunk.
// Precompiles are ignored. If the given address string does not start with a lead `0x`, it is normalized.
func addAddressToMaps(address string, bn, txid uint64, addrMap AddressBooleanMap) {
	if base.IsPrecompile(address) {
		return
	}

	// Normalize implicit strings. (Implicit strings come in 32-bytes long with no leading `0x`.)
	if !strings.HasPrefix(address, "0x") {
		addr := base.HexToAddress("0x" + address)
		address = addr.Hex()
	}

	mapSync.Lock()
	defer mapSync.Unlock()

	key := fmt.Sprintf("%s\t%09d\t%05d", address, bn, txid)
	addrMap[key] = true
}
