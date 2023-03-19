package types

type RawIndexAddress interface{}

type SimpleIndexAddress struct {
	Address string `json:"address"`
	Range   string `json:"range"`
	Offset  uint32 `json:"offset"`
	Count   uint32 `json:"count"`
}

func (s *SimpleIndexAddress) Write(p []byte) (n int, err error) {
	return 0, nil
}

func (s *SimpleIndexAddress) Read(p []byte) (n int, err error) {
	return 0, nil
}
