package sdk

import "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"

type SortOrder = types.SortOrder

const (
	Asc SortOrder = types.Ascending
	Dec SortOrder = types.Descending
)

type SortSpec struct {
	Fields []string    `json:"fields"`
	Order  []SortOrder `json:"orders"`
}
