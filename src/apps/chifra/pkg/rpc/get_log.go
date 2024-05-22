package rpc

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc/query"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// GetLogsByNumber returns the logs of a block
func (conn *Connection) GetLogsByNumber(bn base.Blknum, ts base.Timestamp) ([]types.Log, error) {
	if conn.StoreReadable() {
		logGroup := &types.LogGroup{
			BlockNumber:      bn,
			TransactionIndex: base.NOPOSN,
		}
		if err := conn.Store.Read(logGroup, nil); err == nil {
			return logGroup.Logs, nil
		}
	}

	filter := LogFilter{
		FromBlock: bn,
		ToBlock:   bn,
	}

	if logs, err := conn.getLogs(filter); err != nil {
		return logs, err
	} else {
		if conn.StoreWritable() && conn.EnabledMap["logs"] && base.IsFinal(conn.LatestBlockTimestamp, ts) {
			logGroup := &types.LogGroup{
				Logs:             logs,
				BlockNumber:      bn,
				TransactionIndex: base.NOPOSN,
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

func (conn *Connection) getLogs(filter LogFilter) ([]types.Log, error) {
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

	if rawLogs, err := query.Query[[]types.Log](conn.Chain, method, params); err != nil {
		return []types.Log{}, err

	} else if rawLogs == nil || len(*rawLogs) == 0 {
		return []types.Log{}, nil

	} else {
		curBlock := base.NOPOSN
		curTs := base.NOPOSI
		var ret []types.Log
		for _, rawLog := range *rawLogs {
			bn := rawLog.BlockNumber
			if bn != curBlock {
				curTs = conn.GetBlockTimestamp(bn)
				curBlock = bn
			}
			rawLog.BlockNumber = curBlock
			rawLog.Timestamp = curTs
			ret = append(ret, rawLog)
		}
		return ret, nil
	}
}

type LogFilter struct {
	BlockHash base.Hash      `json:"blockHash"`
	Emitters  []base.Address `json:"emitters"`
	FromBlock base.Blknum    `json:"fromBlock"`
	ToBlock   base.Blknum    `json:"toBlock"`
	Topics    []base.Hash    `json:"topics"`
}

func NewLogFilter(emitters []string, topics []string) *LogFilter {
	logFilter := &LogFilter{}
	for _, e := range emitters {
		logFilter.Emitters = append(logFilter.Emitters, base.HexToAddress(e))
	}
	for _, t := range topics {
		logFilter.Topics = append(logFilter.Topics, base.HexToHash(t))
	}
	return logFilter
}

func (filter *LogFilter) PassesFilter(log *types.Log) bool {
	foundEmitter := false
	for _, e := range filter.Emitters {
		if e == log.Address {
			foundEmitter = true
			break
		}
	}

	topicsFound := 0
	for _, t := range filter.Topics {
		for _, lt := range log.Topics {
			if t == lt {
				topicsFound++
			}
		}
	}

	passesEmitter := len(filter.Emitters) == 0 || foundEmitter
	passesTopic := len(filter.Topics) == 0 || topicsFound > 0

	return passesEmitter && passesTopic
}
