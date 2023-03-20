package types

// EXISTING_CODE
// EXISTING_CODE

type RawIndexAddressBelongs interface{}

type SimpleIndexAddressBelongs struct {
	Address string                  `json:"address"`
	Offset  uint32                  `json:"offset"`
	Count   uint32                  `json:"count"`
	Apps    []SimpleIndexAppearance `json:"apps"`
	raw     *RawIndexAddressBelongs `json:"-"`
}

func (s *SimpleIndexAddressBelongs) Raw() *RawIndexAddressBelongs {
	return s.raw
}

func (s *SimpleIndexAddressBelongs) SetRaw(raw *RawIndexAddressBelongs) {
	s.raw = raw
}

func (s *SimpleIndexAddressBelongs) Model(showHidden bool, format string, extraOptions map[string]any) Model {
	// EXISTING_CODE
	// EXISTING_CODE
	model := map[string]interface{}{}
	order := []string{}

	// EXISTING_CODE
	// EXISTING_CODE
	return Model{
		Data:  model,
		Order: order,
	}
}

func (s *SimpleIndexAddressBelongs) Write(p []byte) (n int, err error) {
	// EXISTING_CODE
	// EXISTING_CODE
	return 0, nil
}

func (s *SimpleIndexAddressBelongs) Read(p []byte) (n int, err error) {
	// EXISTING_CODE
	// EXISTING_CODE
	return 0, nil
}

// EXISTING_CODE
// EXISTING_CODE
