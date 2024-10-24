package configtypes

import "encoding/json"

type SettingsGroup struct {
	CachePath      string      `json:"cachePath" toml:"cachePath" comment:"The location of the per chain caches"`
	IndexPath      string      `json:"indexPath" toml:"indexPath" comment:"The location of the per chain unchained indexes"`
	DefaultChain   string      `json:"defaultChain" toml:"defaultChain" comment:"The default chain to use if none is provided"`
	DefaultGateway string      `json:"defaultGateway" toml:"defaultGateway,omitempty"`
	Notify         NotifyGroup `json:"notify" toml:"notify"`
}

func (s *SettingsGroup) String() string {
	bytes, _ := json.Marshal(s)
	return string(bytes)
}
