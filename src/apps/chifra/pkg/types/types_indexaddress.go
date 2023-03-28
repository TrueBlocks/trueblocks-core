package types

import "io"

// EXISTING_CODE
// EXISTING_CODE

type RawIndexAddress interface {
	// EXISTING_CODE
	// EXISTING_CODE
}

type SimpleIndexAddress struct {
	Address string           `json:"address"`
	Range   string           `json:"range"`
	Offset  uint32           `json:"offset"`
	Count   uint32           `json:"count"`
	raw     *RawIndexAddress `json:"-"`
	// EXISTING_CODE
	// EXISTING_CODE
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
	model = map[string]interface{}{
		"address": s.Address,
		"range":   s.Range,
		"offset":  s.Offset,
		"count":   s.Count,
	}
	order = []string{
		"address",
		"range",
		"offset",
		"count",
	}
	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

func (s *SimpleIndexAddress) WriteTo(w io.Writer) (n int64, err error) {
	// EXISTING_CODE
	// EXISTING_CODE
	return 0, nil
}

func (s *SimpleIndexAddress) ReadFrom(r io.Reader) (n int64, err error) {
	// EXISTING_CODE
	// EXISTING_CODE
	return 0, nil
}

// EXISTING_CODE
// EXISTING_CODE
