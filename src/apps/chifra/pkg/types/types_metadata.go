package types

import (
	"encoding/json"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

type MetaData struct {
	Latest    uint64 `json:"client"`
	Finalized uint64 `json:"finalized"`
	Staging   uint64 `json:"staging"`
	Ripe      uint64 `json:"ripe"`
	Unripe    uint64 `json:"unripe"`
	ChainId   uint64 `json:"chainId,omitempty"`
	NetworkId uint64 `json:"networkId,omitempty"`
	Chain     string `json:"chain,omitempty"`
}

func (m *MetaData) String() string {
	ret, _ := json.MarshalIndent(m, "", "  ")
	return string(ret)
}

// Highest returns the height of the index (i.e., max between the finalized, staging, and ripe indexes).
func (m *MetaData) IndexHeight() base.Blknum {
	return utils.Max(m.Finalized, utils.Max(m.Staging, m.Ripe))
}

// NextIndexHeight returns the block after the height of the index.
func (m *MetaData) NextIndexHeight() base.Blknum {
	return m.IndexHeight() + 1
}

// ChainHeight returns the block after the height of the index.
func (m *MetaData) ChainHeight() base.Blknum {
	return m.Latest
}

// StageHieght returns the highest block that's been staged
func (m *MetaData) StageHeight() base.Blknum {
	return m.Staging
}
