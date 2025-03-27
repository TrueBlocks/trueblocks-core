package manifest

import (
	"bytes"
	"encoding/json"
	"errors"
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// LoadManifest reads the manifest from a file or from the Unchained Index smart contract.
//
// It first checks if the manifest file exists. If it does, it reads the manifest from the file.
// If the caller requests the contract or the cached manifest does not exist, it reads the
// manifest from the contract. It then checks if the new manifest has more chunks than the existing
// one. If it does (or if the file didn't exist), it saves the new manifest to the file. Finally, it
// creates a map of chunks for easy lookup and sets the specification if it is not already set.
func LoadManifest(chain string, publisher base.Address, source Source) (man *Manifest, err error) {
	manifestFn := config.PathToManifestFile(chain)
	exists := file.FileExists(manifestFn)
	man = &Manifest{}

	if exists {
		// We will either return this or use it to compare with the downloaded manifest
		// to see if we need to update the file.
		man, err = readManifestFile(manifestFn)
		if err != nil {
			return nil, err
		}
		if man.Chain != chain {
			msg := fmt.Sprintf("The remote manifest's chain (%s) does not match the cached manifest's chain (%s).", man.Chain, chain)
			return man, errors.New(msg)
		}
	}

	if source == TempContract || source == FromContract || !exists {
		database := chain
		cid, err := ReadUnchainedIndex(chain, publisher, database)
		if err != nil {
			return nil, err
		} else if len(cid) == 0 {
			return nil, fmt.Errorf("no record found in the Unchained Index for database %s from publisher %s", database, publisher.Hex())
		}
		gatewayUrl := config.GetChain(chain).IpfsGateway
		logger.InfoTable("Chain:", chain)
		logger.InfoTable("Database:", database)
		logger.InfoTable("Publisher:", publisher)
		logger.InfoTable("Gateway:", gatewayUrl)
		logger.InfoTable("CID:", cid)

		newManifest, err := downloadManifest(chain, gatewayUrl, cid)
		if err != nil {
			return nil, err
		}
		if newManifest.Chain != chain {
			msg := fmt.Sprintf("The remote manifest's chain (%s) does not match the cached manifest's chain (%s).", newManifest.Chain, chain)
			return newManifest, errors.New(msg)
		}
		if source != TempContract {
			err = newManifest.SaveManifest(chain, manifestFn)
			if err != nil {
				return nil, err
			}
		}

		man = newManifest
	}

	man.ChunkMap = make(map[string]*types.ChunkRecord)
	for i := range man.Chunks {
		man.ChunkMap[man.Chunks[i].Range] = &man.Chunks[i]
	}

	return man, nil
}

var specification = "QmUyyU8wKW57c3CuwphhMdZb2QA5bsjt9vVfTE6LcBKmE9"

func Specification() string {
	return specification
}

// readManifestFile reads the manifest from a file.
//
// It first reads the contents of the file into a string. It then decodes the string into the manifest.
func readManifestFile(path string) (*Manifest, error) {
	contents := file.AsciiFileToString(path)
	if len(contents) == 0 {
		return nil, ErrManifestNotFound
	}

	man := &Manifest{}
	reader := bytes.NewReader([]byte(contents))
	if err := json.NewDecoder(reader).Decode(man); err != nil {
		return man, err
	}

	return man, nil
}
