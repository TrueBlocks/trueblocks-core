package chunksPkg

import (
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index/bloom"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/tslib"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

func GetChunkStats(chain, path string) (s simpleChunkStats, err error) {
	chunk, err := index.NewChunk(path)
	if err != nil && !os.IsNotExist(err) {
		return s, err
	}
	defer chunk.Close()

	ts, _ := tslib.FromBnToTs(chain, chunk.Range.Last)
	s = simpleChunkStats{
		Range:    chunk.Range.String(),
		RangeEnd: utils.FormattedDate(ts),
		NBlocks:  chunk.Range.Last - chunk.Range.First + 1,
		NAddrs:   uint64(chunk.Data.Header.AddressCount),
		NApps:    uint64(chunk.Data.Header.AppearanceCount),
		NBlooms:  uint64(chunk.Bloom.Count),
		BloomSz:  uint64(file.FileSize(index.ToBloomPath(path))),
		ChunkSz:  uint64(file.FileSize(index.ToIndexPath(path))),
		RecWid:   4 + bloom.BLOOM_WIDTH_IN_BYTES,
	}

	if s.NBlocks > 0 {
		s.AddrsPerBlock = float64(s.NAddrs) / float64(s.NBlocks)
		s.AppsPerBlock = float64(s.NApps) / float64(s.NBlocks)
	}

	if s.NAddrs > 0 {
		s.AppsPerAddr = float64(s.NApps) / float64(s.NAddrs)
	}

	if s.BloomSz > 0 {
		s.Ratio = float64(s.ChunkSz) / float64(s.BloomSz)
	}

	return s, nil
}
