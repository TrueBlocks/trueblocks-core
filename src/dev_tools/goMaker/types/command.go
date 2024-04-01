package types

import (
	"fmt"
	"os"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

type Command struct {
	Num           int          `json:"num" csv:"num"`
	Route         string       `json:"route,omitempty" csv:"route"`
	Group         string       `json:"group,omitempty" csv:"group"`
	Description   string       `json:"description,omitempty" csv:"description"`
	Endpoint      Endpoint     `json:"endpoint,omitempty" csv:"endpoint"`
	Options       []Option     `json:"options,omitempty" csv:"options"`
	Capabilities  string       `json:"capabilities,omitempty" csv:"capabilities"`
	Usage         string       `json:"usage,omitempty" csv:"usage"`
	Folder        string       `json:"folder,omitempty" csv:"folder"`
	Tool          string       `json:"tool,omitempty" csv:"tool"`
	Summary       string       `json:"summary,omitempty" csv:"summary"`
	Notes         []string     `json:"notes,omitempty" csv:"notes"`
	Aliases       []string     `json:"aliases,omitempty" csv:"aliases"`
	Hidden        []string     `json:"hidden,omitempty" csv:"hidden"`
	Productions   []Production `json:"productions,omitempty"`
	Proper        string       `json:"proper,omitempty"`
	Lower         string       `json:"lower,omitempty"`
	cbPtr         *CodeBase    `json:"-"`
}

func (c *Command) TypeToGroup(t string) string {
	return c.cbPtr.TypeToGroup(t)
}

func (c *Command) ProducedByDescr() string {
	g := strings.Replace(strings.ToLower(c.Group), " ", "", -1)
	types := []string{}
	for i, production := range c.Productions {
		lowerProd := strings.ToLower(production.Value)
		groupProd := c.TypeToGroup(lowerProd)
		if i > 0 {
			if i == len(c.Productions)-1 {
				types = append(types, " or ")
			} else {
				types = append(types, ", ")
			}
		}
		tmpl := fmt.Sprintf("<a href=\"/data-model/%s/#%s\">%s</a>", groupProd, lowerProd, production.Value)
		types = append(types, c.executeTemplate("produces"+production.Value, tmpl))
	}
	tmpl := fmt.Sprintf(" Corresponds to the <a href=\"/chifra/%s/#chifra-{{.Route}}\">chifra {{.Route}}</a> command line.", g)
	return "Produces " + strings.Join(types, "") + " data." + c.executeTemplate("corresponds"+c.Route, tmpl)
}

func (c *Command) ProducedByList() string {
	ret := []string{}
	if len(c.Productions) == 1 {
		camel := CamelCase(c.Productions[0].Value)
		ret = []string{fmt.Sprintf("                      $ref: \"#/components/schemas/%s\"", camel)}
	} else {
		ret = append(ret, "                      oneOf:")
		for _, production := range c.Productions {
			camel := CamelCase(production.Value)
			s := fmt.Sprintf("                        - $ref: \"#/components/schemas/%s\"", camel)
			ret = append(ret, s)
		}
	}
	return strings.Trim(strings.Join(ret, "\n"), "\r\n\t") + "\n"
}

func (c *Command) HasEnums() bool {
	for _, op := range c.Options {
		if op.IsEnum {
			return true
		}
	}
	return false
}

func (c *Command) Clean() {
	cleaned := []Option{}
	notes := []string{}
	aliases := []string{}
	hiddens := []string{}
	for _, op := range c.Options {
		op.GoName = op.toGoName()
		op.GoType = op.toGoType()
		op.GoSdkName = op.toGoSdkName()
		op.GoSdkType = op.toGoSdkType()
		op.GoOptionsType = op.toGoOptionsType()
		op.DefVal = strings.Replace(op.DefVal, "NOPOS", "utils.NOPOS", -1)
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
			op.IsArray = strings.Contains(op.DataType, "list")
			op.IsEnum = true
			if strings.Contains(op.DataType, "list<enum") {
				op.DataType = "list<enum>"
			} else {
				op.DataType = "enum"
			}
		}

		if op.OptionType == "note" {
			notes = append(notes, op.Description)
		} else if op.OptionType == "alias" {
			aliases = append(aliases, op.Description)
		} else if op.OptionType == "description" {
			c.Description = op.Description
		} else if op.IsHidden() {
			op.cmdPtr = c
			cleaned = append(cleaned, op)
			hiddens = append(hiddens, op.LongName)
		} else {
			op.cmdPtr = c
			cleaned = append(cleaned, op)
		}
	}
	c.Proper = Proper(c.Route)
	c.Lower = strings.ToLower(c.Route)
	c.Endpoint.cmdPtr = c
	c.Options = cleaned
	c.Notes = notes
	c.Hidden = hiddens
	c.Aliases = aliases
}

func (c *Command) Positionals() []string {
	ret := []string{}
	for _, op := range c.Options {
		if op.IsPositional() {
			req := ""
			if op.IsRequired {
				req = " (required)"
			}
			item := "\n  " + op.LongName + " - " + op.Description + req
			if op.IsEnum {
				e := "\n\tOne of "
				if op.IsArray {
					e = "\n\tOne or more of "
				}
				e += "[ " + strings.Join(op.Enums, " | ") + " ]"
				item += e
			}
			ret = append(ret, item)
		}
	}
	return ret
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
	caps := strings.Replace(strings.Replace(strings.ToLower(c.Endpoint.Capabilities)+"|", "default|", "verbose|fmt|version|noop|nocolor|chain|noheader|file|output|append|", -1), "caching|", "cache|decache|", -1)
	if c.Route == "names" {
		caps = "create|update|delete|undelete|remove|" + caps
	}
	for _, op := range globals {
		if strings.Contains(caps, strings.ToLower(op.LongName)+"|") {
			tmplName := "pyglobals1"
			tmpl := "    \"{{.CamelCase}}\": {\"hotkey\": \"{{.PyHotKey}}\", \"type\": \"{{.OptionType}}\"},"
			ret = append(ret, op.executeTemplate(tmplName, tmpl))
		}
	}
	return strings.Join(ret, "\n")
}

func (c *Command) YamlGlobals() string {
	ret := []string{}
	caps := strings.Replace(strings.Replace(strings.ToLower(c.Endpoint.Capabilities)+"|", "default|", "verbose|fmt|version|noop|nocolor|chain|noheader|file|output|append|", -1), "caching|", "cache|decache|", -1)
	if c.Route == "names" {
		caps = "create|update|delete|undelete|remove|" + caps
	}
	for _, op := range globals {
		if strings.Contains(caps, strings.ToLower(op.LongName)+"|") {
			tmplName := "pyglobals2"
			tmpl := `        - name: {{.CamelCase}}
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

func (c *Command) HasAddrs() bool {
	for _, op := range c.Options {
		if op.DataType == "<address>" || op.DataType == "list<addr>" {
			return true
		}
	}
	return false
}

func (c *Command) PyOptions() string {
	ret := []string{}
	for _, op := range c.Options {
		if !op.IsPositional() && !op.IsHidden() {
			tmplName := "pyoption"
			tmpl := "    \"{{.CamelCase}}\": {\"hotkey\": \"{{.PyHotKey}}\", \"type\": \"{{.OptionType}}\"},"
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

// SdkFields for tag {{.SdkFields}}}
func (c *Command) SdkFields() string {
	ret := ""
	for _, op := range c.Options {
		ret += op.SdkField()
	}
	ret += `	Globals
`
	return ret
}

// No unique tags found in apps_chifra_sdk_route.go
// No unique tags found in apps_chifra_internal_route_output.go
// No unique tags found in src_apps_chifra_cmd_route.go.go

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
	caps := strings.Split(c.Endpoint.Capabilities, "|")
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

// HiddenStr for tag {{.HiddenStr}}
func (c *Command) HiddenStr() string {
	if len(c.Hidden) == 0 {
		return ""
	}
	hiddens := []string{}
	for _, hidden := range c.Hidden {
		h := "\t_ = " + c.Route + "Cmd.Flags().MarkHidden(\"" + hidden + "\")"
		hiddens = append(hiddens, "	"+h)
	}
	ret := `	if os.Getenv("TEST_MODE") != "true" {
[{LIST}]
	}
`
	return strings.Replace(ret, "[{LIST}]", strings.Join(hiddens, "\n"), -1)
}

// Long for tag {{.Long}}
func (c *Command) Long() string {
	return "Purpose:\n  " + c.Endpoint.Description
}

// NotesStr for tag {{.NotesStr}}
func (c *Command) NotesStr() string {
	if len(c.Notes) == 0 {
		return ""
	}

	ret := `
Notes:
[{NOTES}]`
	notes := []string{}
	for _, note := range c.Notes {
		note = strings.Replace(note, "`", "", -1)
		note = strings.Replace(note, "&#39;", "'", -1)
		notes = append(notes, "  - "+note)
	}
	return strings.Replace(ret, "[{NOTES}]", strings.Join(notes, "\n"), -1)
}

// OptDef for tag {{.OptDef}}
func (c *Command) OptDef() string {
	return ""
}

// SetOptions for tag {{.SetOptions}}
func (c *Command) SetOptions() string {
	options := []string{}
	for _, op := range c.Options {
		options = append(options, op.SetOption())
	}
	return strings.Join(options, "\n") + "\n"
}

// Short for tag {{.Short}}"
func (c *Command) Short() string {
	return FirstLower(strings.Replace(c.Endpoint.Description, ".", "", -1))
}

// UsageStr for tag {{.UsageStr}}
func (c *Command) UsageStr() string {
	ret := c.Route + " " + c.Endpoint.Usage
	if len(c.Positionals()) > 0 {
		ret += `

Arguments:`
	}
	return ret
}

// Use for tag {{.Use}}
func (c *Command) Use() string {
	return strings.Join(c.Positionals(), "")
}

// src_apps_chifra_internal_route_options.go
// DefaultsApi for tag {{.DefaultsApi}}
func (c *Command) DefaultsApi() string {
	ret := []string{}
	hasConfig := false
	for _, op := range c.Options {
		hasConfig = hasConfig || op.Generate == "config"
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
	if !c.HasAddrs() {
		return ""
	}
	ret := []string{}
	for _, op := range c.Options {
		if op.IsSpecialAddr() {
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

func (c *Command) ReturnType() string {
	switch c.Route {
	case "blocks":
		return "types.SimpleBlock[string]"
	case "transactions":
		return "types.SimpleTransaction"
	case "receipts":
		return "types.SimpleReceipt"
	case "logs":
		return "types.SimpleLog"
	case "traces":
		return "types.SimpleTrace"
	case "when":
		return "types.SimpleNamedBlock"
	case "state":
		return "types.SimpleState"
	case "tokens":
		return "bool"
	case "slurp":
		return "types.SimpleSlurp"
	case "names":
		return "types.SimpleName"
	case "abis":
		return "bool"
	case "list":
		return "types.SimpleAppearance"
	case "export":
		return "bool"
	case "monitors":
		return "bool"
	case "config":
		return "bool"
	case "status":
		return "bool"
	case "chunks":
		return "bool"
	case "init":
		return "bool"
	default:
		return "bool"
	}
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

func (c *Command) PkgDoc() string {
	docsPath := "src/dev_tools/goMaker/templates/readme-intros/" + c.Route + ".md"
	docsPath = strings.ReplaceAll(docsPath, " ", "")
	contents := file.AsciiFileToString(docsPath)
	contents = strings.ReplaceAll(contents, "`", "")
	contents = strings.ReplaceAll(contents, "\n", " ")
	contents = strings.ReplaceAll(contents, "  ", " ")
	contents = strings.ReplaceAll(contents, "{{.Route}}", c.Route)
	sentences := strings.Split(contents, ".")
	return "// " + strings.Join(sentences, ".")
}

func (c *Command) LowerGroup() string {
	return strings.Replace(strings.ToLower(c.Group), " ", "", -1)
}

func (c *Command) IsRoute() bool {
	return len(c.Route) > 0 && c.Route != "daemon" && c.Route != "explore"
}

func (c *Command) HasExample() bool {
	return file.FileExists("./src/dev_tools/goMaker/templates/api/examples/" + c.Route + ".txt")
}

func (c *Command) Example() string {
	contents := strings.Trim(file.AsciiFileToString("./src/dev_tools/goMaker/templates/api/examples/"+c.Route+".txt"), "\n\r\t")
	contents = strings.Replace(contents, "\n", "\n                  ", -1)
	return strings.Trim(contents, "\n\r\t") + "\n"
}

func (c *Command) ReadmeName() string {
	return c.Route + ".md"
}

func (c *Command) HelpIntro() string {
	thePath := "src/dev_tools/goMaker/templates/readme-intros/" + c.ReadmeName()
	tmplName := "helpIntro" + c.ReadmeName()
	tmpl := file.AsciiFileToString(thePath)
	return strings.Trim(c.executeTemplate(tmplName, tmpl), "\r\n\t")
}

func (c *Command) HelpText() string {
	thePath := "src/dev_tools/goMaker/templates/readmes/" + c.ReadmeName() + ".tmp"
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
		lowerProd := strings.ToLower(production.Value)
		groupProd := c.TypeToGroup(lowerProd)
		ret = append(ret, "- ["+lowerProd+"](/data-model/"+groupProd+"/#"+lowerProd+")")
	}
	return strings.Join(ret, "\n")
}

func (c *Command) HelpLinks() string {
	tmplName := "Links"
	tmpl := ""
	if c.Route == "daemon" {
		tmplName += "1"
		tmpl = `- no api for this command
- [source code](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/chifra/internal/{{.Route}})
- no tests for this command`
	} else if c.Route == "explore" {
		tmplName += "2"
		tmpl = `- no api for this command
- [source code](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/chifra/internal/{{.Route}})
- [tests](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/dev_tools/testRunner/testCases/{{.Endpoint.Folder}}/{{.Endpoint.Tool}}.csv)`
	} else {
		tmplName += "3"
		tmpl = `- [api docs](/api/#operation/{{.LowerGroup}}-{{.Route}})
- [source code](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/chifra/internal/{{.Route}})
- [tests](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/dev_tools/testRunner/testCases/{{.Endpoint.Folder}}/{{.Endpoint.Tool}}.csv)`
	}
	return c.executeTemplate(tmplName, tmpl)
}

func (c *Command) HelpNotes() string {
	thePath := "src/dev_tools/goMaker/templates/readme-intros/" + c.ReadmeName()
	thePath = strings.Replace(thePath, ".md", ".notes.md", -1)
	if file.FileExists(thePath) {
		tmplName := "Notes" + c.ReadmeName()
		tmpl := file.AsciiFileToString(thePath)
		return "\n\n" + strings.Trim(c.executeTemplate(tmplName, tmpl), "\r\n\t")
	}
	return ""
}

func (c *Command) ReadmeFooter() string {
	thePath := "src/dev_tools/goMaker/templates/readme-intros/README.footer.md"
	return "\n" + strings.Trim(file.AsciiFileToString(thePath), "\n\r\t")
}
