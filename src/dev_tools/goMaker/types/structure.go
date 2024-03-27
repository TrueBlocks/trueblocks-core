package types

import (
	"encoding/json"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

type Structure struct {
	Name        string      `json:"name,omitempty" toml:"name"`
	Class       string      `json:"class,omitempty" toml:"class"`
	BaseClass   string      `json:"base_class,omitempty" toml:"base_class"`
	Fields      string      `json:"fields,omitempty" toml:"fields"`
	GoOutput    string      `json:"go_output,omitempty" toml:"go_output"`
	DocGroup    string      `json:"doc_group,omitempty" toml:"doc_group"`
	DocRoute    string      `json:"doc_route,omitempty" toml:"doc_route"`
	DocDescr    string      `json:"doc_descr,omitempty" toml:"doc_descr"`
	DocAlias    string      `json:"doc_alias,omitempty" toml:"doc_alias"`
	DocProducer string      `json:"doc_producer,omitempty" toml:"doc_producer"`
	ContainedBy string      `json:"contained_by,omitempty" toml:"contained_by"`
	GoModel     string      `json:"go_model,omitempty" toml:"go_model"`
	CacheAs     string      `json:"cache_as,omitempty" toml:"cache_as"`
	CacheBy     string      `json:"cache_by,omitempty" toml:"cache_by"`
	CacheType   string      `json:"cache_type,omitempty" toml:"cache_type"`
	DisableGo   bool        `json:"disable_go,omitempty" toml:"disable_go"`
	DisableDocs bool        `json:"disable_docs,omitempty" toml:"disable_docs"`
	Members     []Member    `json:"members,omitempty" toml:"members"`
	Route       string      `json:"-" toml:"-"`
	templates   TemplateMap `json:"-" toml:"-"`
}

func (s *Structure) String() string {
	s.Members = nil
	bytes, _ := json.MarshalIndent(s, "", "  ")
	return string(bytes)
}

func (s *Structure) RawFields() string {
	ret := []string{}
	for _, m := range s.Members {
		if m.IsCalc || m.IsSimpleOnly || strings.Contains(m.Name, "::") {
			continue
		}
		tmpl := `{{.GoName}} {{.RawType}} {{.RawTag}}`
		ret = append(ret, m.executeTemplate("rawFields", tmpl))
	}
	return strings.Join(ret, "\n")
}

func (s *Structure) MemberFields() string {
	ret := []string{}
	for _, m := range s.Members {
		if m.IsCalc || m.IsRawOnly || strings.Contains(m.Name, "::") {
			continue
		}
		tmpl := `{{.GoName}} {{.GoType}} {{.Tag}}`
		ret = append(ret, m.executeTemplate("fields", tmpl))
	}
	tmpl := "raw *Raw{{.Class}} `json:\"-\"`"
	ret = append(ret, s.executeTemplate("raw", tmpl))

	return strings.Join(ret, "\n")
}

func (s *Structure) HasTimestamp() bool {
	for _, m := range s.Members {
		if m.Name == "timestamp" {
			return true
		}
	}
	return false
}

func (s *Structure) CacheMsg() string {
	ret := ""
	if s.CacheType != "" {
		ret = s.CacheType
		if s.CacheBy != "" {
			ret += " by " + s.CacheBy
		}
		if s.CacheAs != "" {
			ret += " as " + s.CacheAs
		}
	}
	return ret
}

func (s *Structure) ModelName() string {
	if s.GoModel != "" {
		return s.GoModel
	}
	return s.Class
}

func (s *Structure) ModelName2() string {
	if s.GoModel != "" {
		return strings.Replace(s.GoModel, "Block[Tx]", "Block[Tx string | SimpleTransaction]", -1)
	}
	return s.ModelName()
}

func (s *Structure) IsCachable() bool {
	return s.CacheType != ""
}

func (s *Structure) IsMarshalOnly() bool {
	return s.CacheType == "marshal_only"
}

func (s *Structure) IsCacheAsGroup() bool {
	return s.CacheAs == "group"
}

func (s *Structure) IncludeAddr() bool {
	return s.CacheAs == "group"
}

func (s *Structure) CacheIdStr() string {
	switch s.CacheBy {
	case "address,block":
		return "\"%s-%09d\", s.Address.Hex()[2:], s.BlockNumber"
	case "address,block,fourbyte":
		return "\"%s-%s-%09d\", s.Address.Hex()[2:], s.Encoding[2:], s.BlockNumber"
	case "address,tx":
		return "\"%s-%09d-%05d\", s.Address.Hex()[2:], s.BlockNumber, s.TransactionIndex"
	case "block":
		return "\"%09d\", s.BlockNumber"
	case "tx":
		return "\"%09d-%05d\", s.BlockNumber, s.TransactionIndex"
	default:
		logger.Fatal("Unknown cache by format:", s.CacheBy)
		return ""
	}
}
