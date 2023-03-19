package types

import "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"

type RawChunkRecord interface{}

type SimpleChunkRecord struct {
	Range     string        `json:"range,omitempty"`
	BloomHash base.IpfsHash `json:"bloomHash,omitempty"`
	BloomSize int64         `json:"bloomSize,omitempty"`
	IndexHash base.IpfsHash `json:"indexHash,omitempty"`
	IndexSize int64         `json:"indexSize,omitempty"`
}

func (s *SimpleChunkRecord) Write(p []byte) (n int, err error) {
	return 0, nil
}

func (s *SimpleChunkRecord) Read(p []byte) (n int, err error) {
	return 0, nil
}
