package types

// EXISTING_CODE
import (
	"io"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
)

// EXISTING_CODE

type RawIndex interface {
	// EXISTING_CODE
	// EXISTING_CODE
}

type SimpleIndex struct {
	Range           base.FileRange `json:"range"`
	Magic           uint32         `json:"magic"`
	Hash            base.Hash      `json:"hash"`
	AddressCount    uint32         `json:"nAddresses"`
	AppearanceCount uint32         `json:"nAppearances"`
	Size            int64          `json:"fileSize"`
	raw             *RawIndex      `json:"-"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s *SimpleIndex) Raw() *RawIndex {
	return s.raw
}

func (s *SimpleIndex) SetRaw(raw *RawIndex) {
	s.raw = raw
}

func (s *SimpleIndex) Model(showHidden bool, format string, extraOptions map[string]any) Model {
	var model = map[string]interface{}{}
	var order = []string{}

	// EXISTING_CODE
	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

func (s *SimpleIndex) WriteTo(w io.Writer) (n int64, err error) {
	// EXISTING_CODE
	// EXISTING_CODE
	return 0, nil
}

func (s *SimpleIndex) ReadFrom(r io.Reader) (n int64, err error) {
	// EXISTING_CODE
	// EXISTING_CODE
	return 0, nil
}

// EXISTING_CODE
// EXISTING_CODE
