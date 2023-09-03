package rpc

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
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
			if err = conn.Store.Write(logGroup, nil); err != nil {
				logger.Warn("Failed to write logs to cache", err)
			}
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
			txHash := base.HexToHash(rawLog.TransactionHash)
			simpleLog, _ := rawLog.RawToSimple(map[string]any{
				"hash":      txHash,
				"timestamp": curTs,
			})
			ret = append(ret, simpleLog)
		}
		return ret, nil
	}
}
