package manifest

import (
	"bytes"
	"encoding/json"
	"errors"
	"fmt"
	"html/template"
	"io"
	"os"
	"path/filepath"
	"reflect"
	"sort"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config/scrapeCfg"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
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
	Schemas base.IpfsHash `json:"schemas"`

	// An IPFS hash pointing to documentation describing the binary format of the files in the index
	Config scrapeCfg.ScrapeSettings `json:"config"`

	// A list of pinned chunks (see ChunkRecord) detailing the location of all chunks in the index and associated bloom filters
	Chunks []ChunkRecord `json:"chunks"`

	// A map to make set membership easier
	ChunkMap map[string]*ChunkRecord `json:"-"`
}

// A single record in the Manifest's Chunks table. It associates a block range, an IPFS hash of the chunk
// covering that block range, and a hash of the Bloom filter covering that chunk. The format of the chunk and the Bloom
// filter are detailed in the manifest's Schema record.
type ChunkRecord struct {
	Range     string        `json:"range"`
	BloomHash base.IpfsHash `json:"bloomHash"`
	BloomSize int64         `json:"bloomSize"`
	IndexHash base.IpfsHash `json:"indexHash,omitempty"`
	IndexSize int64         `json:"indexSize,omitempty"`
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
		man, err := fromRemote(chain)
		if man != nil {
			man.LoadChunkMap()
		}
		return man, err
	}

	manifestPath := filepath.Join(config.GetPathToChainConfig(chain), "manifest.json")
	contents := file.AsciiFileToString(manifestPath)
	if !file.FileExists(manifestPath) || len(contents) == 0 {
		return nil, ErrManifestNotFound
	}

	man := &Manifest{}
	reader := bytes.NewReader([]byte(contents))
	if err := json.NewDecoder(reader).Decode(man); err != nil {
		return man, err
	}

	man.LoadChunkMap()
	return man, nil
}

func (m *Manifest) LoadChunkMap() {
	m.ChunkMap = make(map[string]*ChunkRecord)
	for i := range m.Chunks {
		m.ChunkMap[m.Chunks[i].Range] = &m.Chunks[i]
	}
}

// TODO: Protect against overwriting files on disc
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

	return OutputManifest(&m, w, "json", false, true, nil)
}

// TODO: Protect against overwriting files on disc
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
	if ok {
		// logger.Info("Replacing chunk at", chunk.Range)
		*man.ChunkMap[chunk.Range] = chunk
	} else {
		// Create somewhere to put it if it's not already there
		// logger.Info("Adding chunk at", chunk.Range)
		man.Chunks = append(man.Chunks, chunk)
		man.ChunkMap[chunk.Range] = &chunk
		sort.Slice(man.Chunks, func(i, j int) bool {
			return man.Chunks[i].Range < man.Chunks[j].Range
		})
	}

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

	logger.Info("Updating manifest with", len(man.Chunks), "chunks", spaces)
	return OutputManifest(man, w, "json", false, true, nil)
}

// TODO: There's got to be a better way - this should use StreamMany
var spaces = strings.Repeat(" ", 40)

func OutputManifest(data interface{}, w io.Writer, format string, hideHeader, first bool, meta *rpcClient.MetaData) error {
	var outputBytes []byte
	var err error

	preceeds := ""
	switch format {
	case "json":
		outputBytes, err = json.MarshalIndent(data, "", "  ")
		if err != nil {
			return err
		}
		if !first {
			preceeds = ","
		}
	default:
		if format == "csv" || format == "txt" || (strings.Contains(format, "\t") || strings.Contains(format, ",")) {
			tt := reflect.TypeOf(data)
			rowTemplate, err := GetTemplate(&tt, format)
			if err != nil {
				return err
			}
			return rowTemplate.Execute(w, data)
		}
		return fmt.Errorf("unsupported format %s", format)
	}
	w.Write([]byte(preceeds))
	w.Write(outputBytes)

	return nil
}

func GetTemplate(t *reflect.Type, format string) (*template.Template, error) {
	fields, sep, quote := utils.GetFields(t, format, false)
	var sb strings.Builder
	for i, field := range fields {
		if i > 0 {
			sb.WriteString(sep)
		}
		sb.WriteString(quote + "{{." + field + "}}" + quote)
	}
	tt, err := template.New("").Parse(sb.String() + "\n")
	return tt, err
}
