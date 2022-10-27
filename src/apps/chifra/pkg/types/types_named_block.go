package types

type RawNamedBlock interface{}

type SimpleNamedBlock struct {
	BlockNumber uint64 `json:"blockNumber"`
	TimeStamp   uint64 `json:"timestamp"`
	Date        string `json:"date"`
	Name        string `json:"name,omitempty"`
}

func (s *SimpleNamedBlock) Raw() *RawNamedBlock {
	return nil
}

func (s *SimpleNamedBlock) Model(showHidden bool, format string) Model {
	model := map[string]interface{}{
		"blockNumber": s.BlockNumber,
		"timestamp":   s.TimeStamp,
		"date":        s.Date,
	}

	order := []string{
		"blockNumber",
		"timestamp",
		"date",
	}

	// implements omitempty
	if format != "json" || len(s.Name) > 0 {
		model["name"] = s.Name
		order = append(order, "name")
	}

	return Model{
		Data:  model,
		Order: order,
	}
}
