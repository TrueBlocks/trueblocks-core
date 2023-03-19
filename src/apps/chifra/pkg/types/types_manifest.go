package types

import "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"

type RawManifest interface{}

type SimpleManifest struct {
	Version string              `json:"version"`
	Chain   string              `json:"chain"`
	Schemas base.IpfsHash       `json:"schemas"`
	Chunks  []SimpleChunkRecord `json:"chunks"`
}
