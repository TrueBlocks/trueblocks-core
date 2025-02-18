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

func NormalizeKnownLogs(log *types.Log) (*types.Log, bool, error) {
	if len(log.Topics) == 0 {
		return log, false, fmt.Errorf("log has no topics: %w", ErrNormalization)
	}

	if !topics.KnownTopics[log.Topics[0]] {
		return log, false, fmt.Errorf("unrecognized transfer type: %w", ErrNormalization)
	}

	var addr1, addr2 base.Address
	var value *base.Wei
	var data = strings.TrimPrefix(log.Data, "0x")

	if len(log.Topics) == 4 {
		// This is an NFT Transfer...
		return log, true, nil

	} else if len(log.Topics) == 3 {
		// A normal, every day Transfer. Assume the two indexed parameters are the addresses.
		addr1 = base.HexToAddress(log.Topics[1].Hex())
		addr2 = base.HexToAddress(log.Topics[2].Hex())
		value = base.HexToWei(log.Data)

	} else if len(log.Topics) == 2 {
		// A non-standard format. Likely only for transfers.
		if len(data) < 128 {
			return log, false, fmt.Errorf("data length too short for two-value format in log %v: %w", log, ErrNormalization)
		}
		addr1 = base.HexToAddress(log.Topics[1].Hex())
		addr2 = base.HexToAddress("0x" + data[:64])
		value = base.HexToWei("0x" + string(data[64:128]))

	} else if len(log.Topics) == 1 {
		// Handle a third format if it exists (again, likely only for transfers)
		if len(data) < 192 {
			return log, false, fmt.Errorf("data length too short for three-value format in log %v: %w", log, ErrNormalization)
		}
		addr1 = base.HexToAddress("0x" + data[:64])
		addr2 = base.HexToAddress("0x" + data[64:128])
		value = base.HexToWei(data[128:192])

	} else {
		return log, false, fmt.Errorf("unrecognized event log format: %w", ErrNormalization)
	}

	newLog := *log
	newLog.Topics = []base.Hash{
		log.Topics[0],
		base.HexToHash(addr1.Hex()),
		base.HexToHash(addr2.Hex()),
	}
	newLog.Data, _ = base.WeiToHash(value)

	return &newLog, false, nil
}
