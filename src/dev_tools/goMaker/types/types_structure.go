package types

import (
	"encoding/json"
	"sort"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

type Structure struct {
	Class       string    `json:"class,omitempty" toml:"class" csv:"class"`
	DocGroup    string    `json:"doc_group,omitempty" toml:"doc_group" csv:"doc_group"`
	DocRoute    string    `json:"doc_route,omitempty" toml:"doc_route"`
	DocDescr    string    `json:"doc_descr,omitempty" toml:"doc_descr" csv:"doc_descr"`
	DocNotes    string    `json:"doc_notes,omitempty" toml:"doc_notes" csv:"doc_notes"`
	ProducedBy  string    `json:"produced_by,omitempty" toml:"produced_by"`
	ContainedBy string    `json:"contained_by,omitempty" toml:"contained_by"`
	GoModel     string    `json:"go_model,omitempty" toml:"go_model"`
	CacheAs     string    `json:"cache_as,omitempty" toml:"cache_as"`
	CacheBy     string    `json:"cache_by,omitempty" toml:"cache_by"`
	CacheType   string    `json:"cache_type,omitempty" toml:"cache_type"`
	DisableGo   bool      `json:"disable_go,omitempty" toml:"disable_go"`
	DisableDocs bool      `json:"disable_docs,omitempty" toml:"disable_docs"`
	Members     []Member  `json:"members,omitempty" toml:"members"`
	Route       string    `json:"-" toml:"-"`
	Producers   []string  `json:"-" toml:"-"`
	cbPtr       *CodeBase `json:"-" toml:"-"`
}

func (s *Structure) executeTemplate(name, tmplCode string) string {
	return executeTemplate(s, "structure", name, tmplCode)
}

func (s *Structure) String() string {
	bytes, _ := json.MarshalIndent(s, "", "  ")
	return string(bytes)
}

func (s Structure) Validate() bool {
	return true
}

func (s *Structure) Name() string {
	return Lower(s.Class)
}

func (s *Structure) Num() int {
	parts := strings.Split(s.DocGroup, "-")
	if len(parts) > 1 {
		return int(base.MustParseInt64(parts[0]))
	}
	logger.Fatal("unknown group: " + s.DocGroup)
	return 0
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

func (s *Structure) HasNotes() bool {
	thePath := "src/dev_tools/goMaker/templates/model-intros/" + CamelCase(s.Class) + ".notes.md"
	return file.FileExists(thePath)
}

func (s *Structure) HasTimestamp() bool {
	for _, m := range s.Members {
		if m.Name == "timestamp" {
			return true
		}
	}
	return false
}

func (s *Structure) NeedsAddress() bool {
	return strings.Contains(s.CacheBy, "address")
}

func (s *Structure) GroupName() string {
	parts := strings.Split(s.DocGroup, "-")
	if len(parts) > 1 {
		return LowerNoSpaces(parts[1])
	}
	logger.Fatal("unknown group: " + s.DocGroup)
	return ""
}

func (s *Structure) ModelName(which string) string {
	if s.GoModel != "" {
		if which == "simple" {
			return strings.Replace(s.GoModel, "Block[Tx]", "Block[Tx string | Transaction]", -1)
		} else if which == "cache" {
			return strings.Replace(s.GoModel, "Block[Tx]", "Block[string]", -1)
		}
		return s.GoModel
	}
	return s.Class

}

func (s *Structure) ModelIntro() string {
	tmplName := "modelIntro" + s.Class
	tmpl := strings.Trim(getContents("templates/model-intros/"+CamelCase(s.Class)), ws)
	return s.executeTemplate(tmplName, tmpl)
}

func (s *Structure) ModelProducers() string {
	ret := []string{}
	for _, route := range s.Producers { // producers are stored as the name of the route that produces it
		tmplName := "modelProducers"
		tmpl := `- [chifra {{.Route}}](/chifra/{{.Group}}/#chifra-{{.Route}})`
		c := Command{
			Route: route,
			Group: s.cbPtr.RouteToGroup(route),
		}
		ret = append(ret, c.executeTemplate(tmplName, tmpl))
	}
	return strings.Join(ret, "\n")
}

func (s *Structure) ModelMembers() string {
	sort.Slice(s.Members, func(i, j int) bool {
		return s.Members[i].DocOrder < s.Members[j].DocOrder
	})
	header := []string{"Field", "Description", "Type"}
	rows := [][]string{}
	for _, m := range s.Members {
		if m.DocOrder > 0 {
			rows = append(rows, []string{m.Name, m.MarkdownDescription(), m.MarkdownType()})
		}
	}
	return MarkdownTable(header, rows)
}

func (s *Structure) ModelNotes() string {
	tmplName := "Notes" + s.Class
	tmpl := strings.Trim(getContents("templates/model-intros/"+CamelCase(s.Class)+".notes"), ws)
	return strings.Trim(s.executeTemplate(tmplName, tmpl), ws)
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
