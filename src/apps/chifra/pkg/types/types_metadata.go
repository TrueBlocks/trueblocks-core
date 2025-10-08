package types

import (
	"encoding/json"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
)

type MetaData struct {
	Latest    base.Blknum    `json:"client"`
	Finalized base.Blknum    `json:"finalized"`
	Staging   base.Blknum    `json:"staging"`
	Ripe      base.Blknum    `json:"ripe"`
	Unripe    base.Blknum    `json:"unripe"`
	ChainId   uint64         `json:"chainId,omitempty"`
	NetworkId uint64         `json:"networkId,omitempty"`
	Chain     string         `json:"chain,omitempty"`
	Calcs     *MetaDataCalcs `json:"calcs,omitempty"`
}

func (m *MetaData) String() string {
	ret, _ := json.MarshalIndent(m, "", "  ")
	return string(ret)
}

func (m *MetaData) Model(chain, format string, verbose bool, extraOpts map[string]any) Model {
	props := NewModelProps(chain, format, verbose, extraOpts)
	rawMap := m.RawMap(props)
	calcMap := m.CalcMap(props, rawMap)

	// Merge raw and calculated maps
	model := make(map[string]any)
	for k, v := range rawMap {
		model[k] = v
	}
	for k, v := range calcMap {
		model[k] = v
	}

	order := []string{
		"client",
		"finalized",
		"staging",
		"ripe",
		"unripe",
		"chainId",
		"networkId",
		"chain",
		"indexHeight",
		"nextIndexHeight",
		"chainHeight",
		"stageHeight",
	}

	return Model{
		Data:  model,
		Order: order,
	}
}

func (m *MetaData) RawMap(props *ModelProps) map[string]any {
	return map[string]any{
		"client":    m.Latest,
		"finalized": m.Finalized,
		"staging":   m.Staging,
		"ripe":      m.Ripe,
		"unripe":    m.Unripe,
		"chainId":   m.ChainId,
		"networkId": m.NetworkId,
		"chain":     m.Chain,
	}
}

func (m *MetaData) CalcMap(props *ModelProps, rawMap map[string]any) map[string]any {
	calcMap := make(map[string]any)

	// Add calculated fields that are based on the raw data
	calcMap["indexHeight"] = m.IndexHeight()
	calcMap["nextIndexHeight"] = m.NextIndexHeight()
	calcMap["chainHeight"] = m.ChainHeight()
	calcMap["stageHeight"] = m.StageHeight()

	return calcMap
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

// MetaDataCalcs holds lazy-loaded calculated fields for MetaData
type MetaDataCalcs struct {
	// EXISTING_CODE
	IndexHeight     base.Blknum `json:"indexHeight"`
	NextIndexHeight base.Blknum `json:"nextIndexHeight"`
	ChainHeight     base.Blknum `json:"chainHeight"`
	StageHeight     base.Blknum `json:"stageHeight"`
	// EXISTING_CODE
}

func (s *MetaData) EnsureCalcs(p *ModelProps, requestedFields []string) error {
	if s.Calcs != nil {
		return nil
	}

	calcMap := s.CalcMap(p, make(map[string]any))
	if len(calcMap) == 0 {
		return nil
	}

	jsonBytes, err := json.Marshal(calcMap)
	if err != nil {
		return err
	}

	s.Calcs = &MetaDataCalcs{}
	return json.Unmarshal(jsonBytes, s.Calcs)
}

// StageHeight returns the highest block that's been staged
func (m *MetaData) StageHeight() base.Blknum {
	return m.Staging
}
