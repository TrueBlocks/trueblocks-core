package types

import (
	"encoding/json"
	"strings"
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
	templates   TemplateMap `json:"-" csv:"-"`
}

func (s *Structure) String() string {
	s.Members = nil
	bytes, _ := json.MarshalIndent(s, "", "  ")
	return string(bytes)
}

func (s *Structure) RawFields() string {
	ret := []string{}
	for _, member := range s.Members {
		if member.Name == "date" {
			continue
		}
		fu := member.GoName()
		omit := ""
		if member.Omitempty {
			omit = ",omitempty"
		}
		v := "\t" + fu + " string `json:\"" + member.Name + omit + "\"`"
		ret = append(ret, v)
	}
	return strings.Join(ret, "\n")
}

func (s *Structure) MemberFields() string {
	ret := []string{}
	for _, member := range s.Members {
		if member.Name == "date" {
			continue
		}
		fu := strings.ToUpper(member.Name[0:1]) + member.Name[1:]
		omit := ""
		if member.Omitempty {
			omit = ",omitempty"
		}
		v := "\t" + fu + " " + member.Type + " `json:\"" + member.Name + omit + "\"`"
		ret = append(ret, v)
	}
	ret = append(ret, "\traw *Raw"+s.Class+" `json:\"-\"`")

	return strings.Join(ret, "\n")
}

func (s *Structure) DateCode() string {
	hasTs := false
	for _, member := range s.Members {
		if member.Name == "timestamp" {
			hasTs = true
			break
		}
	}
	if hasTs {
		ret := `func (s *Simple{{.Class}}) Date() string {
	return utils.FormattedDate(s.Timestamp)
}`
		return strings.Replace(ret, "{{.Class}}", s.Class, -1)
	} else {
		return ""
	}
}

func (s *Structure) CacheCode() string {
	if s.CacheType == "" {
		return ""
	}
	return `// CacheCode`
}
