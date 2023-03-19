package types

import "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"

type RawTimestamp interface{}

type SimpleTimestamp struct {
	BlockNumber uint64         `json:"blockNumber"`
	Timestamp   base.Timestamp `json:"timestamp"`
	Diff        base.Timestamp `json:"diff"`
}

func (s *SimpleTimestamp) Write(p []byte) (n int, err error) {
	return 0, nil
}

func (s *SimpleTimestamp) Read(p []byte) (n int, err error) {
	return 0, nil
}
