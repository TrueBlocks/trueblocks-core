package types

import "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"

type RawManifest interface{}

type SimpleManifest struct {
	Version string              `json:"version"`
	Chain   string              `json:"chain"`
	Schemas base.IpfsHash       `json:"schemas"`
	Chunks  []SimpleChunkRecord `json:"chunks"`
}

func (s *SimpleManifest) Write(p []byte) (n int, err error) {
	return 0, nil
}

func (s *SimpleManifest) Read(p []byte) (n int, err error) {
	return 0, nil
}
