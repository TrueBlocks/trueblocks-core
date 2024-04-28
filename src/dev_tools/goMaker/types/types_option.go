package types

import (
	"encoding/json"
	"strings"
)

type Option struct {
	Num           int      `json:"num" csv:"num"`
	Folder        string   `json:"folder" csv:"folder"`
	Group         string   `json:"group" csv:"group"`
	Route         string   `json:"route" csv:"route"`
	Tool          string   `json:"tool" csv:"tool"`
	LongName      string   `json:"longName" csv:"longName"`
	HotKey        string   `json:"hotKey" csv:"hotKey"`
	DefVal        string   `json:"def_val" csv:"def_val"`
	Attributes    string   `json:"attributes" csv:"attributes"`
	OptionType    string   `json:"option_type" csv:"option_type"`
	DataType      string   `json:"data_type" csv:"data_type"`
	ReturnType    string   `json:"return_type,omitempty" csv:"return_type"`
	Summary       string   `json:"summary,omitempty" csv:"summary"`
	Usage         string   `json:"usage,omitempty" csv:"usage"`
	Capabilities  string   `json:"capabilities,omitempty" csv:"capabilities"`
	Description   string   `json:"description,omitempty" csv:"description"`
	Enums         []string `json:"enums,omitempty"`
	DefaultEnum   string   `json:"default_enum,omitempty"`
	GoName        string   `json:"go_name"`
	GoType        string   `json:"go_type"`
	GoSdkName     string   `json:"go_sdk_name"`
	GoSdkType     string   `json:"go_sdk_type"`
	GoOptionsType string   `json:"go_options_type"`
	cmdPtr        *Command `json:"-" csv:"-"`
}

func (op *Option) IsRequired() bool {
	return strings.Contains(op.Attributes, "required")
}

func (op *Option) IsVisible() bool {
	return strings.Contains(op.Attributes, "visible")
}

func (op *Option) IsVisibleDocs() bool {
	return strings.Contains(op.Attributes, "docs")
}

func (op *Option) IsConfig() bool {
	return strings.Contains(op.Attributes, "config")
}

func (op *Option) IsEnum() bool {
	return strings.Contains(op.DataType, "enum")
}

func (op *Option) IsArray() bool {
	return strings.Contains(op.DataType, "list")
}

func (op Option) String() string {
	bytes, _ := json.MarshalIndent(op, "", "    ")
	return string(bytes)
}

func (op *Option) EnumChoices() string {
	ret := "One of "
	if op.IsArray() {
		ret = "One or more of "
	}
	return ret + "[ " + strings.Join(op.Enums, " | ") + " ]"
}

func (op Option) Validate() bool {
	return op.LongName != "--------"
}

func readCmdOption(op *Option, data *any) (bool, error) {
	op.Group = strings.Trim(op.Group, wss)
	op.Folder = strings.Trim(op.Folder, wss)
	op.Route = strings.Trim(op.Route, wss)
	op.Tool = strings.Trim(op.Tool, wss)
	op.LongName = strings.Trim(op.LongName, wss)
	op.HotKey = strings.Trim(op.HotKey, wss)
	op.DefVal = strings.Trim(op.DefVal, wss)
	op.Attributes = strings.Trim(op.Attributes, wss)
	op.OptionType = strings.Trim(op.OptionType, wss)
	op.DataType = strings.Trim(op.DataType, wss)
	op.ReturnType = strings.Trim(op.ReturnType, wss)
	op.Summary = strings.Trim(op.Summary, wss)
	op.Usage = strings.Trim(op.Usage, wss)
	op.Capabilities = strings.Trim(op.Capabilities, wss)
	op.Description = strings.Trim(op.Description, wss)

	op.Description = strings.ReplaceAll(op.Description, "&#44;", ",")
	op.Description = strings.ReplaceAll(op.Description, "&#39;", "'")
	op.Description = strings.ReplaceAll(op.Description, "`", "")

	op.DefVal = strings.Replace(op.DefVal, "NOPOS", "utils.NOPOS", -1)

	return true, nil
}

func (op *Option) IsHidden() bool {
	return !op.IsVisibleDocs() ||
		(!op.IsRequired() && !op.IsVisible() && op.IsVisibleDocs())
}

func (op *Option) toGoName() string {
	ret := op.LongName
	if len(op.LongName) >= 2 {
		ret = GoName(op.LongName)
		if op.IsConfig() {
			ret = "Settings." + ret
		}
	}
	return ret
}

func (op *Option) toGoType() string {
	dt := op.DataType
	if len(dt) >= 2 {
		isList := strings.HasPrefix(dt, "list")
		isEnum := strings.Contains(dt, "enum[")
		dt = strings.Replace(dt, "list", "", -1)
		if isEnum {
			dt = "<string>"
		}
		m := map[string]string{
			"<addr>":     "base.Address",
			"<address>":  "base.Address",
			"<blknum>":   "base.Blknum",
			"<boolean>":  "bool",
			"<double>":   "float64",
			"<fourbyte>": "base.Fourbyte",
			"<string>":   "string",
			"<topic>":    "base.Hash",
			"<tx_id>":    "base.Blknum",
			"<uint64>":   "uint64",
		}
		if isList {
			dt = "[]" + m[dt]
		} else {
			dt = m[dt]
		}
	}
	return dt
}

func (op *Option) toGoSdkName() string {
	ret := op.toGoName()
	if ret == "Blocks" {
		ret = "BlockIds"
	}
	if ret == "Transactions" {
		ret = "TransactionIds"
	}
	return ret
}

func (op *Option) toGoOptionsType() string {
	if strings.HasPrefix(op.DataType, "enum") {
		return "string"
	}
	if strings.HasPrefix(op.DataType, "list<enum") {
		return "[]string"
	}
	ret := op.toGoSdkType()
	ret = strings.Replace(ret, "base.Address", "string", -1)
	ret = strings.Replace(ret, "base.Blknum", "uint64", -1)
	return ret
}

func (op *Option) toGoSdkType() string {
	dt := op.DataType
	if len(dt) < 2 {
		return dt
	}

	isEnum := strings.Contains(dt, "enum[")
	if isEnum {
		return op.EnumName()
	}

	isList := strings.HasPrefix(dt, "list")
	dt = strings.Replace(dt, "list", "", -1)
	m := map[string]string{
		"<addr>":     "string",
		"<address>":  "base.Address",
		"<blknum>":   "base.Blknum",
		"<boolean>":  "bool",
		"<double>":   "float64",
		"<fourbyte>": "string",
		"<string>":   "string",
		"<topic>":    "string",
		"<tx_id>":    "string",
		"<uint64>":   "uint64",
	}
	if isList {
		m["<blknum>"] = "string"
		return "[]" + m[dt]
	}
	return m[dt]
}

func (op *Option) JsonTag() string {
	return "`json:\"" + CamelCase(op.LongName) + ",omitempty\"`"
}

func (op *Option) PyHotKey() string {
	if len(op.HotKey) == 0 {
		return ""
	}
	return "-" + op.HotKey
}

func (op *Option) CmdDefault() string {
	return strings.Replace(op.Default(), "utils.NOPOS", "0", -1)
}

func (op *Option) IsFlag() bool {
	return op.OptionType == "flag" || op.OptionType == "switch"
}

func (op *Option) IsAlias() bool {
	return op.OptionType == "alias"
}

func (op *Option) IsPositional() bool {
	return op.OptionType == "positional"
}

func (op *Option) Default() string {
	if op.IsArray() {
		return "nil"
	}
	if len(op.DefVal) == 0 {
		if op.DataType == "<boolean>" {
			return "false"
		}
		if op.DataType == "<double>" {
			return "0.0"
		}
		if op.DataType == "<uint64>" || op.DataType == "<blknum>" {
			return "0"
		}
		return "\"\""
	}
	if op.DefVal == "NOPOS" {
		return "0"
	}
	if op.DataType == "<string>" || op.DataType == "<address>" || op.DataType == "<topic>" || strings.Contains(op.DataType, "enum") {
		return "\"" + op.DefVal + "\""
	}
	return op.DefVal
}

func (op *Option) EnumName() string {
	if len(op.Route) < 2 {
		return ""
	}
	return strings.ToUpper(op.Route[0:1]) + op.Route[1:] + op.GoName
}

func (op *Option) EnumTag() string {
	if len(op.Route) < 2 || len(op.GoSdkName) < 2 {
		return ""
	}
	return strings.ToUpper(op.Route[0:1]) + op.GoSdkName[0:1]
}

func (op *Option) EnumNone() string {
	if len(op.Route) < 3 || len(op.GoSdkName) < 2 {
		return ""
	}
	tag := op.EnumTag()
	if tag == "CM" {
		// name clash otherwise
		tag = strings.ToUpper(op.Route[0:2]) + op.GoSdkName[0:1]
	}
	return "No" + tag
}

func (op *Option) EnumDef() string {
	tag := op.EnumTag()
	some := []string{}
	all := []string{}
	ret := []string{}
	for i, e := range op.Enums {
		token := tag + strings.ToUpper(e[0:1]) + e[1:]
		if e == "some" {
			if op.Route == "state" {
				some = []string{"SPBalance", "SPProxy", "SPDeployed", "SPAccttype"}
			}
			token += "=" + strings.Join(some, "|")
		} else if e == "all" {
			token += "=" + strings.Join(all, "|")
		} else {
			if len(ret) < 5 {
				some = append(some, token)
			}
			all = append(all, token)
			if i == 0 {
				ret = append(ret, "\t"+op.EnumNone()+" "+op.EnumName()+" = 0")
				token += " = 1 << iota"
			}
		}
		ret = append(ret, "\t"+token)
	}
	return strings.Join(ret, "\n")
}

func (op *Option) EnumMap() string {
	ret := []string{}
	for _, e := range op.Enums {
		if e == "some" || e == "all" {
			continue
		}
		v := "\t\t" + op.EnumTag() + strings.ToUpper(e[0:1]) + e[1:] + ": \"" + e + "\","
		ret = append(ret, v)
	}
	return strings.Join(ret, "\n")
}

func (op *Option) Lower() string {
	return strings.ToLower(op.LongName)
}

func (op *Option) EnumList() string {
	ret := []string{}
	for _, e := range op.Enums {
		if e == "some" || e == "all" {
			continue
		}
		ret = append(ret, op.EnumTag()+strings.ToUpper(e[0:1])+e[1:])
	}
	return op.EnumName() + "{" + strings.Join(ret, ", ") + "}"
}

func (op *Option) HasEnumAll() bool {
	for _, e := range op.Enums {
		if e == "all" {
			return true
		}
	}
	return false
}

func (op *Option) PreSwitch() string {
	if !op.HasEnumAll() {
		return ""
	}
	tmplName := "preSwitch"
	tmpl := `if len(values) == 1 && values[0] == "all" {
		return {{.EnumTag}}All, nil
	} else if len(values) == 1 && values[0] == "some" {
		return {{.EnumTag}}Some, nil
	}

`
	return op.executeTemplate(tmplName, tmpl)
}

func (op *Option) SomeCases() string {
	if !op.HasEnumAll() {
		return ""
	}
	tmplName := "someCases"
	tmpl := `	case {{.EnumTag}}Some:
		return "some"
	case {{.EnumTag}}All:
		return "all"
`
	return op.executeTemplate(tmplName, tmpl)
}

func (op *Option) EnumCases() string {
	ret := []string{}
	for _, e := range op.Enums {
		if e == "some" || e == "all" {
			continue
		}
		v := op.EnumTag() + strings.ToUpper(e[0:1]) + e[1:]
		ret = append(ret, "\t\tcase \""+e+"\":\n\t\t\tresult |= "+v)
	}
	v := "\t\tdefault:\n\t\t\treturn " + op.EnumNone() + ", fmt.Errorf(\"unknown " + op.LongName + ": %s\", val)"
	ret = append(ret, v)
	return strings.Join(ret, "\n")
}

func (op *Option) IsConfigurableAddr() bool {
	return op.GoName == "Publisher"
}

func (op *Option) IsSpecialAddr() bool {
	return op.GoName == "ProxyFor" || op.IsConfigurableAddr() || op.GoName == "Autoname"
}

func (op *Option) EnsConvert() string {
	ret := ""

	if op.DataType == "<address>" {
		tmplName := "ens"
		tmpl := "	opts.{{.GoName}}, _ = opts.Conn.GetEnsAddress(opts.{{.GoName}})"
		if op.IsConfigurableAddr() {
			tmplName += "1"
			tmpl = "	opts.{{.GoName}}, _ = opts.Conn.GetEnsAddress(config.Get{{.GoName}}(opts.{{.GoName}}))"
		}
		ret = op.executeTemplate(tmplName, tmpl)
		if op.IsSpecialAddr() {
			tmplName += "2"
			tmpl = `
	opts.{{.GoName}}Addr = base.HexToAddress(opts.{{.GoName}})`
			ret += op.executeTemplate(tmplName, tmpl)
		}
	} else if op.DataType == "list<addr>" {
		tmplName := "ensSlice"
		tmpl := "	opts.{{.GoName}}, _ = opts.Conn.GetEnsAddresses(opts.{{.GoName}})"
		ret = op.executeTemplate(tmplName, tmpl)
	}

	return ret
}

func (op *Option) DocType() string {
	if op.DataType == "bool" || op.DataType == "<boolean>" {
		return "boolean"
	} else if op.DataType == "list<addr>" {
		return `array
            items:
              type: string
              format: address`
	} else if op.DataType == "list<topic>" {
		return `array
            items:
              type: string
              format: topic`
	} else if op.DataType == "list<fourbyte>" {
		return `array
            items:
              type: string
              format: fourbyte`
	} else if op.DataType == "list<string>" {
		return `array
            items:
              type: string
              format: string`
	} else if op.DataType == "list<blknum>" {
		return `array
            items:
              type: string
              format: blknum`
	} else if op.DataType == "list<tx_id>" {
		return `array
            items:
              type: string
              format: tx_id`
	} else if op.DataType == "uint64" || op.DataType == "<uint64>" || op.DataType == "<blknum>" {
		return `number
            format: blknum`
	} else if op.DataType == "<string>" {
		return `string`
	} else if op.DataType == "<address>" {
		return `string
            format: address`
	} else if op.DataType == "<double>" {
		return `number
            format: double`
	} else if op.DataType == "enum" {
		ret := `string
            enum:
`
		for _, e := range op.Enums {
			ret += "              - " + e + "\n"
		}
		return strings.Trim(ret, "\n")
	} else if op.DataType == "list<enum>" {
		ret := `array
            items:
              type: string
              enum:
`
		for _, e := range op.Enums {
			if e == "1155" {
				e = "\"1155\""
			}
			ret += "                - " + e + "\n"
		}
		return strings.Trim(ret, "\n")
	}
	return op.DataType
}

func (op *Option) IsNullDefault2() bool {
	return op.IsNullDefault() || op.IsConfig()
}

func (op *Option) IsNullDefault() bool {
	if len(op.DefVal) == 0 ||
		op.DefVal == "0" ||
		op.DefVal == "0.0" ||
		op.DefVal == "false" ||
		op.DefVal == "nil" {
		return true
	}
	return false
}

// Enum1 for tag {{.Enum1}}
func (op *Option) Enum1() string {
	if !op.IsEnum() {
		return ""
	}
	tmplName := "enum1"
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
	return op.executeTemplate(tmplName, tmpl)
}

// Enum2 for tag {{.Enum2}}
func (op *Option) Enum2() string {
	if !op.IsEnum() {
		return ""
	}
	tmplName := "enum2"
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
	return op.executeTemplate(tmplName, tmpl)
}

func (op *Option) IsStringLike() bool {
	return strings.HasPrefix(op.DataType, "list") ||
		strings.HasPrefix(op.DataType, "enum") ||
		op.DataType == "<string>" ||
		op.DataType == "<address>"
}

func (op *Option) IsBool() bool {
	return op.DataType == "<boolean>"
}

func (op *Option) IsFloat() bool {
	return op.DataType == "<double>"
}

func (op *Option) TestLog() string {
	if op.IsConfig() {
		return ""
	}
	tmplName := "testLogs"
	tmpl := `	logger.TestLog(`
	if op.IsFloat() {
		tmplName += "1"
		tmpl += `opts.{{.GoName}} != float64({{.Default}})`

	} else if op.IsBool() {
		tmplName += "2"
		tmpl += `opts.{{.GoName}}`

	} else if op.IsStringLike() {
		tmplName += "3"
		tmpl += `len(opts.{{.GoName}}) > 0`
		if !op.IsNullDefault() {
			tmplName += "3.2"
			tmpl += ` && opts.{{.GoName}} != "{{.DefVal}}"`
		}

	} else {
		tmplName += "4"
		tmpl += `opts.{{.GoName}} != {{.Default}}`
		if strings.HasPrefix(op.LongName, "last") {
			if !op.IsNullDefault() {
				tmplName += "4.2"
				tmpl += ` && opts.{{.GoName}} != 0`
			}
		}
	}
	tmpl += `, "{{.GoName}}: ", opts.{{.GoName}})`

	return op.executeTemplate(tmplName, tmpl)
}

func (op *Option) CobraType() string {
	m := map[string]string{
		"<address>":    "String",
		"<blknum>":     "Uint64",
		"<boolean>":    "Bool",
		"<double>":     "Float64",
		"<string>":     "String",
		"<uint64>":     "Uint64",
		"enum":         "String",
		"list<addr>":   "StringSlice",
		"list<enum>":   "StringSlice",
		"list<string>": "StringSlice",
		"list<topic>":  "StringSlice",
	}
	return m[op.DataType]
}

func (op *Option) DefaultApi() string {
	if op.IsNullDefault() {
		return ""
	}
	if op.DataType == "<string>" || strings.HasPrefix(op.DataType, "enum") {
		tmplName := "defaultApi2"
		tmpl := `	opts.{{.GoName}} = "{{.DefVal}}"`
		return op.executeTemplate(tmplName, tmpl)
	} else {
		tmplName := "defaultApi"
		tmpl := `	opts.{{.GoName}} = {{.DefVal}}`
		return op.executeTemplate(tmplName, tmpl)
	}
}

func (op *Option) GoDef() string {
	if op.IsNullDefault2() {
		return ""
	}
	if op.DataType == "<string>" || strings.Contains(op.DataType, "enum") {
		tmplName := "goDefs2"
		tmpl := `	{{.GoName}}: "{{.DefVal}}",`
		return op.executeTemplate(tmplName, tmpl)
	} else {
		tmplName := "goDefs"
		tmpl := `	{{.GoName}}: {{.DefVal}},`
		return op.executeTemplate(tmplName, tmpl)
	}
}

func (op *Option) OptField() string {
	if strings.Contains(op.toGoName(), "Settings.") {
		return ""
	}
	tmplName := "optFields"
	tmpl := `	{{.GoName}} {{.GoOptionsType}} {{.JsonTag}} // {{.DescrCaps}}`
	ret := op.executeTemplate(tmplName, tmpl)
	if op.LongName == "blocks" {
		tmplName := "optFields3"
		tmpl := `
	{{.GoSdkName}} []identifiers.Identifier`
		ret += op.executeTemplate(tmplName, tmpl)
		ret += "`json:\"blockIds,omitempty\"`   // Block identifiers"
	} else if op.LongName == "transactions" {
		tmplName := "optFields4"
		tmpl := `
	{{.GoSdkName}} []identifiers.Identifier`
		ret += op.executeTemplate(tmplName, tmpl)
		ret += "`json:\"transactionIds,omitempty\"`   // Transaction identifiers"
	}
	return ret
}

func (op *Option) DescrCaps() string {
	return strings.ToUpper(op.Description[0:1]) + op.Description[1:]
}

func (op *Option) RequestOpt() string {
	var ret string
	if op.IsConfig() {
		tmplName := "requestOpts1"
		tmpl := `	case "{{toCamel .LongName}}":
		configs[key] = value[0]`
		ret = op.executeTemplate(tmplName, tmpl)
	} else {
		if strings.HasPrefix(op.DataType, "list") {
			tmplName := "requestOpts2"
			tmpl := `		case "{{toCamel .LongName}}":
			for _, val := range value {
				s := strings.Split(val, " ") // may contain space separated items
				opts.{{.GoName}} = append(opts.{{.GoName}}, s...)
			}`
			ret = op.executeTemplate(tmplName, tmpl)
		} else if op.DataType == "<boolean>" {
			tmplName := "requestOpts3"
			tmpl := `		case "{{toCamel .LongName}}":
			opts.{{.GoName}} = true`
			ret = op.executeTemplate(tmplName, tmpl)
		} else if op.DataType == "<uint64>" || op.DataType == "<blknum>" {
			tmplName := "requestOpts4"
			tmpl := `		case "{{toCamel .LongName}}":
			opts.{{.GoName}} = globals.ToUint64(value[0])`
			ret = op.executeTemplate(tmplName, tmpl)
		} else if op.DataType == "<double>" {
			tmplName := "requestOpts5"
			tmpl := `		case "{{toCamel .LongName}}":
			opts.{{.GoName}} = globals.ToFloat64(value[0])`
			ret = op.executeTemplate(tmplName, tmpl)
		} else {
			tmplName := "requestOpts6"
			tmpl := `		case "{{toCamel .LongName}}":
			opts.{{.GoName}} = value[0]`
			ret = op.executeTemplate(tmplName, tmpl)
		}
	}
	return strings.Replace(ret, "Settings.", "", -1)
}

func (op *Option) executeTemplate(name, tmplCode string) string {
	return executeTemplate(op, "option", name, tmplCode)
}

func (op *Option) RetType() string {
	v := op.ReturnType
	if v != "string" && v != "base.Address" && v != "bool" {
		v = "types." + FirstUpper(v)
	}
	return v
}

func (op *Option) ToolTurd() string {
	if op.Tool == "" {
		return ""
	}
	return "--" + Lower(op.Tool) + " "
}

func (op *Option) ToolParameters() string {
	if len(op.ReturnType) == 0 {
		return ""
	} else if strings.Contains(op.DataType, "string") {
		return "val string"
	} else {
		return ""
	}
}

func (op *Option) ToolAssignment() string {
	if len(op.ReturnType) == 0 {
		return ""
	} else if strings.Contains(op.DataType, "string") {
		return "val"
	} else {
		return "true"
	}
}

func (op *Option) SdkEndpointName() string {
	if len(op.ReturnType) == 0 {
		return ""
	} else if op.OptionType == "positional" {
		return ""
	} else {
		return op.GoName
	}
}

func (op *Option) SdkEndpoint() string {
	if len(op.ReturnType) == 0 {
		return ""
	}

	longName := FirstUpper(op.LongName)
	if op.OptionType == "positional" {
		longName = ""
	}
	opp := Option{
		Route:      FirstUpper(op.Route),
		Tool:       longName,
		ReturnType: op.RetType(),
		OptionType: op.OptionType,
		DataType:   op.DataType,
		GoName:     op.GoName,
	}
	tmplName := "returnTypes"
	tmpl := `	// {{.Route}}{{.SdkEndpointName}} implements the chifra {{toLower .Route}} {{.ToolTurd}}command.
func (opts *{{.Route}}Options) {{.Route}}{{.Tool}}({{.ToolParameters}}) ([]{{.ReturnType}}, *types.MetaData, error) {
	in := opts.toInternal()
	in.{{.SdkEndpointName}} = {{.ToolAssignment}}
	return query{{.Route}}[{{.ReturnType}}](in)
}
`
	if op.OptionType == "positional" {
		tmpl = strings.ReplaceAll(tmpl, `	in.{{.SdkEndpointName}} = {{.ToolAssignment}}
`, "")
		tmpl = strings.ReplaceAll(tmpl, "{{.ToolParameters}}", "")
		return opp.executeTemplate(tmplName+"x", tmpl)
	} else {
		return opp.executeTemplate(tmplName, tmpl)
	}
}

func (op *Option) IsPublicEndpoint() bool {
	return len(op.ReturnType) == 0 || op.IsPositional()
}
