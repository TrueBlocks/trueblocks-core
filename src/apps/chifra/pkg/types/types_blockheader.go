package types

type SimpleBlockHeader struct {
	BlockNumber Blknum `json:"blockNumber"`
	Timestamp   int64  `json:"timestamp"`
	raw         *RawBlock
}

func (s *SimpleBlockHeader) Raw() *RawBlock {
	return s.raw
}

func (s *SimpleBlockHeader) SetRaw(raw *RawBlock) {
	s.raw = raw
}

func (s *SimpleBlockHeader) GetTimestamp() uint64 {
	return uint64(s.Timestamp)
}
