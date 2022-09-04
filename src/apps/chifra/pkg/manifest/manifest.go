package manifest

import (
	"bytes"
	"encoding/json"
	"errors"
	"fmt"
	"os"
	"path/filepath"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/unchained"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/version"
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

	// A list of pinned chunks (see ChunkRecord) detailing the location of all chunks in the index and associated bloom filters
	Chunks []ChunkRecord `json:"chunks"`

	// A map to make set membership easier
	ChunkMap map[string]*ChunkRecord `json:"-"`
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

type ChunkType int

const (
	None ChunkType = iota
	Bloom
	Index
)

func (ch ChunkType) String() string {
	switch ch {
	case Bloom:
		return "bloom"
	case Index:
		return "index"
	default:
		logger.Fatal("Unknown type in ChunkType String")
		return ""
	}
}

func (ch *ChunkRecord) GetFullPath(chain string, cacheType ChunkType) string {
	switch cacheType {
	case Bloom:
		return fmt.Sprintf("%s.bloom", filepath.Join(config.GetPathToIndex(chain), "blooms", ch.Range))
	case Index:
		return fmt.Sprintf("%s.bin", filepath.Join(config.GetPathToIndex(chain), "finalized", ch.Range))
	}
	logger.Fatal("unexpected chunkType in GetFullPath")
	return ""
}

func (ch *ChunkRecord) IsExpectedSize(path string, chunkType ChunkType) bool {
	switch chunkType {
	case Bloom:
		return file.FileExists(path) && file.FileSize(path) == ch.BloomSize
	case Index:
		return file.FileExists(path) && file.FileSize(path) == ch.IndexSize
	}
	logger.Fatal("unexpected chunkType in IsExpectedSize")
	return false
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
	man := &Manifest{}
	if err := json.NewDecoder(reader).Decode(man); err != nil {
		return man, err
	}
	man.ChunkMap = make(map[string]*ChunkRecord)
	for i := range man.Chunks {
		man.ChunkMap[man.Chunks[i].Range] = &man.Chunks[i]
	}

	return man, nil
}

// TODO: BOGUS - Protect against overwriting files
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

// TODO: BOGUS - WORK - Protect against failure while writing
func UpdateManifest(chain string, chunk ChunkRecord) error {
	man, err := ReadManifest(chain, FromCache)
	if err != nil {
		if err != ErrManifestNotFound {
			return err
		}

		// This is okay. Create an empty manifest
		man = &Manifest{
			Version: version.ManifestVersion,
			Chain:   chain,
			Schemas: unchained.Schemas,
			// Databases: unchained.Databases,
			Chunks:   []ChunkRecord{},
			ChunkMap: make(map[string]*ChunkRecord),
		}
	}

	// Make sure this chunk is only added once
	_, ok := man.ChunkMap[chunk.Range]
	if !ok {
		n := &ChunkRecord{}
		man.ChunkMap[chunk.Range] = n
	}
	*man.ChunkMap[chunk.Range] = chunk

	fileName := config.GetPathToChainConfig(chain) + "manifest.json"
	w, err := os.OpenFile(fileName, os.O_RDWR|os.O_CREATE|os.O_TRUNC, 0644)
	if err != nil {
		return fmt.Errorf("creating file: %s", err)
	}
	defer w.Close()

	err = file.Lock(w)
	if err != nil {
		return fmt.Errorf("locking file: %s", err)
	}
	defer file.Unlock(w)

	logger.Log(logger.Info, "Updating manifest with", len(man.Chunks), "chunks", spaces)
	return output.OutputObject(man, w, "json", false, false, true, nil)
}

// TODO: There's got to be a better way
var spaces = strings.Repeat(" ", 40)
