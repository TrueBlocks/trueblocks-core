package pinning

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// PinOneFile pins the named database given a path to the local and/or remote pinning
// services if they are available. If the local service is not available, the remote
// service is used.
func PinOneFile(chain, dbName, fn string, isLocal, isRemote bool) (base.IpfsHash, uint64, error) {
	var hash base.IpfsHash
	var err error

	if !file.FileExists(fn) {
		return hash, 0, fmt.Errorf(dbName+" file (%s) does not exist", fn)
	}

	if !isLocal && !isRemote {
		return hash, 0, fmt.Errorf("no pinning service available")
	}

	logger.Info(colors.Magenta+"Pinning", dbName, "file", fn, "...", colors.Off)
	if isLocal {
		localService, _ := NewPinningService(chain, Local)
		if hash, err = localService.pinTheFile(chain, fn, true); err != nil {
			return hash, 0, fmt.Errorf("error pinning locally: %s", err)
		}
	}

	if isRemote {
		remoteService, _ := NewPinningService(chain, Pinata)
		var remoteHash base.IpfsHash
		if remoteHash, err = remoteService.pinTheFile(chain, fn, false); err != nil {
			return hash, 0, fmt.Errorf("error pinning remotely: %s", err)
		}
		if hash == "" {
			hash = remoteHash
		} else if hash != remoteHash {
			return hash, 0, fmt.Errorf("local (%s) and remote (%s) hashes differ", hash, remoteHash)
		}
	}

	logger.Info(colors.Magenta+"Pinned", dbName, "file", fn, "to", hash, colors.Off)
	return hash, uint64(file.FileSize(fn)), nil
}

// PinOneChunk pins the named chunk given a path to the local and/or remote pinning service
func PinOneChunk(chain, bloomFile, indexFile string, isLocal, isRemote bool) (types.SimpleChunkRecord, types.SimpleChunkRecord, error) {
	rng := base.RangeFromFilename(bloomFile)
	local := types.SimpleChunkRecord{Range: rng.String()}
	remote := types.SimpleChunkRecord{Range: rng.String()}
	var err error

	if isLocal {
		localService, _ := NewPinningService(chain, Local)
		if local.BloomHash, err = localService.pinTheFile(chain, bloomFile, true); err != nil {
			return local, remote, err
		}
		local.BloomSize = file.FileSize(bloomFile)
		if local.IndexHash, err = localService.pinTheFile(chain, indexFile, true); err != nil {
			return local, remote, err
		}
		local.IndexSize = file.FileSize(indexFile)
		logger.Info(colors.Magenta+"Pinned", rng, "local to ", local.BloomHash, local.IndexHash, colors.Off)
	}

	if isRemote {
		remoteService, _ := NewPinningService(chain, Pinata)
		if remote.BloomHash, err = remoteService.pinTheFile(chain, bloomFile, false); err != nil {
			return local, remote, err
		}
		remote.BloomSize = file.FileSize(bloomFile)
		if remote.IndexHash, err = remoteService.pinTheFile(chain, indexFile, false); err != nil {
			return local, remote, err
		}
		remote.IndexSize = file.FileSize(indexFile)
		logger.Info(colors.Magenta+"Pinned", rng, "remote to", remote.BloomHash, remote.IndexHash, colors.Off)
	}

	return local, remote, err
}

// Matches returns true if the Result has both local and remote hashes for both the index and the bloom and they match
func Matches(local, remote *types.SimpleChunkRecord) bool {
	hasLocal := local.IndexHash != "" && local.BloomHash != ""
	hasRemote := remote.IndexHash != "" && remote.BloomHash != ""
	if hasLocal && hasRemote {
		return local.IndexHash == remote.IndexHash && local.BloomHash == remote.BloomHash
	}
	return true
}

// pinTheFile pins the file either locally or remotely
func (s *Service) pinTheFile(chain, filepath string, locally bool) (base.IpfsHash, error) {
	if locally {
		return s.pinFileLocally(chain, filepath)
	}
	return s.pinFileRemotely(chain, filepath)
}
