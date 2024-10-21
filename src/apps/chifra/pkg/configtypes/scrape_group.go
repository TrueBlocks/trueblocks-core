package configtypes

import "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"

type ScrapeSettings struct {
	AppsPerChunk uint64 `toml:"appsPerChunk" json:"appsPerChunk"`
	SnapToGrid   uint64 `toml:"snapToGrid" json:"snapToGrid"`
	FirstSnap    uint64 `toml:"firstSnap" json:"firstSnap"`
	UnripeDist   uint64 `toml:"unripeDist" json:"unripeDist"`
	AllowMissing bool   `toml:"allowMissing" json:"allowMissing,omitempty"`
	ChannelCount uint64 `toml:"channelCount" json:"channelCount,omitempty"`
}

func (s *ScrapeSettings) TestLog(chain string, test bool) {
	logger.TestLog(false, "AppsPerChunk: ", s.AppsPerChunk)
	logger.TestLog(false, "SnapToGrid: ", s.SnapToGrid)
	logger.TestLog(false, "FirstSnap: ", s.FirstSnap)
	logger.TestLog(false, "UnripeDist: ", s.UnripeDist)
	logger.TestLog(false, "ChannelCount: ", s.ChannelCount)
	logger.TestLog(false, "AllowMissing: ", s.AllowMissing)
}
