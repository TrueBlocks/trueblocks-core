package pinning

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// Result carries both the local and remote result of pinning a chunk
type Result struct {
	Local  types.SimpleChunkRecord `json:"local,omitempty"`
	Remote types.SimpleChunkRecord `json:"remote,omitempty"`
}

// Matches returns true if the Result has both local and remote hashes for both the index and the bloom and they match
func (r Result) Matches() bool {
	hasLocal := r.Local.IndexHash != "" && r.Local.BloomHash != ""
	hasRemote := r.Remote.IndexHash != "" && r.Remote.BloomHash != ""
	if hasLocal && hasRemote {
		return r.Local.IndexHash == r.Remote.IndexHash && r.Local.BloomHash == r.Remote.BloomHash
	}
	return true
}

// PinOneFile pins the named database given a path to the local and/or remote pinning
// services if they are available. If the local service is not available, the remote
// service is used.
func PinOneFile(chain, dbName, path string, isLocal, isRemote bool) (base.IpfsHash, error) {
	var hash base.IpfsHash
	var err error

	if !file.FileExists(path) {
		return hash, fmt.Errorf(dbName+" file (%s) does not exist", path)
	}

	if !isLocal && !isRemote {
		return hash, fmt.Errorf("no pinning service available")
	}

	if isLocal {
		localService, _ := NewPinningService(chain, Local)
		if hash, err = localService.pinTheFile(path, true); err != nil {
			return hash, fmt.Errorf("error pinning locally: %s", err)
		}
	}

	if isRemote {
		remoteService, _ := NewPinningService(chain, Pinata)
		var remoteHash base.IpfsHash
		if remoteHash, err = remoteService.pinTheFile(path, false); err != nil {
			return hash, fmt.Errorf("error pinning remotely: %s", err)
		}
		if hash == "" {
			hash = remoteHash
		} else if hash != remoteHash {
			return hash, fmt.Errorf("local (%s) and remote (%s) hashes differ", hash, remoteHash)
		}
	}

	logger.Info(colors.Magenta+"Pinned", dbName, "file", path, "to", hash, colors.Off)
	return hash, nil
}

// PinOneChunk pins the named chunk given a path to the local and/or remote pinning service
func PinOneChunk(chain, bloomFile, indexFile string, isLocal, isRemote bool) (Result, error) {
	rng := base.RangeFromFilename(bloomFile)
	result := Result{
		Local:  types.SimpleChunkRecord{Range: rng.String()},
		Remote: types.SimpleChunkRecord{Range: rng.String()},
	}

	var err error

	if isLocal {
		localService, _ := NewPinningService(chain, Local)
		if result.Local.BloomHash, err = localService.pinTheFile(bloomFile, true); err != nil {
			return Result{}, err
		}
		result.Local.BloomSize = file.FileSize(bloomFile)
		if result.Local.IndexHash, err = localService.pinTheFile(indexFile, true); err != nil {
			return Result{}, err
		}
		result.Local.IndexSize = file.FileSize(indexFile)
		logger.Info(colors.Magenta+"Pinned", rng, "local to ", result.Local.BloomHash, result.Local.IndexHash, colors.Off)
	}

	if isRemote {
		remoteService, _ := NewPinningService(chain, Pinata)
		if result.Remote.BloomHash, err = remoteService.pinTheFile(bloomFile, false); err != nil {
			return Result{}, err
		}
		result.Remote.BloomSize = file.FileSize(bloomFile)
		if result.Remote.IndexHash, err = remoteService.pinTheFile(indexFile, false); err != nil {
			return Result{}, err
		}
		result.Remote.IndexSize = file.FileSize(indexFile)
		logger.Info(colors.Magenta+"Pinned", rng, "remote to", result.Remote.BloomHash, result.Remote.IndexHash, colors.Off)
	}

	return result, nil
}

// pinTheFile pins the file either locally or remotely
func (s *Service) pinTheFile(filepath string, locally bool) (base.IpfsHash, error) {
	if locally {
		return s.pinFileLocally(filepath)
	}
	return s.pinFileRemotely(filepath)
}
