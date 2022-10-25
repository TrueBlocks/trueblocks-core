package types

type RawTimestampCount struct {
	Count uint64 `json:"count"`
}

type SimpleTimestampCount struct {
	Count uint64 `json:"count"`
	raw   *RawTimestampCount
}

func (s *SimpleTimestampCount) Raw() *RawTimestampCount {
	r := RawTimestampCount{Count: s.Count}
	return &r
}

func (s *SimpleTimestampCount) SetRaw(r RawTimestampCount) {
	s.raw = &r
}

func (s *SimpleTimestampCount) Model(showHidden bool, format string) Model {
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
