package types

type RawNamedBlock struct {
	BlockNumber uint64 `json:"blockNumber"`
	TimeStamp   uint64 `json:"timestamp"`
	Date        string `json:"date"`
	Name        string `json:"name,omitempty"`
}

type SimpleNamedBlock struct {
	BlockNumber uint64 `json:"blockNumber"`
	TimeStamp   uint64 `json:"timestamp"`
	Date        string `json:"date"`
	Name        string `json:"name,omitempty"`
	raw         *RawNamedBlock
}

func (s *SimpleNamedBlock) Raw() *RawNamedBlock {
	r := RawNamedBlock{
		BlockNumber: s.BlockNumber,
		TimeStamp:   s.TimeStamp,
		Date:        s.Date,
		Name:        s.Name,
	}
	return &r
}

func (s *SimpleNamedBlock) SetRaw(r RawNamedBlock) {
	s.raw = &r
}

func (s *SimpleNamedBlock) Model(showHidden bool, format string) Model {
	model := map[string]interface{}{
		"blockNumber": s.BlockNumber,
		"timestamp":   s.TimeStamp,
		"date":        s.Date,
		"name":        s.Name,
	}

	order := []string{
		"blockNumber",
		"timestamp",
		"date",
		"name",
	}

	return Model{
		Data:  model,
		Order: order,
	}
}
