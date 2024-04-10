package types

import (
	"encoding/json"
	"sort"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

type Structure struct {
	Class       string    `json:"class,omitempty" toml:"class" csv:"class"`
	BaseClass   string    `json:"base_class,omitempty" toml:"base_class"`
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

func (s *Structure) String() string {
	bytes, _ := json.MarshalIndent(s, "", "  ")
	return string(bytes)
}

func (s *Structure) HasTimestamp() bool {
	for _, m := range s.Members {
		if m.Name == "timestamp" {
			return true
		}
	}
	return false
}

func (s *Structure) CamelCase() string {
	return CamelCase(s.Class)
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

func (s *Structure) ModelName3() string {
	if s.GoModel != "" {
		return strings.Replace(s.GoModel, "Block[Tx]", "Block[string]", -1)
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

func (s *Structure) NeedsAddress() bool {
	return strings.Contains(s.CacheBy, "address")
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

func (s *Structure) ModelIntro() string {
	tmplName := "modelIntro" + s.Class
	tmpl := strings.Trim(file.AsciiFileToString("src/dev_tools/goMaker/templates/model-intros/"+CamelCase(s.Class)+".md"), "\n\r\t")
	return s.executeTemplate(tmplName, tmpl)
}

func (s *Structure) RouteToGroup(r string) string {
	return s.cbPtr.RouteToGroup(r)
}

func (s *Structure) ModelProducers() string {
	ret := []string{}
	for _, producer := range s.Producers {
		tmplName := "modelProducers"
		tmpl := `- [chifra {{.Route}}](/chifra/{{.Group}}/#chifra-{{.Route}})`
		c := Command{
			Route: producer,
			Group: s.RouteToGroup(producer),
		}
		ret = append(ret, c.executeTemplate(tmplName, tmpl))
	}
	return strings.Join(ret, "\n")
}

func (s *Structure) MemberTable() string {
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

func (s *Structure) HasNotes() bool {
	thePath := "src/dev_tools/goMaker/templates/model-intros/" + CamelCase(s.Class) + ".notes.md"
	return file.FileExists(thePath)
}

func (s *Structure) ModelNotes() string {
	thePath := "src/dev_tools/goMaker/templates/model-intros/" + CamelCase(s.Class) + ".notes.md"
	if file.FileExists(thePath) {
		tmplName := "Notes" + s.Class
		tmpl := file.AsciiFileToString(thePath)
		return strings.Trim(s.executeTemplate(tmplName, tmpl), "\r\n\t")
	}
	return ""
}

func (s *Structure) executeTemplate(name, tmplCode string) string {
	return executeTemplate(s, "structure", name, tmplCode)
}

func (s *Structure) GroupName() string {
	parts := strings.Split(s.DocGroup, "-")
	if len(parts) > 1 {
		return LowerNoSpaces(parts[1])
	}
	return "unknown group: " + s.DocGroup
}

func (s *Structure) Name() string {
	return Lower(s.Class)
}

func (s *Structure) DocLead() string {
	return "DocLead" + s.Class
}

func (s *Structure) GroupWeight() int64 {
	parts := strings.Split(s.DocGroup, "-")
	return utils.MustParseInt(parts[0]) * 1000
}

func (s *Structure) GroupTitle() string {
	parts := strings.Split(s.DocGroup, "-")
	return FirstUpper(Lower(parts[1]))
}

func (s *Structure) GroupIntro() string {
	contents := strings.Trim(file.AsciiFileToString("./src/dev_tools/goMaker/templates/model-groups/"+s.GroupName()+".md"), ws)
	return contents
}

func (s *Structure) Markdowns() string {
	ret := []string{}

	filter := s.GroupName()
	for _, st := range s.cbPtr.Structures {
		if st.GroupName() == filter {
			contents := file.AsciiFileToString("src/dev_tools/goMaker/generated/model_" + st.Name() + ".md")
			ret = append(ret, contents)
		}
	}

	return strings.Join(ret, "\n")
}

func (s Structure) Validate() bool {
	return true
}

func (s *Structure) BaseTypes() string {
	filter := s.GroupName()
	typeMap := map[string]bool{}
	for _, st := range s.cbPtr.Structures {
		g := st.GroupName()
		if g == filter {
			for _, m := range st.Members {
				typeMap[m.Type] = true
			}
		}
	}
	ret := [][]string{}
	for _, t := range s.cbPtr.BaseTypes {
		if typeMap[t.Class] {
			ret = append(ret, []string{t.Class, t.DocDescr, t.DocNotes})
		}
	}

	return MarkdownTable([]string{"Type", "Description", "Notes"}, ret)
}
