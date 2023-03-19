package types

import "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"

type SimpleTimestamp struct {
	BlockNumber uint64         `json:"blockNumber"`
	Timestamp   base.Timestamp `json:"timestamp"`
	Diff        base.Timestamp `json:"diff"`
}
