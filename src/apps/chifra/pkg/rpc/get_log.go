package rpc

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc/query"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

// GetLogsByFilter returns the logs given a filter
func (conn *Connection) GetLogsByFilter(filter types.SimpleLogFilter) ([]types.SimpleLog, error) {
	return conn.getLogsSimple(filter)
}

// GetLogsByNumber returns the logs of a block
func (conn *Connection) GetLogsByNumber(bn base.Blknum, ts base.Timestamp) ([]types.SimpleLog, error) {
	if conn.StoreReadable() {
		logGroup := &types.SimpleLogGroup{
			BlockNumber:      bn,
			TransactionIndex: utils.NOPOS,
		}
		if err := conn.Store.Read(logGroup, nil); err == nil {
			return logGroup.Logs, nil
		}
	}

	filter := types.SimpleLogFilter{
		FromBlock: bn,
		ToBlock:   bn,
	}

	if logs, err := conn.getLogsSimple(filter); err != nil {
		return logs, err
	} else {
		if conn.StoreWritable() && conn.EnabledMap["logs"] && base.IsFinal(conn.LatestBlockTimestamp, ts) {
			logGroup := &types.SimpleLogGroup{
				Logs:             logs,
				BlockNumber:      bn,
				TransactionIndex: utils.NOPOS,
			}
			_ = conn.Store.Write(logGroup, nil)
		}
		return logs, err
	}
}

// GetLogsCountInBlock returns the number of logs in a block
func (conn *Connection) GetLogsCountInBlock(bn base.Blknum, ts base.Timestamp) (uint64, error) {
	if logs, err := conn.GetLogsByNumber(bn, ts); err != nil {
		return 0, err
	} else {
		return uint64(len(logs)), nil
	}
}

func (conn *Connection) getLogsSimple(filter types.SimpleLogFilter) ([]types.SimpleLog, error) {
	p := struct {
		FromBlock string   `json:"fromBlock"`
		ToBlock   string   `json:"toBlock"`
		Address   []string `json:"address"` // sorry for the weird conversion...
		Topics    []string `json:"topics"`
	}{
		FromBlock: fmt.Sprintf("0x%x", filter.FromBlock),
		ToBlock:   fmt.Sprintf("0x%x", filter.ToBlock),
	}
	for _, addr := range filter.Emitters {
		p.Address = append(p.Address, addr.Hex())
	}
	for _, topic := range filter.Topics {
		p.Topics = append(p.Topics, topic.Hex())
	}

	method := "eth_getLogs"
	params := query.Params{p}

	if rawLogs, err := query.QuerySlice[types.RawLog](conn.Chain, method, params); err != nil {
		return []types.SimpleLog{}, err
	} else {
		curBlock := utils.NOPOS
		curTs := utils.NOPOSI
		var ret []types.SimpleLog
		for _, rawLog := range rawLogs {
			bn := utils.MustParseUint(rawLog.BlockNumber)
			if bn != curBlock {
				curTs = conn.GetBlockTimestamp(bn)
				curBlock = bn
			}
			log := types.SimpleLog{
				Address:          base.HexToAddress(rawLog.Address),
				BlockHash:        base.HexToHash(rawLog.BlockHash),
				BlockNumber:      utils.MustParseUint(rawLog.BlockNumber),
				Data:             rawLog.Data,
				LogIndex:         utils.MustParseUint(rawLog.LogIndex),
				Timestamp:        curTs,
				TransactionHash:  base.HexToHash(rawLog.TransactionHash),
				TransactionIndex: utils.MustParseUint(rawLog.TransactionIndex),
			}
			for _, topic := range rawLog.Topics {
				log.Topics = append(log.Topics, base.HexToHash(topic))
			}
			log.SetRaw(&rawLog)
			ret = append(ret, log)
		}
		return ret, nil
	}
}

/*
   //---------------------------------------------------------------------------

   	string_q CLogFilter::toRPC(void) const {
   	    ostringstream out;

   	    if (!blockHash.empty())
   	        out << "\"blockHash\": \"" << blockHash << "\"";

   	    if (fromBlock != 0) {
   	        out << (out.str().empty() ? "" : ",");
   	        out << "\"fromBlock\":\"" << uint_2_Hex(fromBlock) << "\"";
   	    }

   	    if (toBlock != 0) {
   	        out << (out.str().empty() ? "" : ",");
   	        out << "\"toBlock\":\"" << uint_2_Hex(toBlock) << "\"";
   	    }

   	    if (emitters.size()) {
   	        out << (out.str().empty() ? "" : ",");
   	        out << "\"address\":[";
   	        size_t cnt = 0;
   	        for (auto addr : emitters) {
   	            if (cnt)
   	                out << ",";
   	            out << "\"" << addr << "\"";
   	            cnt++;
   	        }
   	        out << "]";
   	    }

   	    if (topics.size()) {
   	        out << (out.str().empty() ? "" : ",");
   	        out << "\"topics\":[";
   	        size_t cnt = 0;
   	        for (auto topic : topics) {
   	            if (cnt)
   	                out << ",";
   	            out << "\"" << topic << "\"";
   	            cnt++;
   	        }
   	        out << "]";
   	    }

   	    string_q ret = "[{" + out.str() + "}]";
   	    LOG4("Querying with ", ret);
   	    return ret;

   	    // A valid query for two addresses
   	    //
   	    // curl -X POST --data '{
   	    //    "id":8,
   	    //    "jsonrpc":"2.0",
   	    //    "method":"eth_getLogs",
   	    //    "params":[{
   	    //        "fromBlock":"0x3d37e0",
   	    //        "toBlock":"0x3d37e1",
   	    //        "address":["0x209c4784ab1e8183cf58ca33cb740efbf3fc18ef","0x8d12a197cb00d4747a1fe03395095ce2a5cc6819"]
   	    //        "topics":
   	    //        ["0x6effdda786735d5033bfad5f53e5131abcced9e52be6c507b62d639685fbed6d","0x6effdda786735d5033bfad5f53e5131abcced9e52be6c507b62d639685fbed6d"]
   	    //     }]
   	    //   }' -H "Content-Type: application/json" http://localhost:8545 | jq
   	    // If blockHash is present in in the filter criteria, then neither fromBlock nor toBlock are allowed.
   	}
*/
