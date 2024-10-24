package configtypes

import "encoding/json"

type NotifyGroup struct {
	Url    string `json:"url,omitempty" toml:"url"`
	Author string `json:"author,omitempty" toml:"author"`
}

func (s *NotifyGroup) String() string {
	bytes, _ := json.Marshal(s)
	return string(bytes)
}
