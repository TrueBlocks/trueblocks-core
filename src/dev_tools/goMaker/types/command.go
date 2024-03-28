package types

import (
	"strings"

	"golang.org/x/text/cases"
	"golang.org/x/text/language"
)

type Command struct {
	Route       string          `json:"route" csv:"route"`
	Group       string          `json:"group" csv:"group"`
	Description string          `json:"description" csv:"description"`
	Endpoint    CmdLineEndpoint `json:"endpoint" csv:"endpoint"`
	Options     []CmdLineOption `json:"options" csv:"options"`
	Notes       []string        `json:"notes" csv:"notes"`
	Aliases     []string        `json:"aliases" csv:"aliases"`
	Hidden      []string        `json:"hidden" csv:"hidden"`
	Proper      string          `json:"proper" csv:"proper"`
	Lower       string          `json:"lower" csv:"lower"`
	templates   TemplateMap     `json:"-" csv:"-"`
}

func (c *Command) HasEnums() bool {
	for _, op := range c.Options {
		if op.IsEnum {
			return true
		}
	}
	return false
}

func (c *Command) clean() {
	cleaned := []CmdLineOption{}
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
			op.cmd = c
			cleaned = append(cleaned, op)
			hiddens = append(hiddens, op.LongName)
		} else {
			op.cmd = c
			cleaned = append(cleaned, op)
		}
	}
	titleCaser := cases.Title(language.English)
	c.Proper = titleCaser.String(c.Route)
	c.Lower = strings.ToLower(c.Route)
	c.Endpoint.cmd = c
	c.Options = cleaned
	c.Notes = notes
	c.Hidden = hiddens
	c.Aliases = aliases
}

func (op *CmdLineOption) IsPositional() bool {
	return op.OptionType == "positional"
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

var globals = []CmdLineOption{
	{LongName: "create", HotKey: "", OptionType: "switch"},
	{LongName: "update", HotKey: "", OptionType: "switch"},
	{LongName: "delete", HotKey: "", OptionType: "switch"},
	{LongName: "undelete", HotKey: "", OptionType: "switch"},
	{LongName: "remove", HotKey: "", OptionType: "switch"},
	{LongName: "chain", HotKey: "", OptionType: "flag"},
	{LongName: "noHeader", HotKey: "", OptionType: "switch"},
	{LongName: "cache", HotKey: "o", OptionType: "switch"},
	{LongName: "decache", HotKey: "D", OptionType: "switch"},
	{LongName: "ether", HotKey: "H", OptionType: "switch"},
	{LongName: "raw", HotKey: "w", OptionType: "switch"},
	{LongName: "fmt", HotKey: "x", OptionType: "flag"},
}

func (c *Command) PyGlobals() string {
	ret := []string{}
	caps := strings.Replace(strings.Replace(strings.ToLower(c.Endpoint.Capabilities)+"|", "default|", "verbose|fmt|version|noop|nocolor|chain|noheader|file|output|append|", -1), "caching|", "cache|decache|", -1)
	if c.Route == "names" {
		caps = "create|update|delete|undelete|remove|" + caps
	}
	for _, op := range globals {
		if strings.Contains(caps, strings.ToLower(op.LongName)+"|") {
			code := "    \"{{.SnakeCase}}\": {\"hotkey\": \"{{.PyHotKey}}\", \"type\": \"{{.OptionType}}\"},"
			ret = append(ret, op.executeTemplate("pyglobals", code))
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

func (op *CmdLineOption) IsNullDefault2() bool {
	return op.IsNullDefault() || op.Generate == "config"
}

func (op *CmdLineOption) IsNullDefault() bool {
	if len(op.DefVal) == 0 ||
		op.DefVal == "0" ||
		op.DefVal == "0.0" ||
		op.DefVal == "false" ||
		op.DefVal == "nil" {
		return true
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

func (c *Command) PyOptions() string {
	ret := []string{}
	for _, op := range c.Options {
		if !op.IsPositional() && !op.IsHidden() {
			code := "    \"{{.SnakeCase}}\": {\"hotkey\": \"{{.PyHotKey}}\", \"type\": \"{{.OptionType}}\"},"
			ret = append(ret, op.executeTemplate("pyoption", code))
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

// Enum1 for tag {{.Enum1}}
func (op *CmdLineOption) Enum1() string {
	if !op.IsEnum {
		return ""
	}
	tmpl := `	if key == "{{.LongName}}" {
		var err error
		values := strings.Split(value, ",")
		if opts.{{.GoName}}, err = enumFrom{{.EnumName}}(values); err != nil {
			return false, err
		} else {
			found = true
		}
	}
	`
	return op.executeTemplate("enum1", tmpl)
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

// Enum2 for tag {{.Enum2}}
func (op *CmdLineOption) Enum2() string {
	if !op.IsEnum {
		return ""
	}
	tmpl := `type {{.EnumName}} int

const (
{{.EnumDef}}
)

func (v {{.EnumName}}) String() string {
	switch v {
	case {{.EnumNone}}:
		return "none"
{{.SomeCases}}	}

	var m = map[{{.EnumName}}]string{
{{.EnumMap}}
	}

	var ret []string
	for _, val := range []{{.EnumList}} {
		if v&val != 0 {
			ret = append(ret, m[val])
		}
	}

	return strings.Join(ret, ",")
}

func enumFrom{{.EnumName}}(values []string) ({{.EnumName}}, error) {
	if len(values) == 0 {
		return {{.EnumNone}}, fmt.Errorf("no value provided for {{.Lower}} option")
	}

{{.PreSwitch}}	var result {{.EnumName}}
	for _, val := range values {
		switch val {
{{.EnumCases}}
		}
	}

	return result, nil
}

`
	return op.executeTemplate("enum2", tmpl)
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

// SdkField for tag {{.SdkField}}}
func (op *CmdLineOption) SdkField() string {
	tmpl := `	{{.GoSdkName}} {{.GoSdkType}} {{.JsonTag}}
`
	return op.executeTemplate("sdkField", tmpl)
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

// SetOption for tag {{.SetOption}}
func (op *CmdLineOption) SetOption() string {
	ret := ""
	if !op.IsPositional() && op.OptionType != "alias" {
		tmpl := `[{ROUTE}]Cmd.Flags().{{.CobraType}}VarP(&[{ROUTE}]Pkg.GetOptions().{{.GoName}}, "{{.LongName}}", "{{.HotKey}}", {{.CmdDefault}}, {{.CobraDescription}})`
		ret = op.executeTemplate("setOptions", tmpl)
	}
	ret = strings.ReplaceAll(ret, "[{ROUTE}]", op.ApiRoute)
	return ret
}

func (op *CmdLineOption) CobraType() string {
	m := map[string]string{
		"<string>":     "String",
		"<address>":    "String",
		"<uint64>":     "Uint64",
		"<blknum>":     "Uint64",
		"<double>":     "Float64",
		"list<addr>":   "StringSlice",
		"list<topic>":  "StringSlice",
		"list<string>": "StringSlice",
		"list<enum>":   "StringSlice",
		"enum":         "String",
		"<boolean>":    "Bool",
	}
	return m[op.DataType]
}

func (op *CmdLineOption) CobraDescription() string {
	d := op.Description
	if op.IsHidden() {
		d += " (hidden)"
	}
	if op.IsEnum {
		e := "One of "
		if op.IsArray {
			e = "One or more of "
		}
		e += "[ " + strings.Join(op.Enums, " | ") + " ]"
		return "`" + d + "\n" + e + "`"
	}
	return "\"" + d + "\""
}

// Short for tag {{.Short}}"
func (c *Command) Short() string {
	return strings.ToLower(c.Endpoint.Description[0:1]) + strings.Replace(c.Endpoint.Description[1:], ".", "", -1)
}

// Usage for tag {{.Usage}}
func (c *Command) Usage() string {
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

func (op *CmdLineOption) DefaultApi() string {
	if op.IsNullDefault() {
		return ""
	}
	if op.DataType == "<string>" || strings.HasPrefix(op.DataType, "enum") {
		return op.executeTemplate("defaultApi2", `	opts.{{.GoName}} = "{{.DefVal}}"`)
	}
	return op.executeTemplate("defaultApi", `	opts.{{.GoName}} = {{.DefVal}}`)
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

func (op *CmdLineOption) GoDef() string {
	if op.IsNullDefault2() {
		return ""
	}
	if op.DataType == "<string>" || strings.Contains(op.DataType, "enum") {
		return op.executeTemplate("goDefs2", `	{{.GoName}}: "{{.DefVal}}",`)
	}
	return op.executeTemplate("goDefs", `	{{.GoName}}: {{.DefVal}},`)
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

func (op *CmdLineOption) OptField() string {
	if strings.Contains(op.toGoName(), "Settings.") {
		return ""
	}
	ret := op.executeTemplate("optFields", `	{{.GoName}} {{.GoOptionsType}} {{.JsonTag}} // {{.DescrCaps}}`)
	if op.LongName == "blocks" {
		ret += "\n" + op.executeTemplate("optFields2", `	{{.GoSdkName}} []identifiers.Identifier`)
		ret += "`json:\"blockIds,omitempty\"`   // Block identifiers"
	} else if op.LongName == "transactions" {
		ret += "\n" + op.executeTemplate("optFields2", `	{{.GoSdkName}} []identifiers.Identifier`)
		ret += "`json:\"transactionIds,omitempty\"`   // Transaction identifiers"
	}
	return ret
}

func (op *CmdLineOption) DescrCaps() string {
	return strings.ToUpper(op.Description[0:1]) + op.Description[1:]
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

func (op *CmdLineOption) RequestOpt() string {
	tmpl := `		case "{{.SnakeCase}}":`
	if op.Generate == "config" {
		tmpl = `	case "{{.SnakeCase}}":
		configs[key] = value[0]`
	} else {
		if strings.HasPrefix(op.DataType, "list") {
			tmpl += `
			for _, val := range value {
				s := strings.Split(val, " ") // may contain space separated items
				opts.{{.GoName}} = append(opts.{{.GoName}}, s...)
			}`
		} else if op.DataType == "<boolean>" {
			tmpl += `
			opts.{{.GoName}} = true`
		} else if op.DataType == "<uint64>" || op.DataType == "<blknum>" {
			tmpl += `
			opts.{{.GoName}} = globals.ToUint64(value[0])`
		} else if op.DataType == "<double>" {
			tmpl += `
			opts.{{.GoName}} = globals.ToFloat64(value[0])`
		} else {
			tmpl += `
			opts.{{.GoName}} = value[0]`
		}
	}
	return strings.Replace(op.executeTemplate("requestOpts", tmpl), "Settings.", "", -1)
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

func (op *CmdLineOption) IsStringLike() bool {
	return strings.HasPrefix(op.DataType, "list") ||
		strings.HasPrefix(op.DataType, "enum") ||
		op.DataType == "<string>" ||
		op.DataType == "<address>"
}

func (op *CmdLineOption) IsBool() bool {
	return op.DataType == "<boolean>"
}

func (op *CmdLineOption) IsFloat() bool {
	return op.DataType == "<double>"
}

func (op *CmdLineOption) TestLog() string {
	if op.Generate == "config" {
		return ""
	}
	tmpl := `	logger.TestLog(`
	if op.IsFloat() {
		tmpl += `opts.{{.GoName}} != float64({{.Default}})`

	} else if op.IsBool() {
		tmpl += `opts.{{.GoName}}`

	} else if op.IsStringLike() {
		tmpl += `len(opts.{{.GoName}}) > 0`
		if !op.IsNullDefault() {
			tmpl += ` && opts.{{.GoName}} != "{{.DefVal}}"`
		}

	} else {
		tmpl += `opts.{{.GoName}} != {{.Default}}`
		if strings.HasPrefix(op.LongName, "last") {
			if !op.IsNullDefault() {
				tmpl += ` && opts.{{.GoName}} != 0`
			}
		}
	}

	tmpl += `, "{{.GoName}}: ", opts.{{.GoName}})`

	return op.executeTemplate("testLogs", tmpl)
}

func (cmd *Command) LowerGroup() string {
	return strings.ToLower(cmd.Group)
}
