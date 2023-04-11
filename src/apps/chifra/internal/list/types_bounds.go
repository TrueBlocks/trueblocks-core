package listPkg

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

// TODO: BOGUS2 - MUST DOCUMENT
type simpleBounds struct {
	Count    uint64              `json:"count"`
	FirstApp types.RawAppearance `json:"firstApp"`
	FirstTs  base.Timestamp      `json:"firstTs"`
	LastApp  types.RawAppearance `json:"lastApp"`
	LastTs   base.Timestamp      `json:"lastTs"`
}

func (s *simpleBounds) Raw() *types.RawModeler {
	return nil
}

func (s *simpleBounds) Model(showHidden bool, format string, extraOptions map[string]any) types.Model {
	return types.Model{
		Data: map[string]interface{}{
			"address":   s.FirstApp.Address,
			"count":     s.Count,
			"firstApp":  fmt.Sprintf("%d.%d", s.FirstApp.BlockNumber, s.FirstApp.TransactionIndex),
			"firstTs":   s.FirstTs,
			"firstDate": utils.FormattedDate(s.FirstTs),
			"lastApp":   fmt.Sprintf("%d.%d", s.LastApp.BlockNumber, s.LastApp.TransactionIndex),
			"lastTs":    s.LastTs,
			"lastDate":  utils.FormattedDate(s.LastTs),
			"blockSpan": (s.LastApp.BlockNumber - s.FirstApp.BlockNumber),
			"blockFreq": uint64(s.LastApp.BlockNumber-s.FirstApp.BlockNumber) / s.Count,
		},
		Order: []string{
			"address",
			"count",
			"firstApp",
			"firstTs",
			"firstDate",
			"lastApp",
			"lastTs",
			"lastDate",
			"blockSpan",
			"blockFreq",
		},
	}
}
