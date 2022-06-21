// The manifest package stores information about the IPFS hashes associated with each portion of the Unchained Index (we
// call those portions Chunks). The manifest provides enough information for anyone who receives a copy to reproduce
// the entire index.
package manifest

import (
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// A data structure consisting of a list of chunk records (i.e. block ranges, Bloom filter hashes, and
// chunk hashes) and the nessacary additional information needed to reproduce the index from scratch
// using only the node as source of truth.
type Manifest struct {
	// The specification version used to build this instance of Unchained Index
	Version string `json:"version"`

	// The chain against which this index was created
	Chain string `json:"chain"`

	// An IPFS hash pointing to documentation describing the binary format of the files in the index
	Schemas types.IpfsHash `json:"schemas"`

	// An IPFS hash pointing a file (for example, tar.gz) containing additional databases such as names and timestamps
	Databases types.IpfsHash `json:"databases"`

	// A list of pinned chunks (see ChunkRecord) detailing the location of all chunks in the index and associated bloom filters
	Chunks []ChunkRecord `json:"chunks"`
}

// A single record in the Manifest's Chunks table. It associates a block range, an IPFS hash of the chunk
// covering that block range, and a hash of the Bloom filter covering that chunk. The format of the chunk and the Bloom
// filter are detailed in the manifest's Schema record.
type ChunkRecord struct {
	Range     string         `json:"range"`
	BloomHash types.IpfsHash `json:"bloomHash"`
	IndexHash types.IpfsHash `json:"indexHash"`
}

// TODO: If we included fileSize (pre-zip) here, we could do a check that the file is complete

type Source uint

const (
	FromCache Source = iota
	FromContract
)

// ReadManifest reads the manifest from either the local cache or the Unchained Index smart contract
func ReadManifest(chain string, source Source) (*Manifest, error) {
	if source == FromContract {
		return fromRemote(chain)
	}

	manifestPath := config.GetPathToChainConfig(chain) + "manifest.txt"
	file, err := os.Open(manifestPath)
	if err != nil {
		return nil, err
	}
	return readTabManifest(chain, file)
}
