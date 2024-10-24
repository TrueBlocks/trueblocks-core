package configtypes

import "encoding/json"

type VersionGroup struct {
	Current string `json:"current" toml:"current" comment:"Do not edit"`
}

func (s *VersionGroup) String() string {
	bytes, _ := json.Marshal(s)
	return string(bytes)
}
