package types

import (
	"encoding/json"
	"sort"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
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
	Producers   []string    `json:"-" toml:"-"`
	cbPtr       *CodeBase   `json:"-" toml:"-"`
	templates   TemplateMap `json:"-" toml:"-"`
}

func (s *Structure) String() string {
	bytes, _ := json.MarshalIndent(s, "", "  ")
	return string(bytes)
}

func (s *Structure) SnakeCase() string {
	return SnakeCase(s.Class)
}

func (s *Structure) HasTimestamp() bool {
	for _, m := range s.Members {
		if m.Name == "timestamp" {
			return true
		}
	}
	return false
}

func (s *Structure) ModelName() string {
	if s.GoModel != "" {
		return s.GoModel
	}
	return s.Class
}

func (s *Structure) ModelName3() string {
	if s.GoModel != "" {
		return strings.Replace(s.GoModel, "Block[Tx]", "Block[string]", -1)
	}
	return s.ModelName()
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
	tmpl := strings.Trim(file.AsciiFileToString("src/dev_tools/goMaker/templates/model-intros/"+SnakeCase(s.Class)+".md"), "\n\r\t")
	return s.executeTemplate("Intro", tmpl)
}

func (s *Structure) RouteToGroup(r string) string {
	return s.cbPtr.RouteToGroup(r)
}

func (s *Structure) ModelProducers() string {
	ret := []string{}
	for _, producer := range s.Producers {
		tmpl := `- [chifra {{.Route}}](/chifra/{{.Group}}/#chifra-{{.Route}})`
		c := Command{
			Route: producer,
			Group: s.RouteToGroup(producer),
		}
		ret = append(ret, c.executeTemplate("modelProducers", tmpl))
	}
	return strings.Join(ret, "\n")
}

func (s *Structure) TypeToGroup(t string) string {
	return s.cbPtr.TypeToGroup[strings.ToLower(t)]
}

func (s *Structure) Widest() (int, int, int) {
	widest := [3]int{1, 1, 1}
	for _, m := range s.Members {
		if m.DocOrder > 0 {
			if len(m.Name) > widest[0] {
				widest[0] = len(m.Name)
			}
			if len(m.MarkdownDescription()) > widest[1] {
				widest[1] = len(m.MarkdownDescription())
			}
			if len(m.MarkdownType()) > widest[2] {
				widest[2] = len(m.MarkdownType())
			}
		}
	}
	return widest[0], widest[1], widest[2]
}

func (s *Structure) dividerRow() string {
	wName, wDescr, wType := s.Widest()
	name := strings.Repeat("-", wName)
	descr := strings.Repeat("-", wDescr)
	typ := strings.Repeat("-", wType)
	return "| " + strings.Join([]string{name, descr, typ}, " | ") + " |"
}

func (s *Structure) markdownRow(fields []string) string {
	wName, wDescr, wType := s.Widest()
	name := Pad(fields[0], wName)
	descr := Pad(fields[1], wDescr)
	typ := Pad(fields[2], wType)
	return "| " + strings.Join([]string{name, descr, typ}, " | ") + " |"
}

func (s *Structure) MarkdownTable() string {
	sort.Slice(s.Members, func(i, j int) bool {
		return s.Members[i].DocOrder < s.Members[j].DocOrder
	})
	ret := []string{}
	ret = append(ret, s.markdownRow([]string{"Field", "Description", "Type"}))
	ret = append(ret, s.dividerRow())
	for _, m := range s.Members {
		if m.DocOrder > 0 {
			ret = append(ret, s.markdownRow([]string{m.Name, m.MarkdownDescription(), m.MarkdownType()}))
		}
	}
	return strings.Join(ret, "\n")
}

// | Field          | Description                                                                                                          | Type      |
// | -------------- | -------------------------------------------------------------------------------------------------------------------- | --------- |
// | address        | the recipient for the withdrawn ether                                                                                | address   |
// | amount         | a nonzero amount of ether given in gwei (1e9 wei)                                                                    | wei       |
// | blockNumber    | the number of this block                                                                                             | blknum    |
// | index          | a monotonically increasing zero-based index that increments by 1 per withdrawal to uniquely identify each withdrawal | uint64    |
// | timestamp      | the timestamp for this block                                                                                         | timestamp |
// | date           | the timestamp as a date (calculated)                                                                                 | datetime  |
// | validatorIndex | the validator_index of the validator on the consensus layer the withdrawal corresponds to                            | uint64    |
// }

func (s *Structure) ModelNotes() string {
	thePath := "src/dev_tools/goMaker/templates/model-intros/" + SnakeCase(s.Class) + ".md"
	thePath = strings.Replace(thePath, ".md", ".notes.md", -1)
	if file.FileExists(thePath) {
		tmpl := file.AsciiFileToString(thePath)
		return "\n\n" + strings.Trim(s.executeTemplate("Notes", tmpl), "\r\n\t")
	}
	return ""
}

func (s *Structure) Plural() string {
	return Plural(s.Class)
}
