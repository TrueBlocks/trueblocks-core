package pinning

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

type PinResult struct {
	Range   base.FileRange          `json:"range,omitempty"`
	Local   types.SimpleChunkRecord `json:"local,omitempty"`
	Remote  types.SimpleChunkRecord `json:"remote,omitempty"`
	Matches bool                    `json:"matches,omitempty"`
	err     error
}

// var errNoPinningService = errors.New("no pinning service available")

// PinItem pins the named database given a path to the local and/or remote pinning
// services if they are available. If the local service is not available, the remote
// service is used.
func PinItem(chain string, dbName, path string, isRemote bool) (hash base.IpfsHash, err error) {
	isLocal := LocalDaemonRunning()
	if !isLocal && !isRemote {
		err = fmt.Errorf("no pinning service available")
		return
	}

	if !file.FileExists(path) {
		err = fmt.Errorf(dbName+" file (%s) does not exist", path)
		return
	}

	if isLocal {
		localService, _ := NewPinningService(chain, Local)
		if hash, err = localService.PinFile(path, true); err != nil {
			err = fmt.Errorf("error pinning locally: %s", err)
			return
		}
	}

	if isRemote {
		remoteService, _ := NewPinningService(chain, Pinata)
		var remoteHash base.IpfsHash
		if remoteHash, err = remoteService.PinFile(path, false); err != nil {
			err = fmt.Errorf("error pinning remotely: %s", err)
			return
		}
		if hash == "" {
			hash = remoteHash
		} else if hash != remoteHash {
			err = fmt.Errorf("local (%s) and remote (%s) hashes differ", hash, remoteHash)
			return
		}
	}

	logger.Info(colors.Magenta+"Pinned", dbName, "file", path, "to", hash, colors.Off)
	return
}

func PinChunk(chain, bloomFile, indexFile string, isRemote bool) (PinResult, error) {
	rng := base.RangeFromFilename(bloomFile)
	result := PinResult{
		Range:  rng,
		Local:  types.SimpleChunkRecord{Range: rng.String()},
		Remote: types.SimpleChunkRecord{Range: rng.String()},
	}

	localService, _ := NewPinningService(chain, Local)
	remoteService, _ := NewPinningService(chain, Pinata)

	isLocal := LocalDaemonRunning()
	if isLocal {
		if result.Local.BloomHash, result.err = localService.PinFile(bloomFile, true); result.err != nil {
			return PinResult{}, result.err
		}
		result.Local.BloomSize = file.FileSize(bloomFile)
		if result.Local.IndexHash, result.err = localService.PinFile(indexFile, true); result.err != nil {
			return PinResult{}, result.err
		}
		result.Local.IndexSize = file.FileSize(indexFile)
		logger.Info(colors.Magenta+"Pinned", rng, "local to ", result.Local.BloomHash, result.Local.IndexHash, colors.Off)
	}

	if isRemote {
		if result.Remote.BloomHash, result.err = remoteService.PinFile(bloomFile, false); result.err != nil {
			return PinResult{}, result.err
		}
		result.Remote.BloomSize = file.FileSize(bloomFile)
		if result.Remote.IndexHash, result.err = remoteService.PinFile(indexFile, false); result.err != nil {
			return PinResult{}, result.err
		}
		result.Remote.IndexSize = file.FileSize(indexFile)
		logger.Info(colors.Magenta+"Pinned", rng, "remote to", result.Remote.BloomHash, result.Remote.IndexHash, colors.Off)
	}

	// TODO: We used to use this to report an error between local and remote pinning, but it got turned off. Turn it back on
	if isLocal && isRemote {
		result.Matches = result.Local.IndexHash == result.Remote.IndexHash && result.Local.BloomHash == result.Remote.BloomHash
	} else {
		result.Matches = true
	}

	return result, nil
}

func (s *Service) PinFile(filepath string, local bool) (base.IpfsHash, error) {
	if local {
		return s.pinFileLocally(filepath)
	}
	return s.pinFileRemotely(filepath)
}
