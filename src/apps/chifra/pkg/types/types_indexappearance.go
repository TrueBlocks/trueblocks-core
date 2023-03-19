package types

type RawIndexAppearance interface{}

type SimpleIndexAppearance struct {
	BlockNumber      uint32 `json:"blockNumber"`
	TransactionIndex uint32 `json:"transactionIndex"`
}

func (s *SimpleIndexAppearance) Write(p []byte) (n int, err error) {
	return 0, nil
}

func (s *SimpleIndexAppearance) Read(p []byte) (n int, err error) {
	return 0, nil
}
