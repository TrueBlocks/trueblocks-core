package types

import "io"

// EXISTING_CODE
// EXISTING_CODE

type RawIndexAppearance interface {
	// EXISTING_CODE
	// EXISTING_CODE
}

type SimpleIndexAppearance struct {
	BlockNumber      uint32              `json:"blockNumber"`
	TransactionIndex uint32              `json:"transactionIndex"`
	raw              *RawIndexAppearance `json:"-"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s *SimpleIndexAppearance) Raw() *RawIndexAppearance {
	return s.raw
}

func (s *SimpleIndexAppearance) SetRaw(raw *RawIndexAppearance) {
	s.raw = raw
}

func (s *SimpleIndexAppearance) Model(showHidden bool, format string, extraOptions map[string]any) Model {
	var model = map[string]interface{}{}
	var order = []string{}

	// EXISTING_CODE
	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

func (s *SimpleIndexAppearance) WriteTo(w io.Writer) (n int64, err error) {
	// EXISTING_CODE
	// EXISTING_CODE
	return 0, nil
}

func (s *SimpleIndexAppearance) ReadFrom(r io.Reader) (n int64, err error) {
	// EXISTING_CODE
	// EXISTING_CODE
	return 0, nil
}

// EXISTING_CODE
// EXISTING_CODE
