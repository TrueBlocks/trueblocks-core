package types

type RawWhenCount interface{}

type SimpleWhenCount struct {
	Count uint64 `json:"count"`
}

func (s *SimpleWhenCount) Raw() *RawWhenCount {
	return nil
}

func (s *SimpleWhenCount) Model(showHidden bool, format string, extraOptions map[string]any) Model {
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
