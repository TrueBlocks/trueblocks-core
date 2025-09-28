package types

import (
	"strings"
)

// Sorts2 for tag {{.Sorts2}}
func (s *Structure) Sorts2() string {
	tmplName := "sortCode"
	tmpl := `
func Sort{{toPlural .Class}}({{toLowerPlural .Class}} []types.{{.Class}}, sortSpec SortSpec) error {
if len(sortSpec.Fields) != len(sortSpec.Order) {
	return fmt.Errorf("fields and order must have the same length")
}

sorts := make([]func(p1, p2 types.{{.Class}}) bool, len(sortSpec.Fields))
for i, field := range sortSpec.Fields {
	if field == "" {
		continue
	}
	if !slices.Contains(types.GetSortFields{{.Class}}(), field) {
		return fmt.Errorf("%s is not an {{.Class}} sort field", field)
	}
	sorts[i] = types.{{.Class}}By(types.{{.Class}}Field(field), types.SortOrder(sortSpec.Order[i]))
}

if len(sorts) > 0 {
	sort.SliceStable({{toLowerPlural .Class}}, types.{{.Class}}Cmp({{toLowerPlural .Class}}, sorts...))
}
return nil
}
`
	return executeTemplate(*s, "sort", tmplName, tmpl)
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
	case "wei":
		return `	case {{.Container}}{{firstUpper .Name}}: // {{.Type}}
		return func(p1, p2 {{.Container}}) bool {
			if order == Ascending {
				return p1.{{.GoName}}.LessThan(&p2.{{.GoName}})
			}
			return p2.{{.GoName}}.LessThan(&p1.{{.GoName}})
		}
`

	case "datetime":
		return `	case {{.Container}}{{firstUpper .Name}}: // {{.Type}}
		return func(p1, p2 {{.Container}}) bool {
			if order == Ascending {
				return p1.{{.GoName}}() < p2.{{.GoName}}()
			}
			return p1.{{.GoName}}() > p2.{{.GoName}}()
		}
`

	case "address":
		return `	case {{.Container}}{{firstUpper .Name}}: // {{.Type}}
		return func(p1, p2 {{.Container}}) bool {
			if order == Ascending {
				return p1.{{.GoName}}.LessThan(p2.{{.GoName}})
			}
			return p2.{{.GoName}}.LessThan(p1.{{.GoName}})
		}
`
	case "RangeDates":
		return `	case {{.Container}}{{firstUpper .Name}}: // {{.Type}}
		return func(p1, p2 {{.Container}}) bool {
			if p1.{{.GoName}} == nil && p2.{{.GoName}} == nil {
				return false
			}
			if p1.{{.GoName}} == nil {
				return order == Ascending
			}
			if p2.{{.GoName}} == nil {
				return order != Ascending
			}
			cmp := p1.{{.GoName}}.Cmp(*p2.{{.GoName}})
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
