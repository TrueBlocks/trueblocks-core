package types

import (
	"fmt"
	"os"
	"sort"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

type Command struct {
	Num          int          `json:"num"`
	Folder       string       `json:"folder,omitempty"`
	Route        string       `json:"route,omitempty"`
	Group        string       `json:"group,omitempty"`
	Tool         string       `json:"tool,omitempty"`
	Description  string       `json:"description,omitempty"`
	Options      []Option     `json:"options,omitempty"`
	ReturnType   string       `json:"return_type,omitempty"`
	Capabilities string       `json:"capabilities,omitempty"`
	Usage        string       `json:"usage,omitempty"`
	Summary      string       `json:"summary,omitempty"`
	Notes        []string     `json:"notes,omitempty"`
	Aliases      []string     `json:"aliases,omitempty"`
	Productions  []*Structure `json:"productions,omitempty"`
	cbPtr        *CodeBase    `json:"-"`
}

func (c *Command) TypeToGroup(t string) string {
	return c.cbPtr.TypeToGroup(t)
}

func (c *Command) ProducedByDescr() string {
	g := c.GroupName()
	types := []string{}
	for i, production := range c.Productions {
		lowerProd := strings.ToLower(production.Class)
		groupProd := c.TypeToGroup(lowerProd)
		if i > 0 {
			if i == len(c.Productions)-1 {
				types = append(types, " or ")
			} else {
				types = append(types, ", ")
			}
		}
		tmpl := fmt.Sprintf("<a href=\"/data-model/%s/#%s\">%s</a>", groupProd, lowerProd, production.Class)
		types = append(types, c.executeTemplate("produces"+production.Class, tmpl))
	}
	tmpl := fmt.Sprintf(" Corresponds to the <a href=\"/chifra/%s/#chifra-{{.Route}}\">chifra {{.Route}}</a> command line.", g)
	return "Produces " + strings.Join(types, "") + " data." + c.executeTemplate("corresponds"+c.Route, tmpl)
}

func (c *Command) ProducedByList() string {
	ret := []string{}
	if len(c.Productions) == 1 {
		camel := CamelCase(c.Productions[0].Class)
		ret = []string{fmt.Sprintf("                      $ref: \"#/components/schemas/%s\"", camel)}
	} else {
		ret = append(ret, "                      oneOf:")
		for _, production := range c.Productions {
			camel := CamelCase(production.Class)
			s := fmt.Sprintf("                        - $ref: \"#/components/schemas/%s\"", camel)
			ret = append(ret, s)
		}
	}
	return strings.Join(ret, "\n") + "\n"
}

func (c *Command) HasPositionals() bool {
	for _, op := range c.Options {
		if op.IsPositional() {
			return true
		}
	}
	return false
}

func (c *Command) HasNotes() bool {
	return len(c.Notes) > 0
}

func (c *Command) HasExample() bool {
	return file.FileExists("./src/dev_tools/goMaker/templates/api/examples/" + c.Route + ".txt")
}

func (c *Command) HasHidden() bool {
	for _, op := range c.Options {
		if op.IsHidden() {
			return true
		}
	}
	return false
}

func (c *Command) HasAddrs() bool {
	for _, op := range c.Options {
		if op.DataType == "<address>" || op.DataType == "list<addr>" {
			return true
		}
	}
	return false
}

func (c *Command) HasEnums() bool {
	for _, op := range c.Options {
		if op.IsEnum() {
			return true
		}
	}
	return false
}

func (c *Command) Clean() {
	cleaned := []Option{}
	c.Notes = []string{}
	c.Aliases = []string{}
	for _, op := range c.Options {
		op.GoName = op.toGoName()
		op.GoType = op.toGoType()
		op.GoSdkName = op.toGoSdkName()
		op.GoSdkType = op.toGoSdkType()
		op.GoOptionsType = op.toGoOptionsType()
		if strings.Contains(op.DataType, "enum[") {
			v := strings.Replace(strings.Replace(strings.Split(op.DataType, "[")[1], "]", "", -1), ">", "", -1)
			op.Enums = strings.Split(v, "|")
			for i := 0; i < len(op.Enums); i++ {
				e := strings.TrimSpace(op.Enums[i])
				if strings.Contains(e, "*") {
					e = strings.Replace(e, "*", "", -1)
					op.DefaultEnum = e
				}
				op.Enums[i] = e
			}
			if strings.Contains(op.DataType, "list<enum") {
				op.DataType = "list<enum>"
			} else {
				op.DataType = "enum"
			}
		}

		if op.OptionType == "note" {
			c.Notes = append(c.Notes, op.Description)
		} else if op.OptionType == "alias" {
			c.Aliases = append(c.Aliases, op.Description)
		} else if op.OptionType == "command" {
			c.Description = op.Description
		} else if op.OptionType == "group" {
			// c.Description = op.Description
		} else {
			op.cmdPtr = c
			cleaned = append(cleaned, op)
		}
	}
	c.Options = cleaned
}

func (op *Option) DescriptionEx() string {
	d := op.Description
	if op.IsRequired() {
		d += " (required)"
	}
	if op.IsHidden() {
		d += " (hidden)"
	}
	return d
}

var globals = []Option{
	{LongName: "create", HotKey: "", OptionType: "switch", Description: "create a new item", DataType: "string"},
	{LongName: "update", HotKey: "", OptionType: "switch", Description: "update an existing item", DataType: "string"},
	{LongName: "delete", HotKey: "", OptionType: "switch", Description: "delete the item, but do not remove it", DataType: "string"},
	{LongName: "undelete", HotKey: "", OptionType: "switch", Description: "undelete a previously deleted item", DataType: "string"},
	{LongName: "remove", HotKey: "", OptionType: "switch", Description: "remove a previously deleted item", DataType: "string"},
	{LongName: "chain", HotKey: "", OptionType: "flag", Description: "the chain to use", DataType: "string"},
	{LongName: "noHeader", HotKey: "", OptionType: "switch", Description: "suppress the header in the output", DataType: "boolean"},
	{LongName: "cache", HotKey: "o", OptionType: "switch", Description: "force the results of the query into the cache", DataType: "boolean"},
	{LongName: "decache", HotKey: "D", OptionType: "switch", Description: "removes related items from the cache", DataType: "boolean"},
	{LongName: "ether", HotKey: "H", OptionType: "switch", Description: "export values in ether", DataType: "boolean"},
	{LongName: "raw", HotKey: "w", OptionType: "switch", Description: "report raw data directly from the source", DataType: "boolean"},
	{LongName: "fmt", HotKey: "x", OptionType: "flag", Description: "export format, one of [ txt | csv | json ]", DataType: "string"},
}

func (c *Command) PyGlobals() string {
	ret := []string{}
	caps := strings.Replace(strings.Replace(strings.ToLower(c.Capabilities)+"|", "default|", "verbose|fmt|version|noop|nocolor|chain|noheader|file|output|append|", -1), "caching|", "cache|decache|", -1)
	if c.Route == "names" {
		caps = "create|update|delete|undelete|remove|" + caps
	}
	for _, op := range globals {
		if strings.Contains(caps, strings.ToLower(op.LongName)+"|") {
			tmplName := "pyglobals1"
			tmpl := "    \"{{toCamel .LongName}}\": {\"hotkey\": \"{{.PyHotKey}}\", \"type\": \"{{.OptionType}}\"},"
			ret = append(ret, op.executeTemplate(tmplName, tmpl))
		}
	}
	return strings.Join(ret, "\n")
}

func (c *Command) YamlGlobals() string {
	ret := []string{}
	caps := strings.Replace(strings.Replace(strings.ToLower(c.Capabilities)+"|", "default|", "verbose|fmt|version|noop|nocolor|chain|noheader|file|output|append|", -1), "caching|", "cache|decache|", -1)
	if c.Route == "names" {
		caps = "create|update|delete|undelete|remove|" + caps
	}
	for _, op := range globals {
		if strings.Contains(caps, strings.ToLower(op.LongName)+"|") {
			tmplName := "pyglobals2"
			tmpl := `        - name: {{toCamel .LongName}}
          description: {{.Description}}
          required: false
          style: form
          in: query
          explode: true
          schema:
            type: {{.DataType}}`
			ret = append(ret, op.executeTemplate(tmplName, tmpl))
		}
	}
	return strings.Join(ret, "\n")
}

func (c *Command) FirstPositional() string {
	for _, op := range c.Options {
		if op.IsPositional() {
			return op.LongName
		}
	}
	return ""
}

func (c *Command) PyOptions() string {
	ret := []string{}
	for _, op := range c.Options {
		if !op.IsPositional() && !op.IsHidden() {
			tmplName := "pyoption"
			tmpl := "    \"{{toCamel .LongName}}\": {\"hotkey\": \"{{.PyHotKey}}\", \"type\": \"{{.OptionType}}\"},"
			ret = append(ret, op.executeTemplate(tmplName, tmpl))
		}
	}
	return strings.Join(ret, "\n")
}

// Tags found in sdk_go_route.go

// Enums1 for tag {{.Enums1}}
func (c *Command) Enums1() string {
	if !c.HasEnums() {
		return "// No enums"
	}

	ret := ""
	for _, op := range c.Options {
		ret += op.Enum1()
	}
	return ret
}

// Enums2 for tag {{.Enums2}}
func (c *Command) Enums2() string {
	if !c.HasEnums() {
		return "// No enums"
	}

	ret := ""
	for _, op := range c.Options {
		ret += op.Enum2()
	}
	return ret
}

// Enums3 for tag {{.Enums3}}
func (c *Command) Enums3() string {
	if !c.HasEnums() {
		return "_"
	}
	return "opts"
}

// Pkg for tag {{.Pkg}}
func (c *Command) Pkg() string {
	if c.Route == "init" {
		// init is a GoLang reserved word
		return c.Route + "Pkg"
	}
	return c.Route
}

// AliasStr for tag {{.AliasStr}}}
func (c *Command) AliasStr() string {
	if len(c.Aliases) == 0 {
		return ""
	}
	ret := "Aliases: []string{"
	for _, alias := range c.Aliases {
		ret += "\"" + alias + "\",\n"
	}
	ret += "},"
	return ret
}

// AddCaps for tag {{.AddCaps}}
func (c *Command) AddCaps() string {
	ret := []string{}
	caps := strings.Split(c.Capabilities, "|")
	for _, cap := range caps {
		if len(cap) == 0 {
			continue
		}
		v := "\tcapabilities = capabilities.Add(caps." + strings.ToUpper(cap[0:1]) + cap[1:] + ")"
		ret = append(ret, v)
	}
	str := `var capabilities caps.Capability // capabilities for chifra [{ROUTE}]
[{CAPS}]
`
	return strings.Replace(strings.Replace(str, "[{CAPS}]", strings.Join(ret, "\n"), -1), "[{ROUTE}]", c.Route, -1)
}

// DefaultsApi for tag {{.DefaultsApi}}
func (c *Command) DefaultsApi() string {
	ret := []string{}
	hasConfig := false
	for _, op := range c.Options {
		hasConfig = hasConfig || op.IsConfig()
		v := op.DefaultApi()
		if len(v) > 0 {
			ret = append(ret, v)
		}
	}
	if hasConfig {
		ret = append(ret, "	configs := make(map[string]string, 10)")
	}
	if len(ret) == 0 {
		return ""
	}
	return strings.Join(ret, "\n") + "\n"
}

// EnsConvert1 for tag {{.EnsConvert1}}
func (c *Command) EnsConvert1() string {
	ret := []string{}
	if c.HasAddrs() {
		for _, op := range c.Options {
			if op.IsSpecialAddr() {
				v := op.EnsConvert()
				if len(v) > 0 {
					ret = append(ret, v)
				}
			}
		}
	}
	return strings.Join(ret, "\n") + "\n"
}

// EnsConvert2 for tag {{.EnsConvert2}}
func (c *Command) EnsConvert2() string {
	if !c.HasAddrs() {
		return ""
	}
	ret := []string{}
	for _, op := range c.Options {
		if !op.IsSpecialAddr() {
			v := op.EnsConvert()
			if len(v) > 0 {
				ret = append(ret, v)
			}
		}
	}
	if len(ret) == 0 {
		return ""
	}
	return strings.Join(ret, "\n") + "\n"
}

// GoDefs for tag {{.GoDefs}}
func (c *Command) GoDefs() string {
	ret := []string{}
	for _, op := range c.Options {
		v := op.GoDef()
		if len(v) > 0 {
			ret = append(ret, v)
		}
	}
	if len(ret) == 0 {
		return ""
	}
	return strings.Join(ret, "\n")
}

// OptFields for tag {{.OptFields}}
func (c *Command) OptFields() string {
	ret := []string{}
	for _, op := range c.Options {
		v := op.OptField()
		if len(v) > 0 {
			ret = append(ret, v)
		}
	}
	if c.Route == "scrape" {
		ret = append(ret, "Settings  config.ScrapeSettings `json:\"settings,omitempty\"`  // Configuration items for the scrape")
	}
	ret = append(ret, "Globals globals.GlobalOptions `json:\"globals,omitempty\"`  // The global options")
	ret = append(ret, "Conn *rpc.Connection `json:\"conn,omitempty\"` // The connection to the RPC server")
	ret = append(ret, "BadFlag error `json:\"badFlag,omitempty\"` // An error flag if needed")
	return strings.Join(ret, "\n") + "\n"
}

// RequestOpts for tag {{.RequestOpts}}
func (c *Command) RequestOpts() string {
	ret := []string{}
	for _, op := range c.Options {
		v := op.RequestOpt()
		if len(v) > 0 {
			ret = append(ret, v)
		}
	}
	if len(ret) == 0 {
		return ""
	}
	return strings.Join(ret, "\n") + "\n"
}

// TestLogs for tag {{.TestLogs}}
func (c *Command) TestLogs() string {
	ret := []string{}
	for _, op := range c.Options {
		v := op.TestLog()
		if len(v) > 0 {
			ret = append(ret, v)
		}
	}
	if c.Route == "scrape" {
		ret = append(ret, "opts.Settings.TestLog(opts.Globals.Chain, opts.Globals.TestMode)")
	}
	return strings.Join(ret, "\n") + "\n"
}

func (c *Command) PackageComments() string {
	docsPath := "src/dev_tools/goMaker/templates/readme-intros/" + c.Route + ".md"
	lines := file.AsciiFileToLines(docsPath)

	ret := []string{"// " + c.Route + "Pkg implements the chifra " + c.Route + " command.\n//"}
	for i := 0; i < len(lines); i++ {
		lines[i] = strings.ReplaceAll(lines[i], "{{.Route}}", c.Route)
		ret = append(ret, "// "+strings.ReplaceAll(lines[i], "`", ""))
	}
	return strings.Join(ret, "\n")
}

func (c *Command) GroupName() string {
	return LowerNoSpaces(c.Group)
}

func (c *Command) IsRoute() bool {
	if len(c.Route) == 0 {
		return false
	}

	excludedRoutes := map[string]bool{
		"daemon":  true,
		"explore": true,
	}
	return !excludedRoutes[c.Route]
}

func (c *Command) Example() string {
	contents := strings.Trim(file.AsciiFileToString("./src/dev_tools/goMaker/templates/api/examples/"+c.Route+".txt"), ws)
	contents = strings.Replace(contents, "\n", "\n                  ", -1)
	return strings.Trim(contents, ws) + "\n"
}

func (c *Command) ReadmeName() string {
	return c.Route + ".md"
}

func (c *Command) HelpIntro() string {
	thePath := "src/dev_tools/goMaker/templates/readme-intros/" + c.ReadmeName()
	tmplName := "helpIntro" + c.ReadmeName()
	tmpl := file.AsciiFileToString(thePath)
	return strings.Trim(c.executeTemplate(tmplName, tmpl), ws)
}

func (c *Command) HelpText() string {
	thePath := "src/dev_tools/goMaker/generated/" + c.ReadmeName() + ".tmp"
	defer os.Remove(thePath)
	utils.System("chifra " + c.Route + " --help 2>" + thePath)
	helpText := strings.Trim(file.AsciiFileToString(thePath), wss)
	if strings.Contains(helpText, "unknown") {
		logger.Fatal("Error: " + helpText)
	}
	return helpText
}

func (c *Command) HelpDataModels() string {
	ret := []string{"- none"}
	for i, production := range c.Productions {
		if i == 0 {
			ret = []string{}
		}
		lowerProd := strings.ToLower(production.Class)
		groupProd := c.TypeToGroup(lowerProd)
		ret = append(ret, "- ["+lowerProd+"](/data-model/"+groupProd+"/#"+lowerProd+")")
	}
	return strings.Join(ret, "\n")
}

func (c *Command) HelpLinks() string {
	tmplName := "Links"
	tmpl := ""
	if !c.IsRoute() {
		tmplName += "2"
		tmpl = `- no api for this command
- [source code](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/chifra/internal/{{.Route}})`
	} else {
		tmplName += "3"
		tmpl = `- [api docs](/api/#operation/{{.GroupName}}-{{.Route}})
- [source code](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/chifra/internal/{{.Route}})`
	}
	return c.executeTemplate(tmplName, tmpl)
}

func (c *Command) HelpNotes() string {
	thePath := "src/dev_tools/goMaker/templates/readme-intros/" + c.ReadmeName()
	thePath = strings.Replace(thePath, ".md", ".notes.md", -1)
	if file.FileExists(thePath) {
		tmplName := "Notes" + c.ReadmeName()
		tmpl := file.AsciiFileToString(thePath)
		return "\n\n" + strings.Trim(c.executeTemplate(tmplName, tmpl), ws)
	}
	return ""
}

func (c *Command) ReadmeFooter() string {
	thePath := "src/dev_tools/goMaker/templates/readme-intros/README.footer.md"
	return strings.Trim(file.AsciiFileToString(thePath), ws)
}

func (c *Command) executeTemplate(name, tmplCode string) string {
	return executeTemplate(c, "command", name, tmplCode)
}

func (c *Command) GroupMenu(reason string) string {
	if reason == "model" {
		return `
  data:
    parent: collections`
	} else if reason == "readme" {
		return `
  chifra:
    parent: commands`
	} else {
		logger.Fatal("Unknown reason for group menu:", reason)
		return ""
	}
}

func (c *Command) GroupTitle() string {
	return FirstUpper(Lower(c.Group))
}

func (c *Command) GroupAlias(reason string) string {
	if reason == "model" {
		return ""
	}
	return strings.ReplaceAll(`aliases:
 - "/docs/chifra/[{GN}]"
`, "[{GN}]", c.GroupName())
}

func getContents(fnIn string) string {
	fn := "./src/dev_tools/goMaker/" + fnIn + ".md"
	if !file.FileExists(fn) {
		logger.Fatal("Error: file does not exist: " + fn)
	}
	return file.AsciiFileToString(fn)
}

func (c *Command) GroupIntro(reason string) string {
	return getContents("templates/" + reason + "-groups/" + c.GroupName())
}

func (c *Command) GroupMarkdowns(reason, filter string) string {
	ret := []string{}
	if reason == "model" {
		sort.Slice(c.cbPtr.Structures, func(i, j int) bool {
			return c.cbPtr.Structures[i].Num() < c.cbPtr.Structures[j].Num()
		})
		for _, st := range c.cbPtr.Structures {
			if st.GroupName() == filter && st.Name() != "" {
				ret = append(ret, getContents("generated/model_"+st.Name()))
			}
		}
	} else if reason == "readme" {
		sort.Slice(c.cbPtr.Commands, func(i, j int) bool {
			return c.cbPtr.Commands[i].Num < c.cbPtr.Commands[j].Num
		})
		for _, cmd := range c.cbPtr.Commands {
			if cmd.GroupName() == filter && cmd.Route != "" {
				ret = append(ret, getContents("generated/readme_"+cmd.Route))
			}
		}
	} else {
		logger.Fatal("Error: unknown reason: " + reason)
	}
	return strings.Join(ret, "\n")
}

func (cb *CodeBase) GroupList(filter string) []Command {
	ret := []Command{}
	for _, c := range cb.Commands {
		if c.Route == "" && c.Group != "" {
			if filter == "" || c.GroupName() == filter {
				ret = append(ret, c)
			}
		}
	}
	return ret
}

func (c *Command) BaseTypes() string {
	filter := c.GroupName()
	typeMap := map[string]bool{}
	for _, st := range c.cbPtr.Structures {
		g := st.GroupName()
		if g == filter {
			for _, m := range st.Members {
				typeMap[m.Type] = true
			}
		}
	}
	ret := [][]string{}
	for _, t := range c.cbPtr.BaseTypes {
		if typeMap[t.Class] {
			ret = append(ret, []string{t.Class, t.DocDescr, t.DocNotes})
		}
	}

	return MarkdownTable([]string{"Type", "Description", "Notes"}, ret)
}

func (c *Command) ReturnTypes() string {
	present := map[string]bool{}
	ret := []string{}
	for _, op := range c.Options {
		if len(op.ReturnType) > 0 {
			if !present[op.RetType()] {
				if op.LongName == "mode" {
					ret = append(ret, op.EnumTypes()...)
				} else {
					ret = append(ret, op.RetType())
				}
			}
			present[op.RetType()] = true
		}
	}
	return strings.Join(ret, "|\n")
}

func (c *Command) HasSdkEndpoints() bool {
	for _, op := range c.Options {
		if len(op.ReturnType) > 0 {
			return true
		}
	}
	return false
}

func (c *Command) SdkEndpoints() string {
	ret := []string{}
	for _, op := range c.Options {
		if len(op.ReturnType) > 0 {
			v := op.SdkEndpoint()
			if len(v) > 0 {
				ret = append(ret, v)
			}
		}
	}
	return strings.Join(ret, "\n")
}

func (c *Command) FuzzerSwitches() string {
	if c.Route == "chunks" || c.ReturnType == "bool" {
		return "// Thing\n"
	}

	ret := []string{}
	for _, op := range c.Options {
		if len(op.ReturnType) > 0 {
			v := op.FuzzerSwitch()
			if len(v) > 0 {
				ret = append(ret, v)
			}
		}
	}

	ret = append(ret, `	default:
		ReportError(fn, opts, fmt.Errorf("unknown which: %s", which))
		return`)
	return strings.Join(ret, "\n")
}

func (c *Command) FuzzerInits() string {
	ret := `FuzzerInits tag

	`
	return ret
}
