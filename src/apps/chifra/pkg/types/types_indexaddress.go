package types

// EXISTING_CODE
// EXISTING_CODE

type RawIndexAddress interface{}

type SimpleIndexAddress struct {
	Address string           `json:"address"`
	Range   string           `json:"range"`
	Offset  uint32           `json:"offset"`
	Count   uint32           `json:"count"`
	raw     *RawIndexAddress `json:"-"`
}

func (s *SimpleIndexAddress) Raw() *RawIndexAddress {
	return s.raw
}

func (s *SimpleIndexAddress) SetRaw(raw *RawIndexAddress) {
	s.raw = raw
}

func (s *SimpleIndexAddress) Model(showHidden bool, format string, extraOptions map[string]any) Model {
	var model = map[string]interface{}{}
	var order = []string{}

	// EXISTING_CODE
	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

func (s *SimpleIndexAddress) Write(p []byte) (n int, err error) {
	// EXISTING_CODE
	// EXISTING_CODE
	return 0, nil
}

func (s *SimpleIndexAddress) Read(p []byte) (n int, err error) {
	// EXISTING_CODE
	// EXISTING_CODE
	return 0, nil
}

// EXISTING_CODE
// EXISTING_CODE
