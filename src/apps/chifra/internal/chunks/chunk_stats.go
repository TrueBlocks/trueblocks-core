package chunksPkg

import (
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index/bloom"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func GetChunkStats(path string) (s simpleChunkStats, err error) {
	chunk, err := index.NewChunk(path)
	if err != nil && !os.IsNotExist(err) {
		return s, err
	}
	defer chunk.Close()

	s = simpleChunkStats{
		Start:   chunk.Range.First,
		End:     chunk.Range.Last,
		NBlocks: chunk.Range.Last - chunk.Range.First + 1,
		NAddrs:  uint64(chunk.Data.Header.AddressCount),
		NApps:   uint64(chunk.Data.Header.AppearanceCount),
		NBlooms: uint64(chunk.Bloom.Count),
		BloomSz: uint64(file.FileSize(cache.ToBloomPath(path))),
		ChunkSz: uint64(file.FileSize(cache.ToIndexPath(path))),
		RecWid:  4 + bloom.BLOOM_WIDTH_IN_BYTES,
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

type simpleChunkStats struct {
	AddrsPerBlock float64 `json:"addrsPerBlock"`
	AppsPerAddr   float64 `json:"appsPerAddr"`
	AppsPerBlock  float64 `json:"appsPerBlock"`
	BloomSz       uint64  `json:"bloomSz"`
	ChunkSz       uint64  `json:"chunkSz"`
	End           uint64  `json:"end"`
	NAddrs        uint64  `json:"nAddrs"`
	NApps         uint64  `json:"nApps"`
	NBlocks       uint64  `json:"nBlocks"`
	NBlooms       uint64  `json:"nBlooms"`
	Ratio         float64 `json:"ratio"`
	RecWid        uint64  `json:"recWid"`
	Start         uint64  `json:"start"`
}

func (s *simpleChunkStats) Raw() *types.RawModeler {
	return nil
}

func (s *simpleChunkStats) Model(showHidden bool, format string, extraOptions map[string]any) types.Model {
	return types.Model{
		Data: map[string]any{
			"addrsPerBlock": s.AddrsPerBlock,
			"appsPerAddr":   s.AppsPerAddr,
			"appsPerBlock":  s.AppsPerBlock,
			"bloomSz":       s.BloomSz,
			"chunkSz":       s.ChunkSz,
			"end":           s.End,
			"nAddrs":        s.NAddrs,
			"nApps":         s.NApps,
			"nBlocks":       s.NBlocks,
			"nBlooms":       s.NBlooms,
			"ratio":         s.Ratio,
			"recWid":        s.RecWid,
			"start":         s.Start,
		},
		Order: []string{
			"start",
			"end",
			"nAddrs",
			"nApps",
			"nBlocks",
			"nBlooms",
			"recWid",
			"bloomSz",
			"chunkSz",
			"addrsPerBlock",
			"appsPerBlock",
			"appsPerAddr",
			"ratio",
		},
	}
}
