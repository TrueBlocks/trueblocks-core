package types

import (
	"sort"
	"strings"
)

func (c *Command) HasSorts() bool {
	return strings.Contains(c.Attributes, "sorts=")
}

func getSorts(attributes string) []string {
	parts := strings.Split(attributes, "=")
	if len(parts) < 2 {
		return []string{}
	}
	parts = strings.Split(parts[1], ":")
	ret := []string{}
	for _, part := range parts {
		ret = append(ret, FirstUpper(part))
	}
	sort.Strings(ret)
	return ret
}

type Sort struct {
	Type string
}

// Sorts2 for tag {{.Sorts2}}
func (c *Command) Sorts2() string {
	ret := ""
	for _, sort := range c.Sorts {
		tmplName := "sortCode"
		tmpl := `
func Sort{{toPlural .Type}}({{toLowerPlural .Type}} []types.{{.Type}}, sortSpec SortSpec) error {
	if len(sortSpec.Fields) != len(sortSpec.Order) {
		return fmt.Errorf("Fields and Order must have the same length")
	}

	sorts := make([]func(p1, p2 types.{{.Type}}) bool, len(sortSpec.Fields))
	for i, field := range sortSpec.Fields {
		if !types.IsValid{{.Type}}Field(field) {
			return fmt.Errorf("%s is not an {{.Type}} sort field", field)
		}
		sorts[i] = types.{{.Type}}By(types.{{.Type}}Field(field), types.SortOrder(sortSpec.Order[i]))
	}

	sort.Slice({{toLowerPlural .Type}}, types.{{.Type}}Cmp({{toLowerPlural .Type}}, sorts...))
	return nil
}
`
		ss := Sort{Type: sort}
		ret += executeTemplate(ss, "sort", tmplName, tmpl)
	}
	return ret
}

func (m *Member) IsSortable() bool {
	return strings.Contains(m.Attributes, "sort")
}

func (s *Structure) SortString() string {
	ret := ""
	for _, member := range s.Members {
		if member.IsSortable() {
			ret += ("\"" + FirstLower(member.Name) + "\",")
		}
	}
	return strings.Trim(ret, ",")
}

func (s *Structure) SortFields() string {
	fields := []string{}
	for _, member := range s.Members {
		if member.IsSortable() {
			tmplName := "sortField" + s.Class
			tmpl := s.Class + "{{firstUpper .Name}} {{.Container}}Field = \"{{firstLower .Name}}\""
			code := member.executeTemplate(tmplName, tmpl)
			if member.Type == "type" {
				code = strings.ReplaceAll(code, s.Class+s.Class, s.Class)
			}
			fields = append(fields, code)
		}
	}
	return "const (\n" + strings.Join(fields, "\n") + ")\n"
}

func (s *Structure) SortSwitches() string {
	fields := []string{}
	for _, member := range s.Members {
		if member.IsSortable() {
			tmplName := "sortCase" + member.Type + s.Class
			tmpl := getSortCode(member.Type)
			fields = append(fields, member.executeTemplate(tmplName, tmpl))
		}
	}
	return strings.Join(fields, "")
}

func getSortCode(typ string) string {
	switch typ {
	case "bool":
		return `	case {{.Container}}{{firstUpper .Name}}: // {{.Type}}
		return func(p1, p2 {{.Container}}) bool {
			if order == Ascending {
				return !p1.{{.GoName}} && p2.{{.GoName}}
			}
			return p1.{{.GoName}} && !p2.{{.GoName}}
		}
`
	case "address":
		return `	case {{.Container}}{{firstUpper .Name}}: // {{.Type}}
		return func(p1, p2 {{.Container}}) bool {
			cmp := p1.{{.GoName}}.Cmp(p2.Address.{{.GoName}})
			if order == Ascending {
				return cmp == -1
			}
			return cmp == 1
		}
`
	case "RangeDates":
		return `	case {{.Container}}{{firstUpper .Name}}: // {{.Type}}
		return func(p1, p2 {{.Container}}) bool {
			cmp := p1.{{.GoName}}.Cmp(p2.{{.GoName}})
			if order == Ascending {
				return cmp == -1
			}
			return cmp == 1
		}
`
	default:
		return `	case {{.Container}}{{firstUpper .Name}}: // {{.Type}}
		return func(p1, p2 {{.Container}}) bool {
			if order == Ascending {
				return p1.{{.GoName}} < p2.{{.GoName}}
			}
			return p1.{{.GoName}} > p2.{{.GoName}}
		}
`
	}
}
