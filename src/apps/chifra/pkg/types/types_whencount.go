package types

// EXISTING_CODE
// EXISTING_CODE

type RawWhenCount interface{}

type SimpleWhenCount struct {
	Count uint64        `json:"count"`
	raw   *RawWhenCount `json:"-"`
}

func (s *SimpleWhenCount) Raw() *RawWhenCount {
	return nil
}

func (s *SimpleWhenCount) SetRaw(raw *RawWhenCount) {
	s.raw = raw
}

func (s *SimpleWhenCount) Model(showHidden bool, format string, extraOptions map[string]any) Model {
	// EXISTING_CODE
	// EXISTING_CODE
	model := map[string]interface{}{
		"count": s.Count,
	}
	order := []string{
		"count",
	}

	// EXISTING_CODE
	// EXISTING_CODE
	return Model{
		Data:  model,
		Order: order,
	}
}

func (s *SimpleWhenCount) Write(p []byte) (n int, err error) {
	// EXISTING_CODE
	// EXISTING_CODE
	return 0, nil
}

func (s *SimpleWhenCount) Read(p []byte) (n int, err error) {
	// EXISTING_CODE
	// EXISTING_CODE
	return 0, nil
}

// EXISTING_CODE
// EXISTING_CODE
