package normalize

import (
	"errors"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/topics"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func NormalizeLog(log *types.Log) (types.Log, error) {
	var from, to base.Address
	var value *base.Wei
	var data = strings.TrimPrefix(log.Data, "0x")

	if len(log.Topics) == 3 && log.Topics[0] == topics.TransferTopic {
		from = base.HexToAddress(log.Topics[1].Hex())
		to = base.HexToAddress(log.Topics[2].Hex())
		value = base.HexToWei(log.Data)
	} else if len(log.Topics) == 2 {
		if len(data) < 128 {
			return *log, errors.New("data length too short for two values")
		}
		from = base.HexToAddress(log.Topics[1].Hex())
		to = base.HexToAddress("0x" + data[:64])
		value = base.HexToWei(string(data[64:128]))
	} else if len(log.Topics) == 1 || len(log.Topics) == 0 {
		if len(data) < 192 {
			return *log, errors.New("data length too short for three values")
		}
		from = base.HexToAddress("0x" + data[:64])
		to = base.HexToAddress("0x" + data[64:128])
		value = base.HexToWei(data[128:192])
	} else {
		return types.Log{}, errors.New("unrecognized Transfer event format")
	}

	newLog := types.Log{
		Topics: []base.Hash{
			topics.TransferTopic,
			base.HexToHash(from.Hex()),
			base.HexToHash(to.Hex()),
		},
		Data: base.WeiToHash(value),
	}

	return newLog, nil
}
