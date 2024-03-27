package types

import (
	"encoding/json"
	"strings"
)

type Member struct {
	Num          int         `json:"num" csv:"num"`
	DocOrder     int         `json:"docOrder,omitempty" csv:"docOrder"`
	Name         string      `json:"name,omitempty" csv:"name"`
	Type         string      `json:"type,omitempty" csv:"type"`
	StrDefault   string      `json:"strDefault,omitempty" csv:"strDefault"`
	Attributes   string      `json:"attributes,omitempty" csv:"attributes"`
	Description  string      `json:"description,omitempty" csv:"description"`
	IsOmitEmpty  bool        `json:"isOmitEmpty,omitempty" csv:"-"`
	IsCalc       bool        `json:"isCalc,omitempty" csv:"-"`
	IsRawOnly    bool        `json:"isRawOnly,omitempty" csv:"-"`
	IsSimpleOnly bool        `json:"isSimpleOnly,omitempty" csv:"-"`
	IsArray      bool        `json:"isArray,omitempty" csv:"-"`
	IsPointer    bool        `json:"isPointer,omitempty" csv:"-"`
	Proper       string      `json:"-" csv:"-"`
	Class        string      `json:"-" csv:"-"`
	templates    TemplateMap `json:"-" csv:"-"`
}

func readMember(m *Member, data *any) (bool, error) {
	m.IsPointer = strings.Contains(m.Type, "*")
	m.IsArray = strings.Contains(m.Type, "[]")
	m.IsOmitEmpty = strings.Contains(m.Attributes, "omitempty")
	m.IsCalc = strings.Contains(m.Attributes, "calc")
	m.IsRawOnly = strings.Contains(m.Attributes, "rawonly")
	m.IsSimpleOnly = strings.Contains(m.Attributes, "simponly")

	m.Name = strings.Trim(m.Name, " ")
	m.Type = strings.Trim(m.Type, " []*")
	m.StrDefault = strings.Trim(m.StrDefault, " ")
	m.Attributes = strings.Trim(m.Attributes, " ")
	m.Description = strings.ReplaceAll(m.Description, "&#44;", ",")
	m.Proper = strings.ToUpper(m.Name[0:1]) + m.Name[1:]
	m.Description = strings.Trim(m.Description, " ")

	return true, nil
}

func (m *Member) String() string {
	bytes, _ := json.MarshalIndent(m, "", "  ")
	return string(bytes)
}

func (m Member) Validate() bool {
	return m.Name != "" && m.Type != ""
}

func (m *Member) SortName() string {
	if m.Name == "type" {
		// We can't change the sort for this because it effects the way things are stored in the cache
		return m.Proper
	}
	return m.GoName()
}

func (m *Member) GoName() string {
	if m.Name == "type" {
		return m.Class + m.Proper
	}
	return m.Proper
}

func (m *Member) TagName() string {
	return m.Name
}

func (m *Member) Lower() string {
	return strings.ToLower(m.GoName())
}

func (m *Member) LowerSingular() string {
	if strings.HasSuffix(m.GoName(), "s") {
		return strings.ToLower(m.GoName())[:len(m.GoName())-1]
	}
	return strings.ToLower(m.GoName())
}

func (m *Member) IsObjType() bool {
	return m.Type[0:1] != strings.ToLower(m.Type[0:1])
}

func (m *Member) RawTag() string {
	if m.Name == "transactionIndex" && m.Class == "Trace" {
		return "`json:\"transactionPosition\"`"
	}
	if m.Name == "blockNumber" && m.Class == "Block" {
		return "`json:\"number\"`"
	}
	return "`json:\"" + m.TagName() + "\"`"
}

func (m *Member) Tag() string {
	tmpl := "`json:\"{{.TagName}}{{if .IsOmitEmpty}},omitempty{{end}}\"`"
	return m.executeTemplate("tag", tmpl)
}

func (m *Member) RawType() string {
	if strings.HasPrefix(m.Class, "Block") {
		if m.GoName() == "Transactions" {
			return "[]any"
		} else if m.GoName() == "Withdrawals" {
			return "[]RawWithdrawal"
		}
	}

	ret := "string"
	switch m.Class {
	case "Trace":
		if m.GoName() == "Action" {
			return "RawTraceAction"
		} else if m.GoName() == "TransactionHash" {
			return "string"
		}
		if m.IsObjType() {
			if m.GoName() == "Result" {
				return "*RawTraceResult"
			}
		} else {
			switch m.GoName() {
			case "BlockHash":
				return "string"
			case "BlockNumber":
				return "base.Blknum"
			case "TraceAddress":
				return "[]uint64"
			case "TransactionHash":
				return "hash"
			}
			return m.Type
		}
	case "Receipt":
		if m.GoName() == "Logs" {
			return "[]RawLog"
		}
	case "Transaction":
		if m.GoName() == "AccessList" {
			return "[]StorageSlot"
		}
	}

	ret = "string"
	one := m.Class == "Function" && (m.GoName() == "Inputs" || m.GoName() == "Outputs")
	two := m.Class == "Manifest" && m.GoName() == "Chunks"
	three := m.Class == "Parameter" && m.GoName() == "Components"
	if !one && !two && !three && m.IsArray {
		ret = "[]" + ret
	}
	return ret
}

func (m *Member) GoType() string {
	if strings.HasPrefix(m.Class, "Block") && m.GoName() == "Transactions" {
		return "[]Tx"
	}

	ret := m.Type
	if m.IsObjType() {
		if m.GoName() != "TokenType" {
			ret = "Simple" + ret
		}
	} else {
		if m.GoName() == "Value" && m.Class == "Parameter" {
			ret = "any"
		} else if m.GoName() == "CumulativeGasUsed" && m.Class == "Receipt" {
			ret = "string"
		} else {
			switch m.Type {
			case "address":
				ret = "base.Address"
			case "hash":
				ret = "base.Hash"
			case "bytes":
				ret = "string"
			case "wei":
				ret = "base.Wei"
			case "uint8":
				ret = "bool"
			case "gas":
				ret = "base.Gas"
			case "int256":
				ret = "base.Wei"
			case "double":
				ret = "float64"
			case "ipfshash":
				ret = "base.IpfsHash"
			case "blknum":
				ret = "base.Blknum"
			case "timestamp":
				ret = "base.Timestamp"
			case "topic":
				ret = "base.Hash"
			case "blkrange":
				ret = "string"
			}
		}
	}

	if m.IsPointer {
		ret = "*" + ret
	}
	if m.IsArray {
		ret = "[]" + ret
	}
	if strings.HasPrefix(ret, "SimpleRaw") {
		ret = strings.Replace(ret, "SimpleRaw", "Raw", -1)
	}

	return ret
}

func (m *Member) NeedsPtr() bool {
	return m.GoType() == "base.Hash" ||
		m.GoType() == "base.Wei" ||
		m.IsObjType()
}

func (m *Member) MarshalCode() string {
	if strings.Contains(m.GoName(), "::") ||
		m.IsCalc ||
		m.IsRawOnly ||
		(m.Class == "Transaction" &&
			(m.GoName() == "CompressedTx" || m.GoName() == "Traces")) {
		return ""
	}

	tmpl := ""
	if m.GoName() == "Value" && m.Class == "Parameter" {
		tmpl = `// {{.GoName}}
	{{.Lower}}, err := json.Marshal(s.{{.GoName}})
	if err != nil {
		return fmt.Errorf("cannot marshal {{.GoName}}: %w", err)
	}
	if err = cache.WriteValue(writer, {{.Lower}}); err != nil {
		return err
	}

`

	} else if m.IsArray && m.GoName() != "Topics" && m.GoName() != "TraceAddress" && m.GoName() != "Uncles" {
		tmpl = `// {{.GoName}}
	{{.Lower}} := make([]cache.Marshaler, 0, len(s.{{.GoName}}))
	for _, {{.LowerSingular}} := range s.{{.GoName}} {
		{{.Lower}} = append({{.Lower}}, {{if .NeedsPtr}}&{{end}}{{.LowerSingular}})
	}
	if err = cache.WriteValue(writer, {{.Lower}}); err != nil {
		return err
	}

`
	} else if m.IsObjType() {

		tmpl = `// {{.GoName}}
	opt{{.GoName}} := &cache.Optional[Simple{{.Type}}]{
		Value: s.{{.GoName}},
	}
	if err = cache.WriteValue(writer, opt{{.GoName}}); err != nil {
		return err
	}

`

	} else {
		tmpl = `// {{.GoName}}
	if err = cache.WriteValue(writer, {{if .NeedsPtr}}&{{end}}s.{{.GoName}}); err != nil {
		return err
	}

`
	}

	return m.executeTemplate("marshalCode", tmpl)
}

func (m *Member) UnmarshalCode() string {
	if strings.Contains(m.GoName(), "::") ||
		m.IsCalc ||
		m.IsRawOnly ||
		(m.Class == "Transaction" &&
			(m.GoName() == "CompressedTx" || m.GoName() == "Traces")) {
		return ""
	}

	tmpl := ""
	if m.GoName() == "Value" && m.Class == "Parameter" {
		tmpl = `// {{.GoName}}
	var {{.Lower}} string
	if err = cache.ReadValue(reader, &{{.Lower}}, version); err != nil {
		return err
	}
	if err = json.Unmarshal([]byte({{.Lower}}), &s.{{.GoName}}); err != nil {
		return fmt.Errorf("cannot unmarshal {{.GoName}}: %w", err)
	}

`
	} else if m.IsArray {
		tmpl = `// {{.GoName}}
	s.{{.GoName}} = make({{.GoType}}, 0)
	if err = cache.ReadValue(reader, &s.{{.GoName}}, version); err != nil {
		return err
	}

`
	} else if m.IsObjType() {

		tmpl = `// {{.GoName}}
	opt{{.GoName}} := &cache.Optional[Simple{{.Type}}]{
		Value: s.{{.GoName}},
	}
	if err = cache.ReadValue(reader, opt{{.GoName}}, version); err != nil {
		return err
	}
	s.{{.GoName}} = opt{{.GoName}}.Get()

`

	} else {
		tmpl = `// {{.GoName}}
	if err = cache.ReadValue(reader, &s.{{.GoName}}, version); err != nil {
		return err
	}

`
	}

	return m.executeTemplate("unmarshalCode", tmpl)
}
