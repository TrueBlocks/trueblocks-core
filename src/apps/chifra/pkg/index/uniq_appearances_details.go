package index

import (
	"errors"
	"fmt"
	"strings"
	"sync"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
	"github.com/ethereum/go-ethereum/common"
	"github.com/ethereum/go-ethereum/common/hexutil"
)

type UniqProcFunc func(s *types.SimpleAppearance) error

func UniqFromTransDetails(chain string, procFunc UniqProcFunc, flow string, trans *types.SimpleTransaction, ts int64, addrMap AddressBooleanMap) error {
	bn := trans.BlockNumber
	txid := trans.TransactionIndex
	traceid := utils.NOPOS
	from := trans.From.Hex()
	StreamAppearance(procFunc, flow, "from", from, bn, txid, traceid, ts, addrMap)

	to := trans.To.Hex()
	StreamAppearance(procFunc, flow, "to", to, bn, txid, traceid, ts, addrMap)

	if !trans.Receipt.ContractAddress.IsZero() {
		contract := trans.Receipt.ContractAddress.Hex()
		StreamAppearance(procFunc, flow, "creation", contract, bn, txid, traceid, ts, addrMap)
	}

	if len(trans.Input) > 10 {
		reason := "input"
		inputData := trans.Input[10:]
		for i := 0; i < len(inputData)/64; i++ {
			str := string(inputData[i*64 : (i+1)*64])
			if IsImplicitAddress(str) {
				StreamAppearance(procFunc, flow, str, reason, bn, txid, traceid, ts, addrMap)
			}
		}
	}

	if err := UniqFromLogsDetails(chain, procFunc, flow, trans.Receipt.Logs, ts, addrMap); err != nil {
		return err
	}

	if err := UniqFromTracesDetails(chain, procFunc, flow, trans.Traces, ts, addrMap); err != nil {
		return err
	}

	return nil
}

// UniqFromLogsDetails extracts addresses from the logs
func UniqFromLogsDetails(chain string, procFunc UniqProcFunc, flow string, logs []types.SimpleLog, ts int64, addrMap AddressBooleanMap) (err error) {
	traceid := utils.NOPOS
	for l, log := range logs {
		log := log
		generator := log.Address.Hex()
		reason := fmt.Sprintf("log_%d_generator", l)
		StreamAppearance(procFunc, flow, reason, generator, log.BlockNumber, log.TransactionIndex, traceid, ts, addrMap)

		for t, topic := range log.Topics {
			str := string(topic.Hex()[2:])
			if IsImplicitAddress(str) {
				reason := fmt.Sprintf("log_%d_topic_%d", l, t)
				StreamAppearance(procFunc, flow, reason, str, log.BlockNumber, log.TransactionIndex, traceid, ts, addrMap)
			}
		}

		if len(log.Data) > 2 {
			reason := fmt.Sprintf("log_%d_data", l)
			inputData := log.Data[2:]
			for i := 0; i < len(inputData)/64; i++ {
				str := string(inputData[i*64 : (i+1)*64])
				if IsImplicitAddress(str) {
					StreamAppearance(procFunc, flow, reason, str, log.BlockNumber, log.TransactionIndex, traceid, ts, addrMap)
				}
			}
		}
	}

	return
}

func traceReason(i uint64, trace *types.SimpleTrace, r string) string {
	switch r {
	case "from":
		fallthrough
	case "to":
		fallthrough
	case "input":
		fallthrough
	case "output":
		fallthrough
	case "self-destruct":
		fallthrough
	case "refund":
		fallthrough
	case "creation":
		fallthrough
	case "code":
		if i == 0 {
			return r
		} else {
			a := ""
			for i, x := range trace.TraceAddress {
				if i > 0 {
					a += fmt.Sprintf("%d_", x)
				}
			}
			if len(a) > 0 {
				a = "[" + strings.Trim(a, "_") + "]_"
			}
			return fmt.Sprintf("trace_%d_%s%s", i, a, r)
		}
	default:
		return "unknown"
	}
}

// UniqFromTracesDetails extracts addresses from traces
func UniqFromTracesDetails(chain string, procFunc UniqProcFunc, flow string, traces []types.SimpleTrace, ts int64, addrMap AddressBooleanMap) (err error) {
	for _, trace := range traces {
		trace := trace
		traceid := trace.TraceIndex
		bn := base.Blknum(trace.BlockNumber)
		txid := uint64(trace.TransactionIndex)

		from := trace.Action.From.Hex()
		StreamAppearance(procFunc, flow, traceReason(traceid, &trace, "from"), from, bn, txid, traceid, ts, addrMap)

		to := trace.Action.To.Hex()
		StreamAppearance(procFunc, flow, traceReason(traceid, &trace, "to"), to, bn, txid, traceid, ts, addrMap)

		if trace.TraceType == "call" {
			// If it's a call, get the to and from, we're done

		} else if trace.TraceType == "reward" {
			if trace.Action.RewardType == "block" {
				author := trace.Action.Author.Hex()
				falseTxid := uint64(99999)
				// Early clients allowed misconfigured miner settings with address 0x0 (reward got
				// burned). We enter a false record with a false tx_id to account for this.
				if validate.IsZeroAddress(author) {
					author = "0xdeaddeaddeaddeaddeaddeaddeaddeaddeaddead"
					falseTxid = uint64(99997)
				}
				StreamAppearance(procFunc, flow, "miner", author, bn, falseTxid, traceid, ts, addrMap)

			} else if trace.Action.RewardType == "uncle" {
				author := trace.Action.Author.Hex()
				falseTxid := uint64(99998)
				// Early clients allowed misconfigured miner settings with address 0x0 (reward got
				// burned). We enter a false record with a false tx_id to account for this.
				if validate.IsZeroAddress(author) {
					author = "0xdeaddeaddeaddeaddeaddeaddeaddeaddeaddead"
					falseTxid = uint64(99998)
				}
				StreamAppearance(procFunc, flow, "uncle", author, bn, falseTxid, traceid, ts, addrMap)

			} else if trace.Action.RewardType == "external" {
				// This only happens in xDai as far as we know...
				author := trace.Action.Author.Hex()
				falseTxid := uint64(99996)
				StreamAppearance(procFunc, flow, "external", author, bn, falseTxid, traceid, ts, addrMap)

			} else {
				return errors.New("Unknown reward type" + trace.Action.RewardType)
			}

		} else if trace.TraceType == "suicide" {
			// add the contract that died, and where it sent it's money
			refundAddress := trace.Action.RefundAddress.Hex()
			StreamAppearance(procFunc, flow, traceReason(traceid, &trace, "refund"), refundAddress, bn, txid, traceid, ts, addrMap)

			address := trace.Action.Address.Hex()
			StreamAppearance(procFunc, flow, traceReason(traceid, &trace, "self-destruct"), address, bn, txid, traceid, ts, addrMap)

		} else if trace.TraceType == "create" {
			if trace.Result != nil {
				// may be both...record the self-destruct instead of the creation since we can only report on one
				address := trace.Result.Address.Hex()
				StreamAppearance(procFunc, flow, traceReason(traceid, &trace, "self-destruct"), address, bn, txid, traceid, ts, addrMap)
			}

			// If it's a top level trace, then the call data is the init,
			// so to match with TrueBlocks, we just parse init
			if len(trace.TraceAddress) == 0 {
				if len(trace.Action.Init) > 10 {
					initData := trace.Action.Init[10:]
					for i := 0; i < len(initData)/64; i++ {
						str := string(initData[i*64 : (i+1)*64])
						if IsImplicitAddress(str) {
							StreamAppearance(procFunc, flow, traceReason(traceid, &trace, "code"), str, bn, txid, traceid, ts, addrMap)
						}
					}
				}
			}

			// Handle contract creations that may have errored out
			if trace.Action.To.IsZero() {
				if trace.Result != nil && trace.Result.Address.IsZero() {
					if trace.Error != "" {
						// TODO: Why does this interface always accept nil and zero at the end?
						receipt, err := rpcClient.GetTransactionReceipt(chain, rpcClient.ReceiptQuery{
							Bn:      uint64(bn),
							Txid:    uint64(txid),
							NeedsTs: false,
						})
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
								StreamAppearance(procFunc, flow, "creation", fixMap[key], bn, txid, traceid, ts, addrMap)
							}

						} else {
							addr := hexutil.Encode(receipt.ContractAddress.Bytes())
							StreamAppearance(procFunc, flow, "creation", addr, bn, txid, traceid, ts, addrMap)
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
					StreamAppearance(procFunc, flow, traceReason(traceid, &trace, "input"), str, bn, txid, traceid, ts, addrMap)
				}
			}
		}

		// Parse output of trace
		if trace.Result != nil && len(trace.Result.Output) > 2 {
			outputData := trace.Result.Output[2:]
			for i := 0; i < len(outputData)/64; i++ {
				str := string(outputData[i*64 : (i+1)*64])
				if IsImplicitAddress(str) {
					StreamAppearance(procFunc, flow, traceReason(traceid, &trace, "output"), str, bn, txid, traceid, ts, addrMap)
				}
			}
		}
	}

	return
}

var mapSync2 sync.Mutex

// StreamAppearance streams an appearance to the model channel if we've not seen this appearance before. We
// keep track of appearances we've seen with `appsMap`.
func StreamAppearance(procFunc UniqProcFunc, flow string, reason string, address string, bn, txid, traceid uint64, ts int64, addrMap AddressBooleanMap) {
	if base.IsPrecompile(address) {
		return
	}

	if len(flow) > 0 {
		switch flow {
		case "from":
			if !strings.Contains(reason, "from") {
				return
			}
		case "to":
			test := strings.Replace(reason, "topic", "", -1)
			test = strings.Replace(test, "generator", "", -1)
			if !strings.Contains(test, "to") {
				return
			}
		case "reward":
			if !strings.Contains(reason, "miner") && !strings.Contains(reason, "uncle") {
				return
			}
		default:
			logger.Error("Unknown flow:", flow)
		}
	}

	// Normalize implicit strings. (Implicit strings come in 32-bytes long with no leading `0x`.)
	if !strings.HasPrefix(address, "0x") {
		address = hexutil.Encode(common.HexToAddress(address).Bytes())
	}

	mapSync2.Lock()
	defer mapSync2.Unlock()

	key := fmt.Sprintf("%s\t%09d\t%05d", address, bn, txid)
	if !addrMap[key] {
		addrMap[key] = true
		s := &types.SimpleAppearance{
			Address:          base.HexToAddress(address),
			BlockNumber:      uint32(bn),
			TransactionIndex: uint32(txid),
			Reason:           reason,
			Timestamp:        ts,
			Date:             utils.FormattedDate(ts),
		}

		if traceid != utils.NOPOS {
			s.TraceIndex = uint32(traceid)
		}

		if procFunc != nil {
			procFunc(s)
		}
	}
}
