package types

// EXISTING_CODE
import (
	"io"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
)

// EXISTING_CODE

type RawChunkRecord interface {
	// EXISTING_CODE
	// EXISTING_CODE
}

type SimpleChunkRecord struct {
	Range     string          `json:"range,omitempty"`
	BloomHash base.IpfsHash   `json:"bloomHash,omitempty"`
	BloomSize int64           `json:"bloomSize,omitempty"`
	IndexHash base.IpfsHash   `json:"indexHash,omitempty"`
	IndexSize int64           `json:"indexSize,omitempty"`
	raw       *RawChunkRecord `json:"-"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s *SimpleChunkRecord) Raw() *RawChunkRecord {
	return s.raw
}

func (s *SimpleChunkRecord) SetRaw(raw *RawChunkRecord) {
	s.raw = raw
}

func (s *SimpleChunkRecord) Model(showHidden bool, format string, extraOptions map[string]any) Model {
	var model = map[string]interface{}{}
	var order = []string{}

	// EXISTING_CODE
	model = map[string]any{
		"range":     s.Range,
		"bloomHash": s.BloomHash,
		"bloomSize": s.BloomSize,
		"indexHash": s.IndexHash,
		"indexSize": s.IndexSize,
	}
	order = []string{
		"range",
		"bloomHash",
		"bloomSize",
		"indexHash",
		"indexSize",
	}
	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

func (s *SimpleChunkRecord) WriteTo(w io.Writer) (n int64, err error) {
	// EXISTING_CODE
	// EXISTING_CODE
	return 0, nil
}

func (s *SimpleChunkRecord) ReadFrom(r io.Reader) (n int64, err error) {
	// EXISTING_CODE
	// EXISTING_CODE
	return 0, nil
}

// EXISTING_CODE
// EXISTING_CODE
