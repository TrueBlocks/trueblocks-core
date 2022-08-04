package chunksPkg

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
)

type SimpleMeta struct {
	Latest    uint64 `json:"client"`
	Finalized uint64 `json:"finalized"`
	Staging   uint64 `json:"staging"`
	Ripe      uint64 `json:"ripe"`
	Unripe    uint64 `json:"unripe"`
	ChainId   uint64 `json:"chainId"`
	NetworkId uint64 `json:"networkId"`
	Chain     string `json:"chain"`
}

func (opts *ChunksOptions) HandleStatus(blockNums []uint64) error {
	meta, _ := rpcClient.GetMetaData(opts.Globals.Chain, opts.Globals.TestMode)
	m := SimpleMeta{
		Latest:    meta.Latest,
		Finalized: meta.Finalized,
		Staging:   meta.Staging,
		Ripe:      meta.Ripe,
		Unripe:    meta.Unripe,
		ChainId:   meta.ChainId,
		NetworkId: meta.NetworkId,
		Chain:     meta.Chain,
	}
	defer opts.Globals.RenderFooter()
	err := opts.Globals.RenderHeader(rpcClient.MetaData{}, &opts.Globals.Writer, opts.Globals.Format, opts.Globals.ApiMode, opts.Globals.NoHeader, true)
	if err != nil {
		return err
	}
	return opts.Globals.RenderObject(m, true)
}
