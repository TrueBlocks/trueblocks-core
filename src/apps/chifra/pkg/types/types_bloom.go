package types

// EXISTING_CODE
import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
)

// EXISTING_CODE

type RawBloom interface{}

type SimpleBloom struct {
	Range     base.FileRange `json:"range"`
	Magic     uint16         `json:"magic"`
	Hash      base.Hash      `json:"hash"`
	Count     uint32         `json:"nBlooms"`
	NInserted uint64         `json:"nInserted"`
	Size      int64          `json:"size"`
	Width     uint64         `json:"byteWidth"`
	raw       *RawBloom      `json:"-"`
}

func (s *SimpleBloom) Raw() *RawBloom {
	return s.raw
}

func (s *SimpleBloom) SetRaw(raw *RawBloom) {
	s.raw = raw
}

func (s *SimpleBloom) Model(showHidden bool, format string, extraOptions map[string]any) Model {
	var model = map[string]interface{}{}
	var order = []string{}

	// EXISTING_CODE
	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

func (s *SimpleBloom) Write(p []byte) (n int, err error) {
	// EXISTING_CODE
	// EXISTING_CODE
	return 0, nil
}

func (s *SimpleBloom) Read(p []byte) (n int, err error) {
	// EXISTING_CODE
	// EXISTING_CODE
	return 0, nil
}

// EXISTING_CODE
// EXISTING_CODE