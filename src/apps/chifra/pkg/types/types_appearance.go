package types

type RawAppearance struct {
	Address          string `json:"address"`
	BlockNumber      uint32 `json:"blockNumber"`
	TransactionIndex uint32 `json:"transactionIndex"`
}

type SimpleAppearance struct {
	Address          Address `json:"address"`
	BlockNumber      uint32  `json:"blockNumber"`
	TransactionIndex uint32  `json:"transactionIndex"`
	Reason           string  `json:"reason,omitempty"`
	Timestamp        int64   `json:"timestamp"`
	Date             string  `json:"date"`
	raw              *RawAppearance
}

func (s *SimpleAppearance) Raw() *RawAppearance {
	return s.raw
}

func (s *SimpleAppearance) SetRaw(raw *RawAppearance) {
	s.raw = raw
}

func (s *SimpleAppearance) Model(showHidden bool, format string, extraOptions map[string]any) Model {
	model := map[string]interface{}{
		"address":          s.Address,
		"blockNumber":      s.BlockNumber,
		"transactionIndex": s.TransactionIndex,
		"reason":           s.Reason,
		"timestamp":        s.Timestamp,
		"date":             s.Date,
	}

	order := []string{
		"address",
		"blockNumber",
		"transactionIndex",
		"reason",
		"timestamp",
		"date",
	}

	return Model{
		Data:  model,
		Order: order,
	}
}
