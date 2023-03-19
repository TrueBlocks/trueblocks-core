package types

type RawIndexAddressBelongs interface{}

type SimpleIndexAddressBelongs struct {
	Address string                  `json:"address"`
	Offset  uint32                  `json:"offset"`
	Count   uint32                  `json:"count"`
	Apps    []SimpleIndexAppearance `json:"apps"`
}

func (s *SimpleIndexAddressBelongs) Write(p []byte) (n int, err error) {
	return 0, nil
}

func (s *SimpleIndexAddressBelongs) Read(p []byte) (n int, err error) {
	return 0, nil
}
