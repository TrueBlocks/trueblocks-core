package configtypes

import (
	"encoding/json"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/logger"
)

type ScrapeSettings struct {
	AppsPerChunk uint64 `json:"appsPerChunk" toml:"appsPerChunk"`
	SnapToGrid   uint64 `json:"snapToGrid" toml:"snapToGrid"`
	FirstSnap    uint64 `json:"firstSnap" toml:"firstSnap"`
	UnripeDist   uint64 `json:"unripeDist" toml:"unripeDist"`
	AllowMissing bool   `json:"allowMissing,omitempty" toml:"allowMissing"`
	ChannelCount uint64 `json:"channelCount,omitempty" toml:"channelCount"`
}

func (s *ScrapeSettings) String() string {
	bytes, _ := json.Marshal(s)
	return string(bytes)
}

func (s *ScrapeSettings) TestLog(chain string, test bool) {
	_ = chain
	_ = test
	logger.TestLog(false, "AppsPerChunk: ", s.AppsPerChunk)
	logger.TestLog(false, "SnapToGrid: ", s.SnapToGrid)
	logger.TestLog(false, "FirstSnap: ", s.FirstSnap)
	logger.TestLog(false, "UnripeDist: ", s.UnripeDist)
	logger.TestLog(false, "ChannelCount: ", s.ChannelCount)
	logger.TestLog(false, "AllowMissing: ", s.AllowMissing)
}
