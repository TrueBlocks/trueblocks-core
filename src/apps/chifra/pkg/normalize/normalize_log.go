package normalize

import (
	"errors"
	"fmt"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/topics"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

var ErrNormalization = errors.New("normalization error")

func NormalizeKnownLogs(log *types.Log) (*types.Log, error) {
	if len(log.Topics) == 0 {
		return log, fmt.Errorf("log has no topics: %w", ErrNormalization)
	}

	if !topics.KnownTopics[log.Topics[0]] {
		return log, fmt.Errorf("unrecognized transfer type: %w", ErrNormalization)
	}

	var from, to base.Address
	var value *base.Wei
	var data = strings.TrimPrefix(log.Data, "0x")

	if len(log.Topics) == 4 {
		// This is an NFT Transfer...
		return log, nil

	} else if len(log.Topics) == 3 {
		// A normal, every day Transfer. Assume the two indexed parameters are the addresses.
		from = base.HexToAddress(log.Topics[1].Hex())
		to = base.HexToAddress(log.Topics[2].Hex())
		value = base.HexToWei(log.Data)

	} else if len(log.Topics) == 2 {
		// Handle a second format -- neither to is not indexed.
		if len(data) < 128 {
			return log, fmt.Errorf("data length too short for two-value format in log %v: %w", log, ErrNormalization)
		}
		from = base.HexToAddress(log.Topics[1].Hex())
		to = base.HexToAddress("0x" + data[:64])
		value = base.HexToWei("0x" + string(data[64:128]))

	} else if len(log.Topics) == 1 {
		// Handle a third format -- neither form or to are indexed.
		if len(data) < 192 {
			return log, fmt.Errorf("data length too short for three-value format in log %v: %w", log, ErrNormalization)
		}
		from = base.HexToAddress("0x" + data[:64])
		to = base.HexToAddress("0x" + data[64:128])
		value = base.HexToWei(data[128:192])

	} else {
		return log, fmt.Errorf("unrecognized event log format: %w", ErrNormalization)
	}

	newLog := *log
	newLog.Topics = []base.Hash{
		log.Topics[0],
		base.HexToHash(from.Hex()),
		base.HexToHash(to.Hex()),
	}
	newLog.Data, _ = base.WeiToHash(value)

	return &newLog, nil
}
