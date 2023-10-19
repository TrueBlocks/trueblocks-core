package manifest

import (
	"bytes"
	"encoding/json"
	"errors"
	"fmt"
	"os"
	"sort"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
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

	// A list of pinned chunks (see types.SimpleChunkRecord) detailing the location of all chunks in the index and associated bloom filters
	Chunks []types.SimpleChunkRecord `json:"chunks"`

	// A map to make set membership easier
	ChunkMap map[string]*types.SimpleChunkRecord `json:"-"`
}

type Source uint

const (
	FromCache Source = iota
	FromContract
)

var ErrManifestNotFound = errors.New("could not find manifest.json or it was empty")

// ReadManifest reads the manifest from either the local cache or the Unchained Index smart contract
func ReadManifest(chain string, publisher base.Address, source Source) (*Manifest, error) {
	if source == FromContract {
		database := chain
		cid, err := ReadUnchainedIndex(chain, publisher, database)
		if err != nil {
			return nil, err
		} else if len(cid) == 0 {
			return nil, fmt.Errorf("no record found in the Unchained Index for database %s from publisher %s", database, publisher.Hex())
		}

		gatewayUrl := config.GetChain(chain).IpfsGateway

		logger.InfoTable("Chain:", chain)
		logger.InfoTable("Gateway:", gatewayUrl)
		logger.InfoTable("Publisher:", publisher)
		logger.InfoTable("Database:", database)
		logger.InfoTable("CID:", cid)

		man, err := downloadManifest(chain, gatewayUrl, cid)
		if man != nil {
			man.LoadChunkMap()
			if man.Specification == "" {
				man.Specification = base.IpfsHash(config.GetUnchained().Specification)
			}
		}

		return man, err
	}

	manifestPath := config.PathToManifest(chain)
	if !file.FileExists(manifestPath) {
		// basically EstablishManifest
		if publisher.IsZero() {
			publisher = base.HexToAddress(config.GetUnchained().PreferredPublisher)
		}
		man, err := ReadManifest(chain, publisher, FromContract)
		if err != nil {
			return nil, err
		}
		return man, man.SaveManifest(chain, config.PathToManifest(chain))
	}

	contents := file.AsciiFileToString(manifestPath)
	if len(contents) == 0 {
		return nil, ErrManifestNotFound
	}

	man := &Manifest{}
	reader := bytes.NewReader([]byte(contents))
	if err := json.NewDecoder(reader).Decode(man); err != nil {
		return man, err
	}

	man.LoadChunkMap()
	if man.Specification == "" {
		man.Specification = base.IpfsHash(config.GetUnchained().Specification)
	}

	return man, nil
}

func (m *Manifest) LoadChunkMap() {
	m.ChunkMap = make(map[string]*types.SimpleChunkRecord)
	for i := range m.Chunks {
		m.ChunkMap[m.Chunks[i].Range] = &m.Chunks[i]
	}
}

// TODO: Protect against overwriting files on disc

func UpdateManifest(chain string, publisher base.Address, chunk types.SimpleChunkRecord) error {
	empty := Manifest{
		Version:       config.GetUnchained().SpecVersion,
		Chain:         chain,
		Specification: base.IpfsHash(config.GetUnchained().Specification),
		Chunks:        []types.SimpleChunkRecord{},
		Config:        config.GetScrape(chain),
		ChunkMap:      make(map[string]*types.SimpleChunkRecord),
	}

	var man *Manifest
	if !file.FileExists(config.PathToManifest(chain)) {
		man = &empty
	} else {
		var err error
		man, err = ReadManifest(chain, publisher, FromCache)
		if err != nil {
			if err != ErrManifestNotFound {
				return err
			}
			man = &empty
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

	logger.Info(colors.Magenta+"Updating manifest with", len(man.Chunks), "chunks", spaces, colors.Off)
	return man.SaveManifest(chain, config.PathToManifest(chain))
}

// SaveManifest writes the manifest to disc in JSON at fileName
func (m *Manifest) SaveManifest(chain, fileName string) error {
	m.Config = config.GetScrape(chain)
	m.Config.ChannelCount = 0 // we don't want this in the manifest

	w, err := os.OpenFile(fileName, os.O_RDWR|os.O_CREATE|os.O_TRUNC, 0644)
	if err != nil {
		return fmt.Errorf("creating file: %s", err)
	}
	defer w.Close()

	// Protect against overwriting files on disc
	err = file.Lock(w)
	if err != nil {
		return fmt.Errorf("locking file: %s", err)
	}
	defer func() {
		_ = file.Unlock(w)
	}()

	if outputBytes, err := json.MarshalIndent(m, "", "  "); err != nil {
		return err
	} else {
		_, err = w.Write(outputBytes)
		return err
	}
}

// TODO: There's got to be a better way - this should use StreamMany
var spaces = strings.Repeat(" ", 40)
