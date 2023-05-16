package pinning

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
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

// TODO: BOGUS - WE HAVE TO HAVE A SOLUTION FOR THE TIMESTAMP FILE --PIN --REMOTE ON THE CHIFRA WHEN ROUTINES?
func PinTimestamps(chain string, isRemote bool) error {
	path := config.GetPathToIndex(chain) + "ts.bin"
	var localHash base.IpfsHash
	var remoteHash base.IpfsHash
	var err error
	if LocalDaemonRunning() {
		logger.Info("Pinning timestamps to local daemon")
		localService, _ := NewPinningService(chain, Local)
		if localHash, err = localService.PinFile(path, true); err != nil {
			logger.Fatal("Error in PinTimestamps", err)
			return err
		}
	}

	if isRemote {
		logger.Info("Pinning timestamps to remote daemon")
		remoteService, _ := NewPinningService(chain, Pinata)
		if remoteHash, err = remoteService.PinFile(path, true); err != nil {
			logger.Fatal("Error in PinTimestamps", err)
			return err
		}
	}

	fn := config.GetPathToCache(chain) + "/tmp/ts.ipfs_hash.fil"
	logger.Info("Pinning timestamp file", fn, "to", localHash)
	file.StringToAsciiFile(fn, localHash.String())
	if LocalDaemonRunning() && isRemote {
		if localHash != remoteHash {
			logger.Info("Local and remote hashes differ in PinTimestamps", localHash, remoteHash)
		}
	}

	return nil
}

func PinChunk(chain, path string, isRemote bool) (PinResult, error) {
	bloomFile := cache.ToBloomPath(path)
	indexFile := cache.ToIndexPath(path)

	rng := base.RangeFromFilename(path)
	result := PinResult{
		Range:  rng,
		Local:  types.SimpleChunkRecord{Range: rng.String()},
		Remote: types.SimpleChunkRecord{Range: rng.String()},
	}

	localService, _ := NewPinningService(chain, Local)
	remoteService, _ := NewPinningService(chain, Pinata)

	isLocal := LocalDaemonRunning()
	if isLocal {
		// logger.Info(colors.Magenta+"Pinning locally...", colors.Off)
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
		logger.Info(colors.Magenta+"Pinning remotely...", colors.Off)
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

func (p *Service) PinFile(filepath string, local bool) (base.IpfsHash, error) {
	if local {
		return p.pinFileLocally(filepath)
	}
	return p.pinFileRemotely(filepath)
}
