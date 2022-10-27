package types

type RawWhenCount struct {
	Count uint64 `json:"count"`
}

type SimpleWhenCount struct {
	Count uint64 `json:"count"`
	raw   *RawWhenCount
}

func (s *SimpleWhenCount) Raw() *RawWhenCount {
	r := RawWhenCount{Count: s.Count}
	return &r
}

func (s *SimpleWhenCount) SetRaw(r RawWhenCount) {
	s.raw = &r
}

func (s *SimpleWhenCount) Model(showHidden bool, format string) Model {
	model := map[string]interface{}{
		"count": s.Count,
	}
	order := []string{
		"count",
	}
	return Model{
		Data:  model,
		Order: order,
	}
}
