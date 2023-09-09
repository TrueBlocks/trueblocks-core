package initPkg

import (
	"context"
	"errors"
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/manifest"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/unchained"
)

func (opts *InitOptions) HandleDryRun() error {
	chain := opts.Globals.Chain

	remoteManifest, err := manifest.ReadManifest(chain, base.HexToAddress(opts.Publisher), manifest.FromContract)
	if err != nil {
		return err
	}

	if remoteManifest.Chain != chain {
		msg := fmt.Sprintf("The chain value found in the downloaded manifest (%s) does not match the manifest on the command line (%s).", remoteManifest.Chain, chain)
		return errors.New(msg)
	}

	err = remoteManifest.SaveManifest(chain)
	if err != nil {
		return err
	}

	// Get the list of things we need to download
	schema := unchained.Schemas
	chunksToDownload, nCorrections := opts.prepareDownloadList(chain, remoteManifest, []uint64{})
	if opts.Globals.TestMode {
		if len(chunksToDownload) > 10 {
			chunksToDownload = chunksToDownload[:10]
		}
		schema = "--testing-hash--"
	}

	// Tell the user what we're doing
	logger.InfoTable("Unchained Index:", unchained.GetUnchainedIndexAddress())
	logger.InfoTable("Schemas:", schema)
	logger.InfoTable("Config Folder:", config.GetPathToChainConfig(chain))
	logger.InfoTable("Index Folder:", config.GetPathToIndex(chain))
	logger.InfoTable("Chunks in Manifest:", fmt.Sprintf("%d", len(remoteManifest.Chunks)))
	logger.InfoTable("Corrections Needed:", fmt.Sprintf("%d", nCorrections))

	ctx := context.Background()
	if opts.Globals.Format == "txt" || opts.Globals.Format == "csv" {
		fetchData := func(modelChan chan types.Modeler[types.RawChunkRecord], errorChan chan error) {
			for _, chunk := range chunksToDownload {
				s := types.SimpleChunkRecord{
					Range:     chunk.Range,
					BloomHash: chunk.BloomHash,
					BloomSize: chunk.BloomSize,
					IndexHash: chunk.IndexHash,
					IndexSize: chunk.IndexSize,
				}
				modelChan <- &s
			}
		}

		return output.StreamMany(ctx, fetchData, opts.Globals.OutputOpts())

	} else {
		fetchData := func(modelChan chan types.Modeler[types.RawManifest], errorChan chan error) {

			s := types.SimpleManifest{
				Chain: chain,
			}
			for _, chunk := range chunksToDownload {
				s.Chunks = append(s.Chunks, types.SimpleChunkRecord{
					Range:     chunk.Range,
					BloomHash: chunk.BloomHash,
					BloomSize: chunk.BloomSize,
					IndexHash: chunk.IndexHash,
					IndexSize: chunk.IndexSize,
				})
			}
			modelChan <- &s
		}

		return output.StreamMany(ctx, fetchData, opts.Globals.OutputOpts())
	}
}
