package types

import (
	"encoding/json"
	"path/filepath"
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
	UiRoute     string    `json:"ui_route,omitempty" toml:"ui_route"`
	UiIcon      string    `json:"ui_icon,omitempty" toml:"ui_icon"`
	ProducedBy  string    `json:"produced_by,omitempty" toml:"produced_by"`
	ContainedBy string    `json:"contained_by,omitempty" toml:"contained_by"`
	Parent      string    `json:"parent,omitempty" toml:"parent"`
	Children    string    `json:"children,omitempty" toml:"children"`
	CacheAs     string    `json:"cache_as,omitempty" toml:"cache_as"`
	CacheBy     string    `json:"cache_by,omitempty" toml:"cache_by"`
	CacheType   string    `json:"cache_type,omitempty" toml:"cache_type"`
	DisableGo   bool      `json:"disable_go,omitempty" toml:"disable_go"`
	DisableDocs bool      `json:"disable_docs,omitempty" toml:"disable_docs"`
	Attributes  string    `json:"attributes,omitempty" toml:"attributes"`
	Sorts       string    `json:"sorts,omitempty" toml:"sorts"`
	Members     []Member  `json:"members,omitempty" toml:"members"`
	Facets      []Facet   `json:"facets,omitempty" toml:"facets"`
	Route       string    `json:"-" toml:"-"`
	Producers   []string  `json:"-" toml:"-"`
	ChildTabs   []string  `json:"-" toml:"-"`
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
	for _, facet := range s.Facets {
		if err := facet.ValidateAll(); err != nil {
			logger.Fatal("Facet validation failed:", err)
		}
	}
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

func (s *Structure) IsFilenameCache() bool {
	return s.CacheBy == "filename"
}

func (s *Structure) IsMarshalOnly() bool {
	return s.CacheType == "marshal_only"
}

func (s *Structure) ClassOrClassGroup() string {
	if s.IsCacheAsGroup() {
		return s.Class + "Group"
	}
	return s.Class
}

func (s *Structure) IsCacheAsGroup() bool {
	return s.CacheAs == "group"
}

func (s *Structure) HasNotes() bool {
	notePath := filepath.Join(GetTemplatePath(), "model-intros/", CamelCase(s.Class)+".notes.md")
	return file.FileExists(notePath)
}

func (s *Structure) HasTimestamp() bool {
	for _, m := range s.Members {
		if m.Name == "timestamp" {
			return true
		}
	}
	return false
}

func (s *Structure) HasSorts() bool {
	for _, m := range s.Members {
		if strings.Contains(m.Attributes, "sorts") {
			return true
		}
	}
	return len(s.Sorts) > 0
}

func (s *Structure) NeedsAddress() bool {
	return strings.Contains(s.CacheBy, "address")
}

func (s *Structure) NeedsStatement() bool {
	return strings.Contains(s.CacheBy, "statement")
}

func (s *Structure) GroupName() string {
	parts := strings.Split(s.DocGroup, "-")
	if len(parts) > 1 {
		return LowerNoSpaces(parts[1])
	}
	logger.Fatal("unknown group: " + s.DocGroup)
	return ""
}

func (s *Structure) ModelIntro() string {
	tmplName := "modelIntro" + s.Class
	introName := filepath.Join("model-intros", CamelCase(s.Class))
	fullIntroPath := filepath.Join(GetTemplatePath(), introName+".md")
	if !file.FileExists(fullIntroPath) {
		logger.Fatal("missing model intro file:", fullIntroPath, "            ")
	}
	tmpl := strings.Trim(getTemplateContents(introName), ws)
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
	tmpl := strings.Trim(getTemplateContents(filepath.Join("model-intros", CamelCase(s.Class)+".notes")), ws)
	return strings.Trim(s.executeTemplate(tmplName, tmpl), ws)
}

func (s *Structure) CacheLoc() string {
	if s.Class == "LightBlock" {
		return "Block"
	}
	return s.Class
}

func (s *Structure) CacheIdStr() string {
	switch s.CacheBy {
	case "address":
		return "s.Address.Hex()[2:]"
	case "address,block":
		return "fmt.Sprintf(\"%s-%09d\", s.Address.Hex()[2:], s.BlockNumber)"
	case "address,block,fourbyte":
		return "fmt.Sprintf(\"%s-%s-%09d\", s.Address.Hex()[2:], s.Encoding[2:], s.BlockNumber)"
	case "address,tx":
		return "fmt.Sprintf(\"%s-%09d-%05d\", s.Address.Hex()[2:], s.BlockNumber, s.TransactionIndex)"
	case "statement":
		return "fmt.Sprintf(\"%s-%s-%09d-%05d\", s.Holder.Hex()[2:], s.Asset.Hex()[2:], s.BlockNumber, s.TransactionIndex)"
	case "block":
		return "fmt.Sprintf(\"%09d\", s.BlockNumber)"
	case "tx":
		return "fmt.Sprintf(\"%09d-%05d\", s.BlockNumber, s.TransactionIndex)"
	case "filename":
		return "s.Filename"
	default:
		logger.Fatal("Unknown cache by format:", s.CacheBy)
		return ""
	}
}

func (s *Structure) HasTsTypesTypes() bool {
	return len(s.TsTypesTypes()) > 0
}

func (s *Structure) TsTypesTypes() string {
	theMap := map[string]bool{}
	for _, m := range s.Members {
		t := m.BaseType()
		common := t == "string" || t == "any" || t == "boolean" || t == "number"
		isParam := s.Class == "Parameter" && t == "Parameter"
		if !common && !isParam {
			theMap[t] = true
		}
	}
	ret := []string{}
	for key := range theMap {
		ret = append(ret, key)
	}
	sort.Slice(ret, func(i, j int) bool {
		return strings.ToLower(ret[i]) < strings.ToLower(ret[j])
	})
	return strings.Join(ret, ", ")
}

func (s *Structure) TsTypeMembers() string {
	ret := []string{}
	for _, m := range s.Members {
		ret = append(ret, m.MemTsType())
	}
	return strings.Join(ret, "\n")
}

// ====== for browse code gen ======================================
func (s *Structure) findItems() Member {
	for _, m := range s.Members {
		if m.IsItems() {
			return m
		}
	}
	logger.Fatal("no item in structure: ", s.Class)
	return Member{}
}

func (s *Structure) ItemFullType() string {
	return s.findItems().Type
}

func (s *Structure) ItemName() string {
	m := s.findItems()
	parts := strings.Split(m.Type, ".")
	if len(parts) < 2 {
		logger.Fatal("bad embed type (needs two parts): ", m.Type)
	}
	return parts[1]
}

func (s *Structure) ItemType() string {
	m := s.findItems()
	parts := strings.Split(m.Type, ".")
	if len(parts) < 2 {
		logger.Fatal("bad embed type (needs two parts): ", m.Type)
	}
	if strings.HasPrefix(parts[0], "types") {
		return FirstUpper(parts[1])
	}
	return FirstLower(parts[0]) + "." + FirstUpper(parts[1])
}

func (s *Structure) EmbedName() string {
	for _, m := range s.Members {
		if m.IsEmbed() {
			parts := strings.Split(m.Type, ".")
			if len(parts) < 2 {
				logger.Fatal("bad embed type (needs two parts): ", m.Type)
			}
			return Lower(parts[1])
		}
	}
	return ""
}

func (s *Structure) EmbedType() string {
	for _, m := range s.Members {
		if m.IsEmbed() {
			parts := strings.Split(m.Type, ".")
			if len(parts) < 2 {
				logger.Fatal("bad embed type (needs two parts): ", m.Type)
			}
			if parts[0] == "types" {
				return FirstUpper(parts[1])
			}
			return FirstLower(parts[0]) + "." + FirstUpper(parts[1])
		}
	}
	return ""
}

func (s *Structure) Needs(which string) bool {
	no := "no" + FirstUpper(which)
	return !strings.Contains(s.Attributes, no)
}

func (s *Structure) Wants(which string) bool {
	wants := "wants" + FirstUpper(which)
	return strings.Contains(s.Attributes, wants)
}

func (s *Structure) SortsInstance() string {
	flds := []string{}
	orders := []string{}
	spec := s.Sorts
	fields := strings.Split(spec, ",")
	for _, field := range fields {
		parts := strings.Split(field, "+")
		if len(parts) > 1 {
			flds = append(flds, "\""+parts[0]+"\"")
			orders = append(orders, "sdk."+parts[1])
		}
	}
	ret := "sdk.SortSpec {\n"
	ret += "\tFields: []string{" + strings.Join(flds, ",") + "},\n"
	ret += "\tOrder: []sdk.SortOrder{" + strings.Join(orders, ",") + "},\n"
	ret += "}"
	return ret
}

func (s *Structure) getUiRoutePart(p int) string {
	parts := strings.Split(s.UiRoute, "-")
	if len(parts) != 3 {
		s.UiRoute += "-none"
		parts = append(parts, "none")
	}
	return parts[p]
}

func (s *Structure) UiRouteNum() uint64 {
	return base.MustParseUint64(s.getUiRoutePart(0))
}

func (s *Structure) UiRouteName() string {
	return FirstUpper(s.getUiRoutePart(1))
}

func (s *Structure) UiHotKey() string {
	return s.getUiRoutePart(2)
}

func (s *Structure) HasFacets() bool {
	return len(s.Facets) > 0
}

func (s *Structure) FacetsStr() string {
	if !s.HasFacets() {
		return "NO FACETS"
	}
	ret := []string{}
	for _, f := range s.Facets {
		ret = append(ret, f.Name+" ("+f.Store+")")
	}
	return strings.Join(ret, ", ")
}

func (s *Structure) DocSortOrder() []Member {
	ret := s.Members
	sort.Slice(ret, func(i, j int) bool {
		return ret[i].DocOrder < ret[j].DocOrder
	})
	return ret
}

func (s *Structure) CalcMembers() []string {
	mm := map[string]bool{}
	for _, st := range s.Stores() {
		for _, m := range st.Members() {
			if m.IsCalc() {
				mm[m.Name] = true
			}
		}
	}

	ret := []string{}
	for k := range mm {
		ret = append(ret, k)
	}
	sort.Strings(ret)

	return ret
}

func (s *Structure) HasForms() bool {
	for _, f := range s.Facets {
		if f.IsForm() {
			return true
		}
	}
	return false
}

func (s *Structure) RendererTypes() string {
	rendererSet := make(map[string]struct{})
	for _, f := range s.Facets {
		if f.Renderer != "" {
			rendererSet[f.Renderer] = struct{}{}
		}
	}
	var ret []string
	for renderer := range rendererSet {
		ret = append(ret, renderer)
	}
	sort.Strings(ret)
	return strings.Join(ret, ",")
}
