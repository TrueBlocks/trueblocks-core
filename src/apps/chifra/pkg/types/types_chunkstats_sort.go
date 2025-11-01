package types

import "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/logger"

type ChunkStatsField string

// Fields in the ChunkStats struct available for sorting.
const (
	ChunkStatsAddrsPerBlock ChunkStatsField = "addrsPerBlock"
	ChunkStatsAppsPerAddr   ChunkStatsField = "appsPerAddr"
	ChunkStatsAppsPerBlock  ChunkStatsField = "appsPerBlock"
	ChunkStatsBloomSz       ChunkStatsField = "bloomSz"
	ChunkStatsChunkSz       ChunkStatsField = "chunkSz"
	ChunkStatsNAddrs        ChunkStatsField = "nAddrs"
	ChunkStatsNApps         ChunkStatsField = "nApps"
	ChunkStatsNBlocks       ChunkStatsField = "nBlocks"
	ChunkStatsNBlooms       ChunkStatsField = "nBlooms"
	ChunkStatsRange         ChunkStatsField = "range"
	ChunkStatsRangeDates    ChunkStatsField = "rangeDates"
	ChunkStatsRatio         ChunkStatsField = "ratio"
)

// GetSortFieldsChunkStats returns a []string of valid sort fields for the type.
func GetSortFieldsChunkStats() []string {
	return []string{"addrsPerBlock", "appsPerAddr", "appsPerBlock", "bloomSz", "chunkSz", "nAddrs", "nApps", "nBlocks", "nBlooms", "range", "rangeDates", "ratio"}
}

// ChunkStatsBy returns a comparison function for sorting ChunkStats instances by the given field.
// These comparison functions may be strung together by the CmdChunkStats function.
func ChunkStatsBy(field ChunkStatsField, order SortOrder) func(p1, p2 ChunkStats) bool {
	switch field {
	case ChunkStatsAddrsPerBlock: // float64
		return func(p1, p2 ChunkStats) bool {
			if order == Ascending {
				return p1.AddrsPerBlock < p2.AddrsPerBlock
			}
			return p1.AddrsPerBlock > p2.AddrsPerBlock
		}
	case ChunkStatsAppsPerAddr: // float64
		return func(p1, p2 ChunkStats) bool {
			if order == Ascending {
				return p1.AppsPerAddr < p2.AppsPerAddr
			}
			return p1.AppsPerAddr > p2.AppsPerAddr
		}
	case ChunkStatsAppsPerBlock: // float64
		return func(p1, p2 ChunkStats) bool {
			if order == Ascending {
				return p1.AppsPerBlock < p2.AppsPerBlock
			}
			return p1.AppsPerBlock > p2.AppsPerBlock
		}
	case ChunkStatsBloomSz: // uint64
		return func(p1, p2 ChunkStats) bool {
			if order == Ascending {
				return p1.BloomSz < p2.BloomSz
			}
			return p1.BloomSz > p2.BloomSz
		}
	case ChunkStatsChunkSz: // uint64
		return func(p1, p2 ChunkStats) bool {
			if order == Ascending {
				return p1.ChunkSz < p2.ChunkSz
			}
			return p1.ChunkSz > p2.ChunkSz
		}
	case ChunkStatsNAddrs: // uint64
		return func(p1, p2 ChunkStats) bool {
			if order == Ascending {
				return p1.NAddrs < p2.NAddrs
			}
			return p1.NAddrs > p2.NAddrs
		}
	case ChunkStatsNApps: // uint64
		return func(p1, p2 ChunkStats) bool {
			if order == Ascending {
				return p1.NApps < p2.NApps
			}
			return p1.NApps > p2.NApps
		}
	case ChunkStatsNBlocks: // uint64
		return func(p1, p2 ChunkStats) bool {
			if order == Ascending {
				return p1.NBlocks < p2.NBlocks
			}
			return p1.NBlocks > p2.NBlocks
		}
	case ChunkStatsNBlooms: // uint64
		return func(p1, p2 ChunkStats) bool {
			if order == Ascending {
				return p1.NBlooms < p2.NBlooms
			}
			return p1.NBlooms > p2.NBlooms
		}
	case ChunkStatsRange: // blkrange
		return func(p1, p2 ChunkStats) bool {
			if order == Ascending {
				return p1.Range < p2.Range
			}
			return p1.Range > p2.Range
		}
	case ChunkStatsRangeDates: // RangeDates
		return func(p1, p2 ChunkStats) bool {
			if p1.RangeDates == nil && p2.RangeDates == nil {
				return false
			}
			if p1.RangeDates == nil {
				return order == Ascending
			}
			if p2.RangeDates == nil {
				return order != Ascending
			}
			cmp := p1.RangeDates.Cmp(*p2.RangeDates)
			if order == Ascending {
				return cmp == -1
			}
			return cmp == 1
		}
	case ChunkStatsRatio: // float64
		return func(p1, p2 ChunkStats) bool {
			if order == Ascending {
				return p1.Ratio < p2.Ratio
			}
			return p1.Ratio > p2.Ratio
		}

	}
	logger.ShouldNotHappen("in ChunkStatsBy")
	return nil
}

// ChunkStatsCmp accepts a slice and variadic comparison functions and returns a functions
// that can be used to sort the slice.
func ChunkStatsCmp(slice []ChunkStats, orders ...func(p1, p2 ChunkStats) bool) func(i, j int) bool {
	return func(i, j int) bool {
		p1, p2 := slice[i], slice[j]
		for _, order := range orders {
			if order(p1, p2) {
				return true
			}
			if order(p2, p1) {
				return false
			}
		}
		return false
	}
}
