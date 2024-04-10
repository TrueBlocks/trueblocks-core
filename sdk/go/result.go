package sdk

import "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"

type Result[T any] struct {
	Data []T          `json:"data"`
	Meta rpc.MetaData `json:"meta"`
}
