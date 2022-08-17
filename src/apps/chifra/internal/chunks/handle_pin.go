package chunksPkg

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config/scrape"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index/bloom"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/pinning"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

const (
	ESTUARY_URL = "https://shuttle-4.estuary.tech/content/add"
	PINATA_URL  = "https://api.pinata.cloud/pinning/pinFileToIPFS"
)

func (opts *ChunksOptions) pinFile(ctx *WalkContext, path string, first bool) (bool, error) {
	type Result struct {
		Range   cache.FileRange         `json:"range,omitempty"`
		Local   types.SimpleChunkRecord `json:"local,omitempty"`
		Remote  types.SimpleChunkRecord `json:"remote,omitempty"`
		Matches bool                    `json:"matches,omitempty"`
		err     error
	}

	rng, _ := cache.RangeFromFilename(path)
	result := Result{
		Range:  rng,
		Local:  types.SimpleChunkRecord{Range: rng.String()},
		Remote: types.SimpleChunkRecord{Range: rng.String()},
	}

	pinataKey, pinataSecret, _ := scrape.PinningKeys(opts.Globals.Chain)
	localService := pinning.Service{}
	remoteService := pinning.Service{
		PinUrl:     PINATA_URL,
		Apikey:     pinataKey,
		Secret:     pinataSecret,
		ResultName: "IpfsHash",
		HeaderFunc: PinataHeaders,
	}

	bloomFile := bloom.ToBloomPath(path)
	indexFile := index.ToIndexPath(path)

	if pinning.LocalDaemonRunning() {
		if result.Local.BloomHash, result.err = localService.PinFile(bloomFile, true); result.err != nil {
			return false, result.err
		}
		if result.Local.IndexHash, result.err = localService.PinFile(indexFile, true); result.err != nil {
			return false, result.err
		}
	}

	if opts.Remote {
		if result.Remote.BloomHash, result.err = remoteService.PinFile(bloomFile, false); result.err != nil {
			return false, result.err
		}
		if result.Remote.IndexHash, result.err = remoteService.PinFile(indexFile, false); result.err != nil {
			return false, result.err
		}
	}

	if pinning.LocalDaemonRunning() && opts.Remote {
		result.Matches = result.Local.IndexHash == result.Remote.IndexHash && result.Local.BloomHash == result.Remote.BloomHash
	} else {
		result.Matches = true
	}

	// msg := fmt.Sprintf("Pinning chunk: %s %t", result.Range, result.Matches)
	// logger.Log(logger.Progress, msg)

	if ctx.Data != nil {
		ptr, castOk := ctx.Data.(*int)
		if !castOk {
			return true, fmt.Errorf("could not case pinList")
		}
		*ptr = *ptr + 1
		if pinning.LocalDaemonRunning() {
			return true, opts.Globals.RenderObject(result.Local, *ptr == 1)
		} else {
			return true, opts.Globals.RenderObject(result.Remote, *ptr == 1)
		}
	}

	return true, nil
}

type PinList []types.SimpleChunkRecord

func (opts *ChunksOptions) HandlePin(blockNums []uint64) error {
	defer opts.Globals.RenderFooter()
	err := opts.Globals.RenderHeader(types.SimpleChunkRecord{}, &opts.Globals.Writer, opts.Globals.Format, opts.Globals.ApiMode, opts.Globals.NoHeader, true)
	if err != nil {
		return err
	}

	count := int(0)
	ctx := WalkContext{
		VisitFunc: opts.pinFile,
		Data:      &count,
	}

	return opts.WalkIndexFiles(&ctx, cache.Index_Bloom, blockNums)
}

func PinataHeaders(s *pinning.Service, contentType string) map[string]string {
	headers := make(map[string]string)
	headers["Content-Type"] = contentType
	headers["pinata_secret_api_key"] = s.Secret
	headers["pinata_api_key"] = s.Apikey
	return headers
}

func EstuaryHeaders(s *pinning.Service, contentType string) map[string]string {
	headers := make(map[string]string)
	headers["Content-Type"] = contentType
	headers["Accept"] = "application/json"
	headers["Authorization"] = "Bearer " + s.Apikey
	return headers
}
