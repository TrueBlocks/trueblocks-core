package main

import (
	"strings"
	"text/template"

	"github.com/bykof/gostradamus"
	"golang.org/x/text/cases"
	"golang.org/x/text/language"
)

type Command struct {
	Route       string          `json:"route"`
	Group       string          `json:"group"`
	Description string          `json:"description"`
	Endpoint    CmdLineEndpoint `json:"endpoint"`
	Options     []CmdLineOption `json:"options"`
	Notes1      []string        `json:"notes"`
	Aliases     []string        `json:"aliases"`
	Hidden      []string        `json:"hidden"`
	Proper      string          `json:"proper"`
	Lower       string          `json:"lower"`
	templates   map[string]*template.Template
}

func (c *Command) SdkFields() string {
	ret := ""
	for _, op := range c.Options {
		ret += op.SdkField()
	}
	ret += `	Globals
`
	return ret
}

func (c *Command) HasEnums() bool {
	for _, op := range c.Options {
		if op.IsEnum1 {
			return true
		}
	}
	return false
}

func (c *Command) Enums3() string {
	if !c.HasEnums() {
		return "_"
	}
	return "opts"
}

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

func (c *Command) Pkg() string {
	if c.Route == "init" {
		// name clash otherwise
		return c.Route + "Pkg"
	}
	return c.Route
}

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

func (c *Command) clean() {
	cleaned := []CmdLineOption{}
	notes := []string{}
	aliases := []string{}
	hiddens := []string{}
	for _, op := range c.Options {
		op.GoName1 = op.toGoName()
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
			op.IsEnum1 = true
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
		} else if op.Hidden() {
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
	c.Notes1 = notes
	c.Hidden = hiddens
	c.Aliases = aliases
}

func (c *Command) Year() int {
	now := gostradamus.Now()
	return now.Year()
}

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

func (c *Command) Short() string {
	return strings.ToLower(c.Endpoint.Description[0:1]) + strings.Replace(c.Endpoint.Description[1:], ".", "", -1)
}

func (c *Command) Long() string {
	return "Purpose:\n  " + c.Endpoint.Description
}

func (c *Command) OptDef() string {
	return ""
}

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

func (c *Command) NotesStr() string {
	if len(c.Notes1) == 0 {
		return ""
	}

	ret := `
Notes:
[{NOTES}]`
	notes := []string{}
	for _, note := range c.Notes1 {
		note = strings.Replace(note, "`", "", -1)
		note = strings.Replace(note, "&#39;", "'", -1)
		notes = append(notes, "  - "+note)
	}
	return strings.Replace(ret, "[{NOTES}]", strings.Join(notes, "\n"), -1)
}

func (c *Command) SetOptions() string {
	options := []string{}
	for _, op := range c.Options {
		options = append(options, op.SetOptions())
	}
	return strings.Join(options, "\n") + "\n"
}

func (c *Command) Usage() string {
	ret := c.Route + " " + c.Endpoint.Usage
	if len(c.Positionals()) > 0 {
		ret += `

Arguments:`
	}
	return ret
}

func (c *Command) Positionals() []string {
	ret := []string{}
	for _, op := range c.Options {
		if op.OptionType == "positional" {
			req := ""
			if op.IsRequired {
				req = " (required)"
			}
			item := "\n  " + op.LongName + " - " + op.Description + req
			if op.IsEnum1 {
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

func (c *Command) FirstPositional() string {
	for _, op := range c.Options {
		if op.OptionType == "positional" {
			return op.LongName
		}
	}
	return ""
}

func (c *Command) Use() string {
	return strings.Join(c.Positionals(), "")
}

func (c *Command) Capabilities() string {
	caps := strings.Split(c.Endpoint.Capabilities, "|")
	ret := []string{}
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

func (op *CmdLineOption) OptFields() string {
	if strings.Contains(op.toGoName(), "Settings.") {
		return ""
	}
	ret := op.executeTemplate("optFields", `	{{.GoName1}} {{.GoOptionsType}} {{.JsonTag}} // {{.DescrCaps}}`)
	if op.LongName == "blocks" {
		ret += "\n" + op.executeTemplate("optFields2", `	{{.GoSdkName}} []identifiers.Identifier`)
		ret += "`json:\"blockIds,omitempty\"`   // Block identifiers"
	} else if op.LongName == "transactions" {
		ret += "\n" + op.executeTemplate("optFields2", `	{{.GoSdkName}} []identifiers.Identifier`)
		ret += "`json:\"transactionIds,omitempty\"`   // Transaction identifiers"
	}
	return ret
}

func (c *Command) OptFields() string {
	ret := []string{}
	for _, op := range c.Options {
		ret = append(ret, op.OptFields())
	}
	ret = append(ret, "Globals globals.GlobalOptions `json:\"globals,omitempty\"`  // The global options")
	ret = append(ret, "Conn *rpc.Connection `json:\"conn,omitempty\"` // The connection to the RPC server")
	ret = append(ret, "BadFlag error `json:\"badFlag,omitempty\"` // An error flag if needed")
	return strings.Join(ret, "\n") + "\n"
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

func (op *CmdLineOption) GoDefs() string {
	if op.IsNullDefault() {
		return ""
	}
	if op.DataType == "<string>" || strings.Contains(op.DataType, "enum") {
		return op.executeTemplate("goDefs2", `	{{.GoName1}}: "{{.DefVal}}",`)
	}
	return op.executeTemplate("goDefs", `	{{.GoName1}}: {{.DefVal}},`)
}

func (c *Command) GoDefs() string {
	ret := []string{}
	for _, op := range c.Options {
		v := op.GoDefs()
		if len(v) > 0 {
			ret = append(ret, v)
		}
	}
	if len(ret) == 0 {
		return ""
	}
	return strings.Join(ret, "\n")
}

func (op *CmdLineOption) TestLog() string {
	tmpl := `	logger.TestLog(`
	if op.DataType == "<double>" {
		tmpl += `opts.{{.GoName1}} != float64({{.Default}})`
	} else if strings.HasPrefix(op.DataType, "list") ||
		strings.HasPrefix(op.DataType, "enum") ||
		op.DataType == "<string>" ||
		op.DataType == "<address>" {
		tmpl += `len(opts.{{.GoName1}}) > 0`
	} else if op.DataType == "<boolean>" {
		tmpl += `opts.{{.GoName1}}`
	} else {
		tmpl += `opts.{{.GoName1}} != {{.Default}}`
	}
	tmpl += `, "{{.GoName1}}: ", opts.{{.GoName1}})`
	return op.executeTemplate("testLogs", tmpl)
}

func (c *Command) TestLogs() string {
	ret := []string{}
	for _, op := range c.Options {
		ret = append(ret, op.TestLog())
	}
	return strings.Join(ret, "\n") + "\n"
}

func (op *CmdLineOption) DefaultApi() string {
	if op.IsNullDefault() {
		return ""
	}
	if op.DataType == "<string>" {
		return op.executeTemplate("defaultApi2", `	opts.{{.GoName1}} = "{{.DefVal}}"`)
	}
	return op.executeTemplate("defaultApi", `	opts.{{.GoName1}} = {{.DefVal}}`)
}

func (c *Command) DefaultsApi() string {
	ret := []string{}
	for _, op := range c.Options {
		v := op.DefaultApi()
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
	if strings.HasPrefix(op.DataType, "list") {
		tmpl += `
			for _, val := range value {
				s := strings.Split(val, " ") // may contain space separated items
				opts.{{.GoName1}} = append(opts.{{.GoName1}}, s...)
			}`
	} else if op.DataType == "<boolean>" {
		tmpl += `
			opts.{{.GoName1}} = true`
	} else if op.DataType == "<uint64>" || op.DataType == "<blknum>" {
		tmpl += `
			opts.{{.GoName1}} = globals.ToUint64(value[0])`
	} else if op.DataType == "<double>" {
		tmpl += `
			opts.{{.GoName1}} = globals.ToFloat64(value[0])`
	} else {
		tmpl += `
			opts.{{.GoName1}} = value[0]`
	}
	return op.executeTemplate("requestOpts", tmpl)
}

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

func (c *Command) HasAddrs() bool {
	for _, op := range c.Options {
		if op.DataType == "<address>" || op.DataType == "list<addr>" {
			return true
		}
	}
	return false
}

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
