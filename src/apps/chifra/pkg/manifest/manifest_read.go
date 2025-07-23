package manifest

import (
	"bytes"
	"encoding/json"
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/usage"
)

// LoadManifest reads the manifest from a file or from the Unchained Index smart contract or local cache.
func LoadManifest(chain string, publisher base.Address, source Source) (ret *Manifest, err error) {
	// start with an empty manifest
	ret = &Manifest{}
	defer func() {
		// always update the chunk map on return
		if ret != nil {
			ret.ChunkMap = make(map[string]*types.ChunkRecord)
			if ret.Chunks != nil {
				for i := range ret.Chunks {
					ret.ChunkMap[ret.Chunks[i].Range] = &ret.Chunks[i]
				}
			}
		}
	}()

	manifestFn := config.PathToManifestFile(chain)
	exists := file.FileExists(manifestFn)
	if exists {
		if ret, err = readManifestFile(manifestFn); err != nil {
			return nil, err
		} else if ret.Chain != chain {
			return ret, fmt.Errorf("the remote manifest's chain (%s) does not match the cached manifest's chain (%s)", ret.Chain, chain)
			// } else {
			// ret is now referring to the existing manifest
		}
	}

	if source == LocalCache && exists {
		// The caller only asked for the local cache, if it exists, return it
		return ret, nil
	}

	// We need to download the manifest from the smart contract
	database := chain
	cid, err := ReadUnchainedIndex(chain, publisher, database)
	if err != nil {
		return nil, err
	} else if len(cid) == 0 {
		return nil, fmt.Errorf("no record found in the Unchained Index for database %s from publisher %s", database, publisher.Hex())
	}
	gatewayURL := config.GetChain(chain).IpfsGateway
	logger.InfoTable("Chain:", chain)
	logger.InfoTable("Database:", database)
	logger.InfoTable("Publisher:", publisher)
	logger.InfoTable("Gateway:", gatewayURL)
	logger.InfoTable("CID:", cid)

	remoteManifest, err := downloadManifest(chain, gatewayURL, cid)
	if err != nil {
		return nil, err
	}
	if remoteManifest.Chain != chain {
		return remoteManifest, fmt.Errorf("the remote manifest's chain (%s) does not match the cached manifest's chain (%s)", ret.Chain, chain)
	}

	if source == TempContract {
		// Caller does not want to save the manifest, so return without saving
		ret = remoteManifest
		return ret, nil
	}

	// If, for some reason, the downloaded manifest is older than the cached one...
	if ret.Version > remoteManifest.Version {
		// ...ask the user if they want to use the older manifest...
		if usage.QueryUserNo("The remote manifest is older. Use it anyway? (y/N)?", "Using older manifest...") {
			ret = remoteManifest
			if err = ret.SaveManifest(chain, manifestFn); err != nil {
				return ret, err
			}
			// } else {
			// We use the existing manifest without saving it
		}
	} else {
		// The downloaded manifest is new or the same as the cached one. Use it and save it
		ret = remoteManifest
		if err = ret.SaveManifest(chain, manifestFn); err != nil {
			return ret, err
		}
	}

	return ret, nil
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
