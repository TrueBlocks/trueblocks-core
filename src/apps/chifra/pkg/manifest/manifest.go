package manifest

import (
	"errors"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// Manifest is a data structure consisting of a list of chunk records (i.e. block ranges, Bloom
// filter hashes, and chunk hashes) and the nessacary additional information needed to
// reproduce the index from scratch using only the node as source of truth.
type Manifest struct {
	// The specification version used to build this instance of Unchained Index
	Version string `json:"version"`

	// The chain against which this index was created
	Chain string `json:"chain"`

	// An IPFS hash pointing to documentation describing the binary format of the files in the index
	Specification base.IpfsHash `json:"specification"`

	// An IPFS hash pointing to documentation describing the binary format of the files in the index
	Config config.ScrapeSettings `json:"config"`

	// A list of pinned chunks (see types.ChunkRecord) detailing the location of all chunks in the index and associated bloom filters
	Chunks []types.ChunkRecord `json:"chunks"`

	// A map to make set membership easier
	ChunkMap map[string]*types.ChunkRecord `json:"-"`
}

type Source int

const (
	None Source = 1 << iota
	LocalCache
	FromContract
	TempContract
)

var ErrManifestNotFound = errors.New("could not find manifest.json or it was empty")
