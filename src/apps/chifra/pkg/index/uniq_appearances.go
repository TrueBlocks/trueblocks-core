package index

import (
	"fmt"
	"strconv"
	"strings"
	"sync"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
	"github.com/ethereum/go-ethereum/common"
	"github.com/ethereum/go-ethereum/common/hexutil"
)

// ExtractUniqFromLogs extracts addresses from the logs
func ExtractUniqFromLogs(chain string, bn int, logs *Logs, appsMap AddressAppearanceMap, addressMap AddressBooleanMap) (err error) {
	if logs.Result == nil || len(logs.Result) == 0 {
		return
	}

	for i := 0; i < len(logs.Result); i++ {
		txid, _ := strconv.ParseInt(logs.Result[i].TransactionIndex, 0, 32)
		for j := 0; j < len(logs.Result[i].Topics); j++ {
			addr := string(logs.Result[i].Topics[j][2:])
			AddImplicitToMaps(addr, bn, int(txid), appsMap, addressMap)
		}

		if len(logs.Result[i].Data) > 2 {
			inputData := logs.Result[i].Data[2:]
			for i := 0; i < len(inputData)/64; i++ {
				addr := string(inputData[i*64 : (i+1)*64])
				AddImplicitToMaps(addr, bn, int(txid), appsMap, addressMap)
			}
		}
	}

	return
}

// ExtractUniqFromTraces extracts addresses from traces
func ExtractUniqFromTraces(chain string, bn int, traces *Traces, appsMap AddressAppearanceMap, addressMap AddressBooleanMap) (err error) {
	if traces.Result == nil || len(traces.Result) == 0 {
		return
	}

	for i := 0; i < len(traces.Result); i++ {
		txid := traces.Result[i].TransactionPosition

		if traces.Result[i].Type == "call" {
			// If it's a call, get the to and from
			from := traces.Result[i].Action.From
			AddToMaps(from, bn, txid, appsMap, addressMap)

			to := traces.Result[i].Action.To
			AddToMaps(to, bn, txid, appsMap, addressMap)

		} else if traces.Result[i].Type == "reward" {
			if traces.Result[i].Action.RewardType == "block" {
				author := traces.Result[i].Action.Author
				if validate.IsZeroAddress(author) {
					// Early clients allowed misconfigured miner settings with address
					// 0x0 (reward got burned). We enter a false record with a false tx_id
					// to account for this.
					author = "0xdeaddeaddeaddeaddeaddeaddeaddeaddeaddead"
					AddToMaps(author, bn, 99997, appsMap, addressMap)

				} else {
					AddToMaps(author, bn, 99999, appsMap, addressMap)

				}

			} else if traces.Result[i].Action.RewardType == "uncle" {
				author := traces.Result[i].Action.Author
				if validate.IsZeroAddress(author) {
					// Early clients allowed misconfigured miner settings with address
					// 0x0 (reward got burned). We enter a false record with a false tx_id
					// to account for this.
					author = "0xdeaddeaddeaddeaddeaddeaddeaddeaddeaddead"
					AddToMaps(author, bn, 99998, appsMap, addressMap)

				} else {
					AddToMaps(author, bn, 99998, appsMap, addressMap)

				}

			} else if traces.Result[i].Action.RewardType == "external" {
				// This only happens in xDai as far as we know...
				author := traces.Result[i].Action.Author
				AddToMaps(author, bn, 99996, appsMap, addressMap)

			} else {
				fmt.Println("Unknown reward type", traces.Result[i].Action.RewardType)
				return err
			}

		} else if traces.Result[i].Type == "suicide" {
			// add the contract that died, and where it sent it's money
			address := traces.Result[i].Action.Address
			AddToMaps(address, bn, txid, appsMap, addressMap)

			refundAddress := traces.Result[i].Action.RefundAddress
			AddToMaps(refundAddress, bn, txid, appsMap, addressMap)

		} else if traces.Result[i].Type == "create" {
			// add the creator, and the new address name
			from := traces.Result[i].Action.From
			AddToMaps(from, bn, txid, appsMap, addressMap)

			address := traces.Result[i].Result.Address
			AddToMaps(address, bn, txid, appsMap, addressMap)

			// If it's a top level trace, then the call data is the init,
			// so to match with TrueBlocks, we just parse init
			if len(traces.Result[i].TraceAddress) == 0 {
				if len(traces.Result[i].Action.Init) > 10 {
					initData := traces.Result[i].Action.Init[10:]
					for i := 0; i < len(initData)/64; i++ {
						addr := string(initData[i*64 : (i+1)*64])
						AddImplicitToMaps(addr, bn, txid, appsMap, addressMap)
					}
				}
			}

			// Handle contract creations that may have errored out
			if traces.Result[i].Action.To == "" {
				if traces.Result[i].Result.Address == "" {
					if traces.Result[i].Error != "" {
						// TODO: Why does this interface always accept nil and zero at the end?
						receipt, err := rpcClient.GetTransactionReceipt(chain, rpcClient.ReceiptQuery{
							Bn:      uint64(bn),
							Txid:    uint64(txid),
							NeedsTs: false,
						})
						if err != nil {
							msg := fmt.Sprintf("rpcCall failed at block %d, tx %d hash %s err %s", bn, txid, traces.Result[i].TransactionHash, err)
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
								AddToMaps(fixMap[key], bn, txid, appsMap, addressMap)
							}

						} else {
							addr := hexutil.Encode(receipt.ContractAddress.Bytes())
							AddToMaps(addr, bn, txid, appsMap, addressMap)
						}
					}
				}
			}

		} else {
			fmt.Println("Unknown trace type", traces.Result[i].Type)
			return err
		}

		// Try to get addresses from the input data
		if len(traces.Result[i].Action.Input) > 10 {
			inputData := traces.Result[i].Action.Input[10:]
			//fmt.Println("Input data:", inputData, len(inputData))
			for i := 0; i < len(inputData)/64; i++ {
				addr := string(inputData[i*64 : (i+1)*64])
				AddImplicitToMaps(addr, bn, txid, appsMap, addressMap)
			}
		}

		// Parse output of trace
		if len(traces.Result[i].Result.Output) > 2 {
			outputData := traces.Result[i].Result.Output[2:]
			for i := 0; i < len(outputData)/64; i++ {
				addr := string(outputData[i*64 : (i+1)*64])
				AddImplicitToMaps(addr, bn, txid, appsMap, addressMap)
			}
		}
	}

	return
}

var mapSync sync.Mutex

// AddToMaps adds an address to two maps. The first is a map of addresses to appearance records.
// The second is a map of addresses to booleans. The boolean map is used to build the address table in the chunk.
func AddToMaps(address string, bn, txid int, appsMap AddressAppearanceMap, addressMap AddressBooleanMap) {
	isPrecompile := !base.NotPrecompile(address)
	if isPrecompile {
		return
	}

	// Make sure we have a 20 byte '0x' prefixed string (implicit strings come in as 32-byte, non-0x-prefixed strings)
	if !strings.HasPrefix(address, "0x") {
		address = hexutil.Encode(common.HexToAddress(address).Bytes())
	}

	mapSync.Lock()
	defer mapSync.Unlock()

	key := fmt.Sprintf("%s\t%09d\t%05d", address, bn, txid)
	if !addressMap[key] {
		appsMap[address] = append(appsMap[address], AppearanceRecord{
			BlockNumber:   uint32(bn),
			TransactionId: uint32(txid),
		})
	}
	addressMap[key] = true
}

// AddImplicitToMaps determines if an address is implicit and, if so, adds it to the maps.
func AddImplicitToMaps(address string, bn, txid int, appsMap AddressAppearanceMap, addressMap AddressBooleanMap) {
	isImplicit := IsImplicitAddress(address)
	if !isImplicit {
		return
	}

	AddToMaps(address, bn, txid, appsMap, addressMap)
}
