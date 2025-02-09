package uniq

import (
	"errors"
	"fmt"
	"sort"
	"strings"
	"sync"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/names"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

var AppearanceFmt = "%s\t%09d\t%05d"

type UniqProcFunc func(s *types.Appearance) error
type AddressBooleanMap map[string]bool

// Insert generates item's key according to `AppearanceFmt` and adds the item to the map
func (a *AddressBooleanMap) Insert(address string, bn base.Blknum, txid base.Txnum) string {
	key := fmt.Sprintf(AppearanceFmt, address, bn, txid)
	v := *a
	v[key] = true
	return key
}

func GetUniqAddressesInBlock(chain, flow string, conn *rpc.Connection, procFunc UniqProcFunc, bn base.Blknum) error {
	ts := conn.GetBlockTimestamp(bn)
	addrMap := AddressBooleanMap{}
	traceid := base.NOPOSN
	if bn == 0 {
		if namesMap, err := names.LoadNamesMap(chain, types.Prefund, []string{}); err != nil {
			return err
		} else {
			namesArray := make([]*types.Name, 0, len(namesMap))
			for _, name := range namesMap {
				namesArray = append(namesArray, &name)
			}
			sort.Slice(namesArray, func(i, j int) bool {
				return namesArray[i].Name < namesArray[j].Name
			})
			for i, name := range namesArray {
				tx_id := base.Txnum(i)
				address := name.Address.Hex()
				streamAppearance(procFunc, flow, "genesis", address, bn, tx_id, traceid, ts, addrMap)
			}
		}

	} else {
		if block, err := conn.GetBlockBodyByNumber(bn); err != nil {
			return err
		} else {
			author := block.Miner.Hex()
			fakeId := types.BlockReward
			if base.IsPrecompile(author) {
				// Some blocks have a misconfigured miner setting. We process this block, so that
				// every block gets a record, but it will be excluded from the index. See #3252.
				author = base.SentinelAddr.Hex()
				fakeId = types.MisconfigReward
			}
			streamAppearance(procFunc, flow, "miner", author, bn, fakeId, traceid, ts, addrMap)

			if uncles, err := conn.GetUncleBodiesByNumber(bn); err != nil {
				return err
			} else {
				for _, uncle := range uncles {
					author := uncle.Miner.Hex()
					fakeId := types.UncleReward
					if base.IsPrecompile(author) {
						// Some blocks have a misconfigured miner setting. We process this block, so that
						// every block gets a record, but it will be excluded from the index. See #3252.
						author = base.SentinelAddr.Hex()
						fakeId = types.MisconfigReward
					}
					streamAppearance(procFunc, flow, "uncle", author, bn, fakeId, traceid, ts, addrMap)
				}
			}

			for _, trans := range block.Transactions {
				if trans.Traces, err = conn.GetTracesByTransactionId(trans.BlockNumber, trans.TransactionIndex); err != nil {
					return err
				}
				if err = GetUniqAddressesInTransaction(chain, procFunc, flow, &trans, ts, addrMap, conn); err != nil {
					return err
				}
			}

			for _, withdrawal := range block.Withdrawals {
				streamAppearance(procFunc, flow, "withdrawal", withdrawal.Address.Hex(), bn, withdrawal.Index, traceid, ts, addrMap)
			}
		}
	}

	return nil
}

func GetUniqAddressesInTransaction(chain string, procFunc UniqProcFunc, flow string, trans *types.Transaction, ts base.Timestamp, addrMap AddressBooleanMap, conn *rpc.Connection) error {
	bn := trans.BlockNumber
	txid := trans.TransactionIndex
	traceid := base.NOPOSN
	from := trans.From.Hex()
	streamAppearance(procFunc, flow, "from", from, bn, txid, traceid, ts, addrMap)

	to := trans.To.Hex()
	streamAppearance(procFunc, flow, "to", to, bn, txid, traceid, ts, addrMap)

	if trans.Receipt != nil && !trans.Receipt.ContractAddress.IsZero() {
		contract := trans.Receipt.ContractAddress.Hex()
		streamAppearance(procFunc, flow, "creation", contract, bn, txid, traceid, ts, addrMap)
	}

	if len(trans.Input) > 10 {
		reason := "input"
		inputData := trans.Input[10:]
		for i := 0; i < len(inputData)/64; i++ {
			str := string(inputData[i*64 : (i+1)*64])
			if IsImplicitAddress(str) {
				streamAppearance(procFunc, flow, str, reason, bn, txid, traceid, ts, addrMap)
			}
		}
	}

	// TODO: See issue #3195 - there are addresses on the receipt that do not appear in traces
	if trans.Receipt != nil {
		if err := uniqFromLogsDetails(chain, procFunc, flow, trans.Receipt.Logs, ts, addrMap); err != nil {
			return err
		}
	}

	if err := uniqFromTracesDetails(chain, procFunc, flow, trans.Traces, ts, addrMap, conn); err != nil {
		return err
	}

	return nil
}

// uniqFromLogsDetails extracts addresses from the logs
func uniqFromLogsDetails(chain string, procFunc UniqProcFunc, flow string, logs []types.Log, ts base.Timestamp, addrMap AddressBooleanMap) (err error) {
	_ = chain // linter
	traceid := base.NOPOSN
	for l, log := range logs {
		generator := log.Address.Hex()
		reason := fmt.Sprintf("log_%d_generator", l)
		streamAppearance(procFunc, flow, reason, generator, log.BlockNumber, log.TransactionIndex, traceid, ts, addrMap)

		for t, topic := range log.Topics {
			str := string(topic.Hex()[2:])
			if IsImplicitAddress(str) {
				reason := fmt.Sprintf("log_%d_topic_%d", l, t)
				streamAppearance(procFunc, flow, reason, str, log.BlockNumber, log.TransactionIndex, traceid, ts, addrMap)
			}
		}

		if len(log.Data) > 2 {
			reason := fmt.Sprintf("log_%d_data", l)
			inputData := log.Data[2:]
			for i := 0; i < len(inputData)/64; i++ {
				str := string(inputData[i*64 : (i+1)*64])
				if IsImplicitAddress(str) {
					streamAppearance(procFunc, flow, reason, str, log.BlockNumber, log.TransactionIndex, traceid, ts, addrMap)
				}
			}
		}
	}

	return
}

func traceReason(traceId base.Tracenum, trace *types.Trace, r string) string {
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
		if traceId == 0 {
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
			return fmt.Sprintf("trace_%d_%s%s", traceId, a, r)
		}
	default:
		return "unknown"
	}
}

// uniqFromTracesDetails extracts addresses from traces
func uniqFromTracesDetails(chain string, procFunc UniqProcFunc, flow string, traces []types.Trace, ts base.Timestamp, addrMap AddressBooleanMap, conn *rpc.Connection) (err error) {
	_ = chain // linter
	for _, trace := range traces {
		traceid := trace.TraceIndex
		bn := base.Blknum(trace.BlockNumber)
		txid := trace.TransactionIndex

		from := trace.Action.From.Hex()
		streamAppearance(procFunc, flow, traceReason(traceid, &trace, "from"), from, bn, txid, traceid, ts, addrMap)

		to := trace.Action.To.Hex()
		streamAppearance(procFunc, flow, traceReason(traceid, &trace, "to"), to, bn, txid, traceid, ts, addrMap)

		if trace.TraceType == "call" {
			// If it's a call, get the to and from, we're done

		} else if trace.TraceType == "reward" {
			if trace.Action.RewardType == "block" {
				author := trace.Action.Author.Hex()
				fakeId := types.BlockReward
				if base.IsPrecompile(author) {
					// Some blocks have a misconfigured miner setting. We process this block, so that
					// every block gets a record, but it will be excluded from the index. See #3252.
					author = base.SentinelAddr.Hex()
					fakeId = types.MisconfigReward
				}
				streamAppearance(procFunc, flow, "miner", author, bn, fakeId, traceid, ts, addrMap)

			} else if trace.Action.RewardType == "uncle" {
				author := trace.Action.Author.Hex()
				fakeId := types.UncleReward
				if base.IsPrecompile(author) {
					// Some blocks have a misconfigured miner setting. We process this block, so that
					// every block gets a record, but it will be excluded from the index. See #3252.
					author = base.SentinelAddr.Hex()
					fakeId = types.MisconfigReward
				}
				streamAppearance(procFunc, flow, "uncle", author, bn, fakeId, traceid, ts, addrMap)

			} else if trace.Action.RewardType == "external" {
				author := trace.Action.Author.Hex()
				fakeId := types.ExternalReward
				if base.IsPrecompile(author) {
					// Some blocks have a misconfigured miner setting. We process this block, so that
					// every block gets a record, but it will be excluded from the index. See #3252.
					author = base.SentinelAddr.Hex()
					fakeId = types.MisconfigReward
				}
				streamAppearance(procFunc, flow, "external", author, bn, fakeId, traceid, ts, addrMap)

			} else {
				return errors.New("Unknown reward type" + trace.Action.RewardType)
			}

		} else if trace.TraceType == "suicide" {
			// add the contract that died, and where it sent it's money
			refundAddress := trace.Action.RefundAddress.Hex()
			streamAppearance(procFunc, flow, traceReason(traceid, &trace, "refund"), refundAddress, bn, txid, traceid, ts, addrMap)

			address := trace.Action.Address.Hex()
			streamAppearance(procFunc, flow, traceReason(traceid, &trace, "self-destruct"), address, bn, txid, traceid, ts, addrMap)

		} else if trace.TraceType == "create" {
			if trace.Result != nil {
				// may be both...record the self-destruct instead of the creation since we can only report on one
				address := trace.Result.Address.Hex()
				streamAppearance(procFunc, flow, traceReason(traceid, &trace, "self-destruct"), address, bn, txid, traceid, ts, addrMap)
			}

			// If it's a top level trace, then the call data is the init,
			// so to match with TrueBlocks, we just parse init
			if len(trace.TraceAddress) == 0 {
				if len(trace.Action.Init) > 10 {
					initData := trace.Action.Init[10:]
					for i := 0; i < len(initData)/64; i++ {
						str := string(initData[i*64 : (i+1)*64])
						if IsImplicitAddress(str) {
							streamAppearance(procFunc, flow, traceReason(traceid, &trace, "code"), str, bn, txid, traceid, ts, addrMap)
						}
					}
				}
			}

			// Handle contract creations that may have errored out
			if trace.Action.To.IsZero() {
				if trace.Result != nil && trace.Result.Address.IsZero() {
					if trace.Error != "" {
						if receipt, err := conn.GetReceiptNoTimestamp(bn, txid); err == nil {
							address := receipt.ContractAddress.Hex()
							streamAppearance(procFunc, flow, traceReason(traceid, &trace, "self-destruct"), address, bn, txid, traceid, ts, addrMap)
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
				if IsImplicitAddress(str) {
					streamAppearance(procFunc, flow, traceReason(traceid, &trace, "input"), str, bn, txid, traceid, ts, addrMap)
				}
			}
		}

		// Parse output of trace
		if trace.Result != nil && len(trace.Result.Output) > 2 {
			outputData := trace.Result.Output[2:]
			for i := 0; i < len(outputData)/64; i++ {
				str := string(outputData[i*64 : (i+1)*64])
				if IsImplicitAddress(str) {
					streamAppearance(procFunc, flow, traceReason(traceid, &trace, "output"), str, bn, txid, traceid, ts, addrMap)
				}
			}
		}
	}

	return
}

var mapSync2 sync.Mutex

// streamAppearance streams an appearance to the model channel if we've not seen this appearance before. We
// keep track of appearances we've seen with `appsMap`.
func streamAppearance(procFunc UniqProcFunc, flow string, reason string, address string, bn base.Blknum, txid base.Txnum, traceid base.Tracenum, ts base.Timestamp, addrMap AddressBooleanMap) {
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
		addr := base.HexToAddress("0x" + address)
		address = addr.Hex()
	}

	key := fmt.Sprintf("%s\t%09d\t%05d", address, bn, txid)

	mapSync2.Lock()
	if !addrMap[key] {
		addrMap[key] = true
		mapSync2.Unlock()

		s := &types.Appearance{
			Address:          base.HexToAddress(address),
			BlockNumber:      uint32(bn),
			TransactionIndex: uint32(txid),
			Reason:           reason,
			Timestamp:        ts,
		}

		if traceid != base.NOPOSN {
			s.TraceIndex = uint32(traceid)
		}

		if procFunc != nil {
			_ = procFunc(s)
		}
	} else {
		mapSync2.Unlock()
	}
}
