package types

import "encoding/json"

type Member struct {
	Num         int    `json:"num" csv:"num"`
	DocOrder    int    `json:"docOrder,omitempty" csv:"docOrder"`
	Name        string `json:"name,omitempty" csv:"name"`
	Type        string `json:"type,omitempty" csv:"type"`
	StrDefault  string `json:"strDefault,omitempty" csv:"strDefault"`
	Omitempty   bool   `json:"omitempty,omitempty" csv:"omitempty"`
	Description string `json:"description,omitempty" csv:"description"`
}

func (m *Member) String() string {
	bytes, _ := json.MarshalIndent(m, "", "  ")
	return string(bytes)
}

func (m Member) Validate() bool {
	return m.Name != "" && m.Type != ""
}
