package types

import (
	"encoding/json"
	"sort"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
)

// CodeBase - the top-level structure for the codebase which carries an array of
// Commands which describe, in combination with the Structures, the command line
// options, help file text, and SDK definitions.
type CodeBase struct {
	Commands   []Command   `json:"commands"`
	Structures []Structure `json:"structures"`
	BaseTypes  []Structure `json:"baseTypes"`
}

// String - returns a JSON representation of the codebase
func (c *CodeBase) String() string {
	bytes, _ := json.MarshalIndent(c, "", "    ")
	return string(bytes)
}

// Version - returns the version of the codebase
func (cb *CodeBase) Version(verbose bool) string {
	vers := strings.Trim(file.AsciiFileToString("VERSION"), "\n\r")
	if verbose {
		vers = "GHC-TrueBlocks//" + vers
	}
	return vers + "-release"
}

// Description - returns the description of the codebase for the openapi.yaml file
func (cb *CodeBase) Description() string {
	return strings.Trim(file.AsciiFileToString("src/dev_tools/goMaker/templates/api/description.txt"), ws)
}

// RouteToGroup - returns the group given a route
func (cb *CodeBase) RouteToGroup(route string) string {
	for _, c := range cb.Commands {
		if c.Route == route {
			return c.GroupName()
		}
	}
	return ""
}

// TypeToGroup - returns the group given a type
func (cb *CodeBase) TypeToGroup(typ string) string {
	for _, st := range cb.Structures {
		if strings.EqualFold(st.Name(), typ) {
			return st.GroupName()
		}
	}
	return "unknown type: " + typ
}

// SummaryTag - returns a summary of the commands used in the helpText
func (cb *CodeBase) SummaryTag(filter string) string {
	sort.Slice(cb.Commands, func(i, j int) bool {
		return cb.Commands[i].Num < cb.Commands[j].Num
	})

	ret := []string{}
	for _, c := range cb.Commands {
		if c.Route != "" && c.Group == filter {
			descr := strings.TrimSuffix(FirstLower(c.Description), ".")
			ret = append(ret, "    "+Pad(c.Route, 14)+descr)
		}
	}
	return strings.Join(ret, "\n")
}

// TagSummary - returns a summary of the tags used in the openapi.yaml file
func (cb *CodeBase) TagSummary() string {
	sort.Slice(cb.Commands, func(i, j int) bool {
		return cb.Commands[i].Num < cb.Commands[j].Num
	})

	cur := ""
	ret := []string{}
	for _, c := range cb.Commands {
		if cur != c.Group {
			tmplName := "tags"
			tmpl := `  - name: {{.Group}}
    description: {{.Description}}`
			ret = append(ret, c.executeTemplate(tmplName, tmpl))
		}
		cur = c.Group
	}
	return strings.Join(ret, "\n")
}

func (cb *CodeBase) executeTemplate(name, tmplCode string) string {
	return executeTemplate(cb, "codebase", name, tmplCode)
}
