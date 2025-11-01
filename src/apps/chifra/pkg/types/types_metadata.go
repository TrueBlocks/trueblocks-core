package types

import (
	"encoding/json"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/base"
)

type MetaData struct {
	Latest    base.Blknum `json:"client"`
	Finalized base.Blknum `json:"finalized"`
	Staging   base.Blknum `json:"staging"`
	Ripe      base.Blknum `json:"ripe"`
	Unripe    base.Blknum `json:"unripe"`
	ChainId   uint64      `json:"chainId,omitempty"`
	NetworkId uint64      `json:"networkId,omitempty"`
	Chain     string      `json:"chain,omitempty"`
}

func (m *MetaData) String() string {
	ret, _ := json.MarshalIndent(m, "", "  ")
	return string(ret)
}

// Highest returns the height of the index (i.e., max between the finalized, staging, and ripe indexes).
func (m *MetaData) IndexHeight() base.Blknum {
	return max(m.Finalized, max(m.Staging, m.Ripe))
}

// NextIndexHeight returns the block after the height of the index.
func (m *MetaData) NextIndexHeight() base.Blknum {
	return m.IndexHeight() + 1
}

// ChainHeight returns the block after the height of the index.
func (m *MetaData) ChainHeight() base.Blknum {
	return m.Latest
}

// StageHeight returns the highest block that's been staged
func (m *MetaData) StageHeight() base.Blknum {
	return m.Staging
}
