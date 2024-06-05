package chunksPkg

import (
	"context"
	"sort"
	"strings"
	"time"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/pinning"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (opts *ChunksOptions) HandleList(unusedBns []base.Blknum) error {
	testMode := opts.Globals.TestMode
	if testMode {
		logger.Info("Test mode: list pins not tested")
		return nil
	}

	ctx := context.Background()
	fetchData := func(modelChan chan types.Modeler, errorChan chan error) {
		var perPage = 1000
		if testMode {
			perPage = -100
		}

		showProgress := opts.Globals.ShowProgressNotTesting()
		chunks := map[string]types.ChunkRecord{}
		if array, err := pinning.ListPins(opts.Globals.Chain, "pinned", showProgress, opts.Count, perPage, time.Millisecond*500); err != nil {
			errorChan <- err
		} else {
			for _, line := range array {
				parts := strings.Split(line, "\t")
				if len(parts) == 5 {
					s := types.IpfsPin{
						Cid:        base.IpfsHash(parts[0]),
						DatePinned: parts[1],
						FileName:   parts[2],
						Size:       base.MustParseInt64(parts[3]),
						Status:     parts[4],
					}
					rng := strings.Split(s.FileName, ".")[0]
					chunk := chunks[rng]
					chunk.Range = rng
					if strings.HasSuffix(s.FileName, ".bloom") {
						chunk.BloomHash = s.Cid
						chunk.BloomSize = s.Size
					} else if strings.HasSuffix(s.FileName, ".bin") {
						chunk.IndexHash = s.Cid
						chunk.IndexSize = s.Size
					} else {
						logger.Info("Skipping:", s.FileName)
					}
					chunks[rng] = chunk
				}
			}
		}

		chunkArray := []types.ChunkRecord{}
		for _, chunk := range chunks {
			chunkArray = append(chunkArray, chunk)
		}
		sort.Slice(chunkArray, func(i, j int) bool {
			return chunkArray[i].Range < chunkArray[j].Range
		})
		for _, chunk := range chunkArray {
			modelChan <- &chunk
		}
	}

	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOpts())
}
