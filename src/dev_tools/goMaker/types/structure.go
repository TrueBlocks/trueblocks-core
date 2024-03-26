package types

import "encoding/json"

type Structure struct {
	Class       string   `json:"class,omitempty" toml:"class"`
	BaseClass   string   `json:"base_class,omitempty" toml:"base_class"`
	Fields      string   `json:"fields,omitempty" toml:"fields"`
	GoOutput    string   `json:"go_output,omitempty" toml:"go_output"`
	DocGroup    string   `json:"doc_group,omitempty" toml:"doc_group"`
	DocRoute    string   `json:"doc_route,omitempty" toml:"doc_route"`
	DocDescr    string   `json:"doc_descr,omitempty" toml:"doc_descr"`
	DocAlias    string   `json:"doc_alias,omitempty" toml:"doc_alias"`
	DocProducer string   `json:"doc_producer,omitempty" toml:"doc_producer"`
	ContainedBy string   `json:"contained_by,omitempty" toml:"contained_by"`
	GoModel     string   `json:"go_model,omitempty" toml:"go_model"`
	CacheAs     string   `json:"cache_as,omitempty" toml:"cache_as"`
	CacheBy     string   `json:"cache_by,omitempty" toml:"cache_by"`
	CacheType   string   `json:"cache_type,omitempty" toml:"cache_type"`
	Members     []Member `json:"members,omitempty" toml:"members"`
}

func (s *Structure) String() string {
	bytes, _ := json.MarshalIndent(s, "", "  ")
	return string(bytes)
}
