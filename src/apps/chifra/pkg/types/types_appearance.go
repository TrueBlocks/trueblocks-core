package types

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
)

type RawAppearance struct {
	Address          string `json:"address"`
	BlockNumber      uint32 `json:"blockNumber"`
	TransactionIndex uint32 `json:"transactionIndex"`
}

type SimpleAppearance struct {
	Address          base.Address   `json:"address"`
	BlockNumber      uint32         `json:"blockNumber"`
	TransactionIndex uint32         `json:"transactionIndex"`
	Reason           string         `json:"reason,omitempty"`
	Timestamp        base.Timestamp `json:"timestamp"`
	Date             string         `json:"date"`
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

func (s *SimpleAppearance) Write(p []byte) (n int, err error) {
	return 0, nil
}

func (s *SimpleAppearance) Read(p []byte) (n int, err error) {
	return 0, nil
}
