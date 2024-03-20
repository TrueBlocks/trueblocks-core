package main

import (
	"strings"
	"text/template"
)

type CmdLineOption struct {
	cmd            *Command
	Num            string   `json:"num"`
	Group          string   `json:"group"`
	Tags           string   `json:"tags"`
	ApiRoute       string   `json:"api_route"`
	Tool           string   `json:"tool"`
	LongName       string   `json:"longName"`
	HotKey         string   `json:"hotKey"`
	DefVal         string   `json:"def_val"`
	IsRequired     bool     `json:"is_required"`
	IsCustomizable bool     `json:"is_customizable"`
	IsVisible      bool     `json:"is_visible"`
	IsVisibleDocs  bool     `json:"is_visible_docs"`
	Generate       string   `json:"generate"`
	OptionType     string   `json:"option_type"`
	DataType       string   `json:"data_type"`
	Description    string   `json:"description"`
	IsArray        bool     `json:"is_array"`
	IsEnum1        bool     `json:"is_enum"`
	Enums          []string `json:"enums"`
	DefaultEnum    string   `json:"default_enum"`
	GoName1        string   `json:"go_name"`
	GoType         string   `json:"go_type"`
	GoSdkName      string   `json:"go_sdk_name"`
	GoSdkType      string   `json:"go_sdk_type"`
	GoOptionsType  string   `json:"go_options_type"`
	templates      map[string]*template.Template
}

func (op CmdLineOption) Validate() bool { return len(op.ApiRoute) > 0 }

func ReadCmdOption(op *CmdLineOption, data *any) (bool, error) {
	op.Description = strings.ReplaceAll(op.Description, "&#44;", ",")
	return true, nil
}

func snakeCase(s string) string {
	result := ""
	toUpper := false
	for _, c := range s {
		if c == '_' {
			toUpper = true
			continue
		}
		if toUpper {
			result += strings.ToUpper(string(c))
			toUpper = false
		} else {
			result += string(c)
		}
	}
	return result
}

func (op *CmdLineOption) Hidden() bool {
	return !op.IsVisibleDocs || (!op.IsRequired && !op.IsCustomizable && !op.IsVisible && op.IsVisibleDocs)
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

func (op *CmdLineOption) DescrCaps() string {
	return strings.ToUpper(op.Description[0:1]) + op.Description[1:]
}

func (op *CmdLineOption) CobraDescription() string {
	d := op.Description
	if op.Hidden() {
		d += " (hidden)"
	}
	if op.IsEnum1 {
		e := "One of "
		if op.IsArray {
			e = "One or more of "
		}
		e += "[ " + strings.Join(op.Enums, " | ") + " ]"
		return "`" + d + "\n" + e + "`"
	}
	return "\"" + d + "\""
}

func (op *CmdLineOption) SetOptions() string {
	ret := ""
	if op.OptionType != "positional" && op.OptionType != "alias" {
		tmpl := `[{ROUTE}]Cmd.Flags().{{.CobraType}}VarP(&[{ROUTE}]Pkg.GetOptions().{{.GoName1}}, "{{.LongName}}", "{{.HotKey}}", {{.Default}}, {{.CobraDescription}})`
		ret = op.executeTemplate("setOptions", tmpl)
	}
	ret = strings.ReplaceAll(ret, "[{ROUTE}]", op.ApiRoute)
	return ret
}

func (op *CmdLineOption) toGoName() string {
	if len(op.LongName) < 2 {
		return op.LongName
	}

	ret := strings.ToUpper(op.LongName[0:1]) + snakeCase(op.LongName)[1:]
	if op.Generate == "config" {
		ret = "Settings." + ret
	}

	return ret
}

func (op *CmdLineOption) toGoType() string {
	dt := op.DataType
	if len(dt) < 2 {
		return dt
	}

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
		"<tx_id>":    "base.Blknum",
		"<boolean>":  "bool",
		"<double>":   "float64",
		"<string>":   "string",
		"<uint64>":   "uint64",
		"<fourbyte>": "base.Fourbyte",
		"<topic>":    "base.Hash",
	}
	if isList {
		return "[]" + m[dt]
	}
	return m[dt]
}

func (op *CmdLineOption) toGoSdkName() string {
	ret := op.toGoName()
	if ret == "Blocks" {
		ret = "BlockIds"
	}
	if ret == "Transactions" {
		ret = "TransactionIds"
	}
	return ret
}

func (op *CmdLineOption) Enum1() string {
	if !op.IsEnum1 {
		return ""
	}
	tmpl := enum1Tmpl
	return op.executeTemplate("enum1", tmpl)
}

func (op *CmdLineOption) Enum2() string {
	if !op.IsEnum1 {
		return ""
	}
	tmpl := enum2Tmpl
	return op.executeTemplate("enum2", tmpl)
}

func (op *CmdLineOption) toGoOptionsType() string {
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

func (op *CmdLineOption) toGoSdkType() string {
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
		"<tx_id>":    "string",
		"<boolean>":  "bool",
		"<double>":   "float64",
		"<string>":   "string",
		"<uint64>":   "uint64",
		"<fourbyte>": "string",
		"<topic>":    "string",
	}
	if isList {
		m["<blknum>"] = "string"
		return "[]" + m[dt]
	}
	return m[dt]
}

func pad(s string, width int) string {
	return s + strings.Repeat(" ", width-len(s))
}

func (op *CmdLineOption) JsonTag() string {
	return "`json:\"" + op.SnakeCase() + ",omitempty\"`"
}

func (op *CmdLineOption) SdkField() string {
	tmpl := `	{{.GoSdkName}} {{.GoSdkType}} {{.JsonTag}}
`
	return op.executeTemplate("sdkField", tmpl)
}

func (op *CmdLineOption) SnakeCase() string {
	ret := snakeCase(op.LongName)
	if op.Generate == "config" {
		ret = "Settings." + ret
	}
	return ret
}

func (op *CmdLineOption) PyHotKey() string {
	if len(op.HotKey) == 0 {
		return ""
	}
	return "-" + op.HotKey
}

func (op *CmdLineOption) Default() string {
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

func (op *CmdLineOption) EnumName() string {
	return strings.ToUpper(op.ApiRoute[0:1]) + op.ApiRoute[1:] + op.GoName1
}

func (op *CmdLineOption) EnumTag() string {
	return strings.ToUpper(op.ApiRoute[0:1]) + op.GoSdkName[0:1]
}

func (op *CmdLineOption) EnumNone() string {
	tag := op.EnumTag()
	if tag == "CM" {
		// name clash otherwise
		tag = strings.ToUpper(op.ApiRoute[0:2]) + op.GoSdkName[0:1]
	}
	return "No" + tag
}

func (op *CmdLineOption) EnumDef() string {
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

func (op *CmdLineOption) EnumMap() string {
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

func (op *CmdLineOption) Lower() string {
	return strings.ToLower(op.LongName)
}

func (op *CmdLineOption) EnumList() string {
	ret := []string{}
	for _, e := range op.Enums {
		if e == "some" || e == "all" {
			continue
		}
		ret = append(ret, op.EnumTag()+strings.ToUpper(e[0:1])+e[1:])
	}
	return op.EnumName() + "{" + strings.Join(ret, ", ") + "}"
}

func (op *CmdLineOption) HasEnumAll() bool {
	for _, e := range op.Enums {
		if e == "all" {
			return true
		}
	}
	return false
}

func (op *CmdLineOption) PreSwitch() string {
	if !op.HasEnumAll() {
		return ""
	}
	tmpl := `if len(values) == 1 && values[0] == "all" {
		return {{.EnumTag}}All, nil
	} else if len(values) == 1 && values[0] == "some" {
		return {{.EnumTag}}Some, nil
	}

`
	return op.executeTemplate("preSwitch", tmpl)
}

func (op *CmdLineOption) SomeCases() string {
	if !op.HasEnumAll() {
		return ""
	}
	tmpl := `	case {{.EnumTag}}Some:
		return "some"
	case {{.EnumTag}}All:
		return "all"
`
	return op.executeTemplate("someCases", tmpl)
}

func (op *CmdLineOption) EnumCases() string {
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

var enum1Tmpl = `	if key == "{{.LongName}}" {
		var err error
		values := strings.Split(value, ",")
		if opts.{{.GoName1}}, err = enumFrom{{.EnumName}}(values); err != nil {
			return false, err
		} else {
			found = true
		}
	}
	`

var enum2Tmpl = `type {{.EnumName}} int

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

func (op *CmdLineOption) IsConfigurableAddr() bool {
	return op.GoName1 == "Publisher"
}

func (op *CmdLineOption) IsSpecialAddr() bool {
	return op.GoName1 == "ProxyFor" || op.IsConfigurableAddr() || op.GoName1 == "Autoname"
}

func (op *CmdLineOption) EnsConvert() string {
	ret := ""

	if op.DataType == "<address>" {
		t1 := "	opts.{{.GoName1}}, _ = opts.Conn.GetEnsAddress(opts.{{.GoName1}})"
		if op.IsConfigurableAddr() {
			t1 = "	opts.{{.GoName1}}, _ = opts.Conn.GetEnsAddress(config.Get{{.GoName1}}(opts.{{.GoName1}}))"
		}
		ret = op.executeTemplate("ens", t1)
		if op.IsSpecialAddr() {
			ret += "\n" + op.executeTemplate("ens2", "	opts.{{.GoName1}}Addr = base.HexToAddress(opts.{{.GoName1}})")
		}
	} else if op.DataType == "list<addr>" {
		t1 := "	opts.{{.GoName1}}, _ = opts.Conn.GetEnsAddresses(opts.{{.GoName1}})"
		ret = op.executeTemplate("ensSlice", t1)
	}

	return ret
}
