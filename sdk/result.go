package sdk

import "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"

type Result[T any] struct {
	Data []T            `json:"data"`
	Meta types.MetaData `json:"meta"`
}
