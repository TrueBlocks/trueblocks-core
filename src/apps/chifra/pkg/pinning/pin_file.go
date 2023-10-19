package pinning

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

var ErrNoPinningService = fmt.Errorf("no pinning service available")

// PinOneFile pins the named database given a path to the local and/or remote pinning
// services if they are available. If the local service is not available, the remote
// service is used.
func PinOneFile(chain, dbName, fileName string, local, remote bool) (base.IpfsHash, uint64, error) {
	var hash base.IpfsHash
	var err error

	if !file.FileExists(fileName) {
		return hash, 0, fmt.Errorf(dbName+" file (%s) does not exist", fileName)
	}

	if !local && !remote {
		return hash, 0, ErrNoPinningService
	}

	logger.Info(colors.Magenta+"Pinning", dbName, "file", fileName, "...", colors.Off)
	if local {
		localService, _ := NewService(chain, Local)
		if hash, err = localService.pinFileLocally(chain, fileName); err != nil {
			return hash, 0, fmt.Errorf("error pinning locally: %s", err)
		}
	}

	if remote {
		remoteService, _ := NewService(chain, Pinata)
		var remoteHash base.IpfsHash
		if remoteHash, err = remoteService.pinFileRemotely(chain, fileName); err != nil {
			return hash, 0, fmt.Errorf("error pinning remotely: %s", err)
		}
		if hash == "" {
			hash = remoteHash
		} else if hash != remoteHash {
			return hash, 0, fmt.Errorf("local (%s) and remote (%s) hashes differ", hash, remoteHash)
		}
	}

	logger.Info(colors.Magenta+"Pinned", dbName, "file", fileName, "to", hash, colors.Off)
	return hash, uint64(file.FileSize(fileName)), nil
}

// PinOneChunk pins the named chunk given a path to the local and/or remote pinning service
func PinOneChunk(chain, bloomFile, indexFile string, local, remote bool) (types.SimpleChunkRecord, types.SimpleChunkRecord, error) {
	rng := base.RangeFromFilename(bloomFile)
	localPin := types.SimpleChunkRecord{Range: rng.String()}
	remotePin := types.SimpleChunkRecord{Range: rng.String()}
	var err error

	if !local && !remote {
		return localPin, remotePin, ErrNoPinningService
	}

	if local {
		localService, _ := NewService(chain, Local)
		if localPin.BloomHash, err = localService.pinFileLocally(chain, bloomFile); err != nil {
			return localPin, remotePin, err
		}
		localPin.BloomSize = file.FileSize(bloomFile)
		if localPin.IndexHash, err = localService.pinFileLocally(chain, indexFile); err != nil {
			return localPin, remotePin, err
		}
		localPin.IndexSize = file.FileSize(indexFile)
		logger.Info(colors.Magenta+"Pinned", rng, "local to ", localPin.BloomHash, localPin.IndexHash, colors.Off)
	}

	if remote {
		remoteService, _ := NewService(chain, Pinata)
		if remotePin.BloomHash, err = remoteService.pinFileRemotely(chain, bloomFile); err != nil {
			return localPin, remotePin, err
		}
		remotePin.BloomSize = file.FileSize(bloomFile)
		if remotePin.IndexHash, err = remoteService.pinFileRemotely(chain, indexFile); err != nil {
			return localPin, remotePin, err
		}
		remotePin.IndexSize = file.FileSize(indexFile)
		logger.Info(colors.Magenta+"Pinned", rng, "remote to", remotePin.BloomHash, remotePin.IndexHash, colors.Off)
	}

	return localPin, remotePin, err
}
