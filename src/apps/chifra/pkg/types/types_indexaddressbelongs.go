package types

import "io"

// EXISTING_CODE
// EXISTING_CODE

type RawIndexAddressBelongs interface {
	// EXISTING_CODE
	// EXISTING_CODE
}

type SimpleIndexAddressBelongs struct {
	Address string                  `json:"address"`
	Offset  uint32                  `json:"offset"`
	Count   uint32                  `json:"count"`
	Apps    []SimpleIndexAppearance `json:"apps"`
	raw     *RawIndexAddressBelongs `json:"-"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s *SimpleIndexAddressBelongs) Raw() *RawIndexAddressBelongs {
	return s.raw
}

func (s *SimpleIndexAddressBelongs) SetRaw(raw *RawIndexAddressBelongs) {
	s.raw = raw
}

func (s *SimpleIndexAddressBelongs) Model(showHidden bool, format string, extraOptions map[string]any) Model {
	var model = map[string]interface{}{}
	var order = []string{}

	// EXISTING_CODE
	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

func (s *SimpleIndexAddressBelongs) WriteTo(w io.Writer) (n int64, err error) {
	// EXISTING_CODE
	// EXISTING_CODE
	return 0, nil
}

func (s *SimpleIndexAddressBelongs) ReadFrom(r io.Reader) (n int64, err error) {
	// EXISTING_CODE
	// EXISTING_CODE
	return 0, nil
}

// EXISTING_CODE
// EXISTING_CODE
