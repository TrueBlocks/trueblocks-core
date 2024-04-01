package types

import (
	"encoding/json"
	"sort"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
)

type CodeBase struct {
	Commands   []Command   `json:"commands" csv:"commands"`
	Structures []Structure `json:"structures" csv:"structures"`
}

func (c *CodeBase) String() string {
	bytes, _ := json.MarshalIndent(c, "", "    ")
	return string(bytes)
}

func (cb *CodeBase) TypeToGroup(t string) string {
	for _, st := range cb.Structures {
		if strings.EqualFold(st.Name, t) {
			parts := strings.Split(strings.ToLower(st.DocGroup), "-")
			if len(parts) > 1 {
				return strings.ReplaceAll(parts[1], " ", "")
			}
		}
	}
	return "unknown type: " + t
}

func (cb *CodeBase) Summary(filter string) string {
	ret := []string{}
	sort.Slice(cb.Commands, func(i, j int) bool {
		return cb.Commands[i].Endpoint.Num < cb.Commands[j].Endpoint.Num
	})
	for _, c := range cb.Commands {
		if c.Route == "" {
			continue
		}
		if c.Endpoint.Group == filter {
			descr := strings.ToLower(c.Endpoint.Description[0:1])
			descr += strings.TrimSuffix(c.Endpoint.Description[1:], ".")
			ret = append(ret, "    "+Pad(c.Endpoint.ApiRoute, 14)+descr)
		}
	}
	return strings.Join(ret, "\n")
}

func (cb *CodeBase) AccountsSummary() string {
	return cb.Summary("Accounts")
}

func (cb *CodeBase) ChainDataSummary() string {
	return cb.Summary("Chain Data")
}

func (cb *CodeBase) ChainStateSummary() string {
	return cb.Summary("Chain State")
}

func (cb *CodeBase) AdminSummary() string {
	return cb.Summary("Admin")
}

func (cb *CodeBase) OtherSummary() string {
	return cb.Summary("Other")
}

func (cb *CodeBase) Version() string {
	vers := strings.Trim(file.AsciiFileToString("VERSION"), "\n\r")
	return vers + "-release"
}

func (cb *CodeBase) VersionLong() string {
	return "GHC-TrueBlocks//" + cb.Version()
}

func (cb *CodeBase) Tags() string {
	sort.Slice(cb.Commands, func(i, j int) bool {
		return cb.Commands[i].Endpoint.Num < cb.Commands[j].Endpoint.Num
	})

	ret := []string{}
	for _, c := range cb.Commands {
		if c.Route == "" && c.Group != "" {
			tmplName := "tags"
			tmpl := `  - name: {{.Group}}
    description: {{.Description}}`
			ret = append(ret, c.executeTemplate(tmplName, tmpl))
		}
	}
	return strings.Join(ret, "\n")
}

func (cb *CodeBase) Description() string {
	return "\n" + strings.Trim(file.AsciiFileToString("src/dev_tools/goMaker/templates/api/description.txt"), "\n\t\r")
}

func (cb *CodeBase) RouteToGroup(route string) string {
	for _, c := range cb.Commands {
		if c.Route == route {
			return strings.ToLower(SnakeCase(c.Group))
		}
	}
	return ""
}
