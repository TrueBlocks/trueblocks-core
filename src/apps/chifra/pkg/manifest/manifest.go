package manifest

import (
	"bytes"
	"encoding/json"
	"errors"
	"fmt"
	"log"
	"os"
	"path/filepath"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
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
	BloomSize int64          `json:"bloomSize"`
	IndexHash types.IpfsHash `json:"indexHash"`
	IndexSize int64          `json:"indexSize"`
}

func (ch *ChunkRecord) GetFullPath(chain string, cacheType cache.CacheType) string {
	switch cacheType {
	case cache.Index_Bloom:
		return fmt.Sprintf("%s.bloom", filepath.Join(config.GetPathToIndex(chain), "blooms", ch.Range))
	case cache.Index_Final:
		return fmt.Sprintf("%s.bin", filepath.Join(config.GetPathToIndex(chain), "finalized", ch.Range))
	}
	log.Fatal("Should not happen.")
	return ""
}

type Source uint

const (
	FromCache Source = iota
	FromContract
)

var ErrManifestNotFound = errors.New("could not find manifest.json or it was empty")

// ReadManifest reads the manifest from either the local cache or the Unchained Index smart contract
func ReadManifest(chain string, source Source) (*Manifest, error) {
	if source == FromContract {
		return fromRemote(chain)
	}

	manifestPath := filepath.Join(config.GetPathToChainConfig(chain), "manifest.json")
	contents := utils.AsciiFileToString(manifestPath)
	if !file.FileExists(manifestPath) || len(contents) == 0 {
		return nil, ErrManifestNotFound
	}

	reader := bytes.NewReader([]byte(contents))
	manifest := &Manifest{}
	err := json.NewDecoder(reader).Decode(manifest)
	return manifest, err
}

func (m *Manifest) SaveManifest(chain string) error {
	fileName := config.GetPathToChainConfig(chain) + "manifest.json"
	w, err := os.OpenFile(fileName, os.O_RDWR|os.O_CREATE|os.O_TRUNC, 0666)
	if err != nil {
		return fmt.Errorf("creating file: %s", err)
	}
	defer w.Close()
	err = file.Lock(w)
	if err != nil {
		return fmt.Errorf("locking file: %s", err)
	}

	return output.OutputObject(&m, w, "json", false, false, true, nil)
}
