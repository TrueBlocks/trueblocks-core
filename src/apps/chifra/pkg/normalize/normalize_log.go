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

func NormalizeTransferOrApproval(log *types.Log) (*types.Log, error) {
	if len(log.Topics) == 0 {
		return log, fmt.Errorf("log has no topics: %w", ErrNormalization)
	}
	if log.Topics[0] != topics.TransferTopic && log.Topics[0] != topics.EnsTransferTopic && log.Topics[0] != topics.ApprovalTopic {
		return log, fmt.Errorf("unrecognized transfer type: %w", ErrNormalization)
	}

	var addr1, addr2 base.Address
	var value *base.Wei
	var data = strings.TrimPrefix(log.Data, "0x")

	// Common case: standard log with three topics (indexed addresses) and value in data
	if len(log.Topics) == 3 {
		// We assume the two indexed parameters are the addresses.
		addr1 = base.HexToAddress(log.Topics[1].Hex())
		addr2 = base.HexToAddress(log.Topics[2].Hex())
		value = base.HexToWei(log.Data)

	} else if len(log.Topics) == 2 {
		// Handle alternative format if applicable (likely only for transfers)
		if len(data) < 128 {
			return log, fmt.Errorf("data length too short for two-value format in log %v: %w", log, ErrNormalization)
		}
		addr1 = base.HexToAddress(log.Topics[1].Hex())
		addr2 = base.HexToAddress("0x" + data[:64])
		value = base.HexToWei(data[64:128])

	} else if len(log.Topics) == 1 {
		// Handle a third format if it exists (again, likely only for transfers)
		if len(data) < 192 {
			return log, fmt.Errorf("data length too short for three-value format in log %v: %w", log, ErrNormalization)
		}
		addr1 = base.HexToAddress("0x" + data[:64])
		addr2 = base.HexToAddress("0x" + data[64:128])
		value = base.HexToWei(data[128:192])

	} else {
		return log, fmt.Errorf("unrecognized event log format: %w", ErrNormalization)
	}

	// Build the normalized log. We can reuse the same topics for either event,
	// but note that the first topic will differ depending on whether it's a Transfer or Approval.
	newLog := types.Log{
		Topics: []base.Hash{
			log.Topics[0], // This will be either TransferTopic or ApprovalTopic.
			base.HexToHash(addr1.Hex()),
			base.HexToHash(addr2.Hex()),
		},
		Data: base.WeiToHash(value),
	}

	return &newLog, nil
}
