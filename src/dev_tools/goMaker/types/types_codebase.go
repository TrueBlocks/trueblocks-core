package types

import (
	"encoding/json"
	"fmt"
	"path/filepath"
	"sort"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
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
	apiPath := filepath.Join(GetTemplatePath(), "api/description.txt")
	return strings.Trim(file.AsciiFileToString(apiPath), ws)
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

var knownTypes = map[string]bool{
	"addr":        true,
	"address":     true,
	"alias":       true,
	"any":         true,
	"blknum":      true,
	"blkrange":    true,
	"bool":        true,
	"boolean":     true,
	"bytes":       true,
	"command":     true,
	"datetime":    true,
	"ether":       true,
	"flag":        true,
	"float64":     true,
	"float":       true,
	"fourbyte":    true,
	"gas":         true,
	"group":       true,
	"hash":        true,
	"int256":      true,
	"int64":       true,
	"ipfshash":    true,
	"lognum":      true,
	"note":        true,
	"positional":  true,
	"string":      true,
	"switch":      true,
	"timestamp":   true,
	"topic":       true,
	"tx_id":       true,
	"txnum":       true,
	"uint256":     true,
	"uint32":      true,
	"uint64":      true,
	"value":       true,
	"wei":         true,
	"AddrRecord":  true,
	"AppRecord":   true,
	"StorageSlot": true,
	"TokenType":   true,
	"StatePart":   true,
	"DestType":    true,
}

func (cb *CodeBase) Validate() error {
	for _, st := range cb.Structures {
		order := make(map[int]bool, 50)
		for _, m := range st.Members {
			if m.DocOrder > 0 {
				order[m.DocOrder] = true
			}
			if knownTypes[m.Type] || strings.Contains(m.Type, ".") {
				continue
			}

			if cb.TypeToGroup(m.Type) == "unknown type: "+m.Type {
				msg := fmt.Sprintf("unknown type %s in model: %s", m.Type, st.Class)
				logger.Fatal(msg)
			}
		}
		sorted := make([]int, 0, len(order))
		for k := range order {
			sorted = append(sorted, k)
		}
		sort.Ints(sorted)
		for i, v := range sorted {
			if i+1 != v {
				msg := fmt.Sprintf("doc_order is not sequential in model: %s", st.Class)
				logger.Fatal(msg, sorted)
			}
		}
	}

	for _, cmd := range cb.Commands {
		for _, op := range cmd.Options {
			stripped := op.Stripped()
			ot := op.OptionType
			if knownTypes[ot] && knownTypes[stripped] {
				continue
			}
			if op.IsEnum() || op.IsArray() {
				continue
			}

			msg := fmt.Sprintf("unknown types %s.%s in command: %s", stripped, ot, op.LongName)
			logger.Fatal(msg)
		}
	}

	return nil

}

func (op *Option) Stripped() string {
	ret := strings.ReplaceAll(op.DataType, "<addr>", "<address>")
	ret = strings.ReplaceAll(ret, "list<", "")
	ret = strings.ReplaceAll(ret, "enum<", "")
	return CamelCase(strings.ReplaceAll(strings.ReplaceAll(ret, "<", ""), ">", ""))
}

func (cb *CodeBase) Handlers() string {
	ret := []string{}
	for _, cmd := range cb.Commands {
		tmplName := "handlers"
		tmpl := `{{.HandlerRows}}`
		val := cmd.executeTemplate(tmplName, tmpl)
		if len(val) > 0 {
			ret = append(ret, val)
		}
	}
	return strings.Join(ret, "\n")
}
