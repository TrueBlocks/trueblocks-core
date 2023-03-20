package types

// EXISTING_CODE
import "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"

// EXISTING_CODE

type RawChunkRecord interface{}

type SimpleChunkRecord struct {
	Range     string          `json:"range,omitempty"`
	BloomHash base.IpfsHash   `json:"bloomHash,omitempty"`
	BloomSize int64           `json:"bloomSize,omitempty"`
	IndexHash base.IpfsHash   `json:"indexHash,omitempty"`
	IndexSize int64           `json:"indexSize,omitempty"`
	raw       *RawChunkRecord `json:"-"`
}

func (s *SimpleChunkRecord) Raw() *RawChunkRecord {
	return s.raw
}

func (s *SimpleChunkRecord) SetRaw(raw *RawChunkRecord) {
	s.raw = raw
}

func (s *SimpleChunkRecord) Model(showHidden bool, format string, extraOptions map[string]any) Model {
	// EXISTING_CODE
	// EXISTING_CODE
	model := map[string]interface{}{}
	order := []string{}

	// EXISTING_CODE
	// EXISTING_CODE
	return Model{
		Data:  model,
		Order: order,
	}
}

func (s *SimpleChunkRecord) Write(p []byte) (n int, err error) {
	// EXISTING_CODE
	// EXISTING_CODE
	return 0, nil
}

func (s *SimpleChunkRecord) Read(p []byte) (n int, err error) {
	// EXISTING_CODE
	// EXISTING_CODE
	return 0, nil
}

// EXISTING_CODE
// EXISTING_CODE
