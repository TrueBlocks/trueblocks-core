package sdk

import "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"

// type abisResult struct {
// 	Data []bool       `json:"data"`
// 	Meta rpc.MetaData `json:"meta"`
// }

type Result[T any] struct {
	Data []T          `json:"data"`
	Meta rpc.MetaData `json:"meta"`
}
