package pinning

import (
	"fmt"
	"path/filepath"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/ranges"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/types"
)

var ErrNoPinningService = fmt.Errorf("no pinning service available")

// PinOneFile pins the named database given a path to the local and/or remote pinning
// services if they are available. If the local service is not available, the remote
// service is used.
func PinOneFile(chain, dbName, fileName string, remote bool) (base.IpfsHash, base.IpfsHash, error) {
	var localHash base.IpfsHash
	var remoteHash base.IpfsHash
	var err error

	if !file.FileExists(fileName) {
		return localHash, remoteHash, fmt.Errorf(dbName+" file (%s) does not exist", fileName)
	}

	local := config.IpfsRunning()
	if !local && !remote {
		return localHash, remoteHash, ErrNoPinningService
	}

	toShow := filepath.Base(fileName)
	if local {
		localService, _ := NewService(chain, Local)
		if localHash, err = localService.pinFileLocally(chain, fileName); err != nil {
			return localHash, remoteHash, fmt.Errorf("error pinning locally: %s %s", fileName, err)
		}
	}

	if remote {
		logger.Progress(true, colors.Magenta+"Pinning", dbName, "file", toShow, "remotely...", colors.Off)
		remoteService, _ := NewService(chain, Pinata)
		if remoteHash, err = remoteService.pinFileRemotely(chain, fileName); err != nil {
			return localHash, remoteHash, fmt.Errorf("error pinning remotely: %s %s", fileName, err)
		}
		if localHash == "" {
			localHash = remoteHash
		}
	}

	logger.Info(colors.Magenta+"["+dbName+"] Pinned", dbName, "file", toShow, "to", localHash, colors.Off)
	return localHash, remoteHash, nil
}

// PinOneChunk pins the named chunk given a path to the local and/or remote pinning service
func PinOneChunk(chain, path string, remote bool, progressInfo string) (types.ChunkRecord, types.ChunkRecord, error) {
	bloomFile := index.ToBloomPath(path)
	indexFile := index.ToIndexPath(path)
	local := config.IpfsRunning()

	rng := ranges.RangeFromFilename(bloomFile)
	localPin := types.ChunkRecord{Range: rng.String()}
	remotePin := types.ChunkRecord{Range: rng.String()}
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
		logger.Info(colors.Magenta+"["+progressInfo+"] Pinned", rng, "local to ", localPin.BloomHash, localPin.IndexHash, colors.Off)
	}

	if remote {
		logger.Progress(true, colors.Magenta+"Pinning file", rng.String(), "remotely...", colors.Off)
		remoteService, _ := NewService(chain, Pinata)
		if remotePin.BloomHash, err = remoteService.pinFileRemotely(chain, bloomFile); err != nil {
			return localPin, remotePin, err
		}
		remotePin.BloomSize = file.FileSize(bloomFile)
		if remotePin.IndexHash, err = remoteService.pinFileRemotely(chain, indexFile); err != nil {
			return localPin, remotePin, err
		}
		remotePin.IndexSize = file.FileSize(indexFile)
		logger.Info(colors.Magenta+"["+progressInfo+"] Pinned", rng, "remote to", remotePin.BloomHash, remotePin.IndexHash, colors.Off)
	}

	return localPin, remotePin, err
}
