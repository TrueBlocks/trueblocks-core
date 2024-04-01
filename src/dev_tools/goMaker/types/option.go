package types

import (
	"encoding/json"
	"strings"
)

type Option struct {
	Num            string   `json:"num" csv:"num"`
	Folder         string   `json:"folder" csv:"folder"`
	Tags           string   `json:"tags" csv:"tags"`
	ApiRoute       string   `json:"api_route" csv:"api_route"`
	Tool           string   `json:"tool" csv:"tool"`
	LongName       string   `json:"longName" csv:"longName"`
	HotKey         string   `json:"hotKey" csv:"hotKey"`
	DefVal         string   `json:"def_val" csv:"def_val"`
	IsRequired     bool     `json:"is_required" csv:"is_required"`
	IsCustomizable bool     `json:"is_customizable" csv:"is_customizable"`
	IsVisible      bool     `json:"is_visible" csv:"is_visible"`
	IsVisibleDocs  bool     `json:"is_visible_docs" csv:"is_visible_docs"`
	Generate       string   `json:"generate" csv:"generate"`
	OptionType     string   `json:"option_type" csv:"option_type"`
	DataType       string   `json:"data_type" csv:"data_type"`
	Description    string   `json:"description" csv:"description"`
	IsArray        bool     `json:"is_array" csv:"is_array"`
	IsEnum         bool     `json:"is_enum" csv:"is_enum"`
	Enums          []string `json:"enums" csv:"enums"`
	DefaultEnum    string   `json:"default_enum" csv:"default_enum"`
	GoName         string   `json:"go_name" csv:"go_name"`
	GoType         string   `json:"go_type" csv:"go_type"`
	GoSdkName      string   `json:"go_sdk_name" csv:"go_sdk_name"`
	GoSdkType      string   `json:"go_sdk_type" csv:"go_sdk_type"`
	GoOptionsType  string   `json:"go_options_type" csv:"go_options_type"`
	cmdPtr         *Command `json:"-" csv:"-"`
}

func (op Option) String() string {
	bytes, _ := json.MarshalIndent(op, "", "    ")
	return string(bytes)
}

func (op *Option) SnakeCase() string {
	return SnakeCase(op.LongName)
}

func (op Option) Validate() bool {
	return op.Tags != "Dev"
}

func readCmdOption(op *Option, data *any) (bool, error) {
	op.Description = strings.ReplaceAll(op.Description, "&#44;", ",")
	return true, nil
}

func (op *Option) IsHidden() bool {
	return !op.IsVisibleDocs ||
		(!op.IsRequired && !op.IsCustomizable && !op.IsVisible && op.IsVisibleDocs)
}

func (op *Option) toGoName() string {
	ret := op.LongName
	if len(op.LongName) >= 2 {
		ret = strings.ToUpper(op.LongName[0:1]) + SnakeCase(op.LongName)[1:]
		if op.Generate == "config" {
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
	return "`json:\"" + op.SnakeCase() + ",omitempty\"`"
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

func (op *Option) IsPositional() bool {
	return op.OptionType == "positional"
}

func (op *Option) Default() string {
	if op.IsArray || strings.HasPrefix(op.DataType, "list") {
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
	if len(op.ApiRoute) < 2 {
		return ""
	}
	return strings.ToUpper(op.ApiRoute[0:1]) + op.ApiRoute[1:] + op.GoName
}

func (op *Option) EnumTag() string {
	if len(op.ApiRoute) < 2 || len(op.GoSdkName) < 2 {
		return ""
	}
	return strings.ToUpper(op.ApiRoute[0:1]) + op.GoSdkName[0:1]
}

func (op *Option) EnumNone() string {
	if len(op.ApiRoute) < 3 || len(op.GoSdkName) < 2 {
		return ""
	}
	tag := op.EnumTag()
	if tag == "CM" {
		// name clash otherwise
		tag = strings.ToUpper(op.ApiRoute[0:2]) + op.GoSdkName[0:1]
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
			if op.ApiRoute == "state" {
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
			tmpl = "	opts.{{.GoName}}Addr = base.HexToAddress(opts.{{.GoName}})"
			ret += "\n" + op.executeTemplate(tmplName, tmpl)
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
	return op.IsNullDefault() || op.Generate == "config"
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
	if !op.IsEnum {
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
	if !op.IsEnum {
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
	if op.Generate == "config" {
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

// SdkField for tag {{.SdkField}}}
func (op *Option) SdkField() string {
	tmplName := "sdkField"
	tmpl := `	{{.GoSdkName}} {{.GoSdkType}} {{.JsonTag}}
`
	return op.executeTemplate(tmplName, tmpl)
}

// SetOption for tag {{.SetOption}}
func (op *Option) SetOption() string {
	ret := ""
	if !op.IsPositional() && op.OptionType != "alias" {
		tmplName := "setOptions"
		tmpl := `[{ROUTE}]Cmd.Flags().{{.CobraType}}VarP(&[{ROUTE}]Pkg.GetOptions().{{.GoName}}, "{{.LongName}}", "{{.HotKey}}", {{.CmdDefault}}, {{.CobraDescription}})`
		ret = op.executeTemplate(tmplName, tmpl)
	}
	ret = strings.ReplaceAll(ret, "[{ROUTE}]", op.ApiRoute)
	return ret
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

func (op *Option) CobraDescription() string {
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
		tmpl := `	{{.GoSdkName}} []identifiers.Identifier`
		ret += "\n" + op.executeTemplate(tmplName, tmpl)
		ret += "`json:\"blockIds,omitempty\"`   // Block identifiers"
	} else if op.LongName == "transactions" {
		tmplName := "optFields4"
		tmpl := `	{{.GoSdkName}} []identifiers.Identifier`
		ret += "\n" + op.executeTemplate(tmplName, tmpl)
		ret += "`json:\"transactionIds,omitempty\"`   // Transaction identifiers"
	}
	return ret
}

func (op *Option) DescrCaps() string {
	return strings.ToUpper(op.Description[0:1]) + op.Description[1:]
}

func (op *Option) RequestOpt() string {
	var ret string
	if op.Generate == "config" {
		tmplName := "requestOpts1"
		tmpl := `	case "{{.SnakeCase}}":
		configs[key] = value[0]`
		ret = op.executeTemplate(tmplName, tmpl)
	} else {
		if strings.HasPrefix(op.DataType, "list") {
			tmplName := "requestOpts2"
			tmpl := `		case "{{.SnakeCase}}":
			for _, val := range value {
				s := strings.Split(val, " ") // may contain space separated items
				opts.{{.GoName}} = append(opts.{{.GoName}}, s...)
			}`
			ret = op.executeTemplate(tmplName, tmpl)
		} else if op.DataType == "<boolean>" {
			tmplName := "requestOpts3"
			tmpl := `		case "{{.SnakeCase}}":
			opts.{{.GoName}} = true`
			ret = op.executeTemplate(tmplName, tmpl)
		} else if op.DataType == "<uint64>" || op.DataType == "<blknum>" {
			tmplName := "requestOpts4"
			tmpl := `		case "{{.SnakeCase}}":
			opts.{{.GoName}} = globals.ToUint64(value[0])`
			ret = op.executeTemplate(tmplName, tmpl)
		} else if op.DataType == "<double>" {
			tmplName := "requestOpts5"
			tmpl := `		case "{{.SnakeCase}}":
			opts.{{.GoName}} = globals.ToFloat64(value[0])`
			ret = op.executeTemplate(tmplName, tmpl)
		} else {
			tmplName := "requestOpts6"
			tmpl := `		case "{{.SnakeCase}}":
			opts.{{.GoName}} = value[0]`
			ret = op.executeTemplate(tmplName, tmpl)
		}
	}
	return strings.Replace(ret, "Settings.", "", -1)
}
