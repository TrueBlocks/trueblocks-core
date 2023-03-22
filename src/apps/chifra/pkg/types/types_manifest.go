package types

// EXISTING_CODE
import "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"

// EXISTING_CODE

type RawManifest interface {
	// EXISTING_CODE
	// EXISTING_CODE
}

type SimpleManifest struct {
	Version string              `json:"version"`
	Chain   string              `json:"chain"`
	Schemas base.IpfsHash       `json:"schemas"`
	Chunks  []SimpleChunkRecord `json:"chunks"`
	raw     *RawManifest        `json:"-"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s *SimpleManifest) Raw() *RawManifest {
	return s.raw
}

func (s *SimpleManifest) SetRaw(raw *RawManifest) {
	s.raw = raw
}

func (s *SimpleManifest) Model(showHidden bool, format string, extraOptions map[string]any) Model {
	var model = map[string]interface{}{}
	var order = []string{}

	// EXISTING_CODE
	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

func (s *SimpleManifest) Write(p []byte) (n int, err error) {
	// EXISTING_CODE
	// EXISTING_CODE
	return 0, nil
}

func (s *SimpleManifest) Read(p []byte) (n int, err error) {
	// EXISTING_CODE
	// EXISTING_CODE
	return 0, nil
}

// EXISTING_CODE
// EXISTING_CODE
