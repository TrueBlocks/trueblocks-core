package types

import (
	"encoding/json"
)

type Member struct {
	Num         int    `json:"num" csv:"num"`
	DocOrder    int    `json:"docOrder,omitempty" csv:"docOrder"`
	Name        string `json:"name,omitempty" csv:"name"`
	Type        string `json:"type,omitempty" csv:"type"`
	StrDefault  string `json:"strDefault,omitempty" csv:"strDefault"`
	Attributes  string `json:"attributes,omitempty" csv:"attributes"`
	IsOmitempty bool   `json:"isOmitEmpty,omitempty" csv:"isOmitEmpty"`
	IsCalc      bool   `json:"isCalc,omitempty" csv:"isCalc"`
	IsRawonly   bool   `json:"rawOnly,omitempty" csv:"rawOnly"`
	Description string `json:"description,omitempty" csv:"description"`
	Proper      string `json:"-" csv:"-"`
	Struct      string `json:"-" csv:"-"`
}

func (m *Member) String() string {
	bytes, _ := json.MarshalIndent(m, "", "  ")
	return string(bytes)
}

func (m Member) Validate() bool {
	return m.Name != "" && m.Type != ""
}

func (m *Member) GoName() string {
	if m.Name == "type" {
		return m.Struct + m.Proper
	}
	return m.Proper
}

func (m *Member) GoType() string {
	return m.Type
}
