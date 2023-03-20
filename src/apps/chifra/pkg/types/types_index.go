package types

// EXISTING_CODE
import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/paths"
	"github.com/ethereum/go-ethereum/common"
)

// EXISTING_CODE

type RawIndex interface{}

type SimpleIndex struct {
	Range           paths.FileRange `json:"range"`
	Magic           uint32          `json:"magic"`
	Hash            common.Hash     `json:"hash"`
	AddressCount    uint32          `json:"nAddresses"`
	AppearanceCount uint32          `json:"nAppearances"`
	Size            int64           `json:"fileSize"`
	raw             *RawIndex       `json:"-"`
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

func (s *SimpleIndex) Write(p []byte) (n int, err error) {
	// EXISTING_CODE
	// EXISTING_CODE
	return 0, nil
}

func (s *SimpleIndex) Read(p []byte) (n int, err error) {
	// EXISTING_CODE
	// EXISTING_CODE
	return 0, nil
}

// EXISTING_CODE
// EXISTING_CODE
