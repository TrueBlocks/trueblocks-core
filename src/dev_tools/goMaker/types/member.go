package types

import (
	"encoding/json"
	"fmt"
	"strings"
)

type Member struct {
	Name        string     `json:"name,omitempty" csv:"name"`
	Type        string     `json:"type,omitempty" csv:"type"`
	StrDefault  string     `json:"strDefault,omitempty" csv:"strDefault"`
	Attributes  string     `json:"attributes,omitempty" csv:"attributes"`
	DocOrder    int        `json:"docOrder,omitempty" csv:"docOrder"`
	Description string     `json:"description,omitempty" csv:"description"`
	Num         int        `json:"num"`
	IsArray     bool       `json:"isArray,omitempty"`
	IsPointer   bool       `json:"isPointer,omitempty"`
	stPtr       *Structure `json:"-"`
}

func (m *Member) String() string {
	bytes, _ := json.MarshalIndent(m, "", "  ")
	return string(bytes)
}

func (m Member) Validate() bool {
	return m.Name != "" && m.Type != ""
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

func (m *Member) Container() string {
	return m.stPtr.Class
}

func (m *Member) GoName() string {
	if m.Name == "type" {
		return m.Container() + FirstUpper(m.Name)
	}
	return FirstUpper(m.Name)
}

func (m *Member) IsObject() bool {
	// assume it's an object if its the first letter of its type is upper case.
	return m.Type != FirstLower(m.Type)
}

func (m *Member) IsOmitEmpty() bool {
	return strings.Contains(m.Attributes, "omitempty")
}

func (m *Member) IsRawOnly() bool {
	return strings.Contains(m.Attributes, "rawonly")
}

func (m *Member) IsSimpOnly() bool {
	return strings.Contains(m.Attributes, "simponly")

}

func (m *Member) IsSubField() bool {
	return strings.Contains(m.Name, "::")
}

func (m *Member) IsCalc() bool {
	return strings.Contains(m.Attributes, "calc")
}

func (m *Member) IsRawField() bool {
	return !m.IsCalc() && !m.IsSimpOnly() && !m.IsSubField()
}

func (m *Member) IsSimpField() bool {
	return !m.IsCalc() && !m.IsRawOnly() && !m.IsSubField()
}

func (m *Member) SortName() string {
	if m.Name == "type" {
		return Proper(m.Name)
	}
	return m.GoName()
}

func (m *Member) RawTag() string {
	if m.Name == "transactionIndex" && m.Container() == "Trace" {
		return "`json:\"transactionPosition\"`"
	}
	if m.Name == "blockNumber" && m.Container() == "Block" {
		return "`json:\"number\"`"
	}
	return "`json:\"" + m.Name + "\"`"
}

func (m *Member) Tag() string {
	tmplName := "tag"
	tmpl := "`json:\"{{.Name}}{{if .IsOmitEmpty}},omitempty{{end}}\"`"
	return m.executeTemplate(tmplName, tmpl)
}

func (m *Member) MarkdownDescription() string {
	descr := m.Description
	if m.IsCalc() {
		descr += " (calculated)"
	}
	return descr
}

func (m *Member) TypeToGroup(t string) string {
	return m.stPtr.cbPtr.TypeToGroup(m.Type)
}

func (m *Member) MarkdownType() string {
	typ := m.Type
	if m.IsArray {
		typ = typ + "[]"
	}
	if m.IsObject() {
		group := m.TypeToGroup(m.Type)
		if !strings.Contains(group, "unknown") {
			return "[" + typ + "](/data-model/" + group + "/#" + strings.ToLower(m.Type) + ")"
		}
	}
	return typ
}

func (m *Member) RawType() string {
	if strings.HasPrefix(m.Container(), "Block") {
		if m.GoName() == "Transactions" {
			return "[]any"
		} else if m.GoName() == "Withdrawals" {
			return "[]RawWithdrawal"
		}
	}

	ret := "string"
	switch m.Container() {
	case "Trace":
		if m.GoName() == "Action" {
			return "RawTraceAction"
		} else if m.GoName() == "TransactionHash" {
			return "string"
		}
		if m.IsObject() {
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
	one := m.Container() == "Function" && (m.GoName() == "Inputs" || m.GoName() == "Outputs")
	two := m.Container() == "Manifest" && m.GoName() == "Chunks"
	three := m.Container() == "Parameter" && m.GoName() == "Components"
	if !one && !two && !three && m.IsArray {
		ret = "[]" + ret
	}
	return ret
}

func (m *Member) GoType() string {
	if strings.HasPrefix(m.Container(), "Block") && m.GoName() == "Transactions" {
		return "[]Tx"
	}

	if m.Container() == "Status" {
		if m.Type == "CacheItem" {
			return "[]simpleCacheItem"
		} else if m.Type == "Chain" {
			return "[]types.SimpleChain"
		}
	}

	ret := m.Type
	if m.IsObject() {
		if m.GoName() != "TokenType" {
			ret = "Simple" + ret
		}
	} else {
		if m.GoName() == "Value" && m.Container() == "Parameter" {
			ret = "any"
		} else if m.GoName() == "CumulativeGasUsed" && m.Container() == "Receipt" {
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
			case "datetime":
				ret = "string"
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
		m.IsObject()
}

func (m *Member) MarshalCode() string {
	if strings.Contains(m.GoName(), "::") ||
		m.IsCalc() ||
		m.IsRawOnly() ||
		(m.Container() == "Transaction" &&
			(m.GoName() == "CompressedTx" || m.GoName() == "Traces")) {
		return ""
	}

	tmplName := "cache"
	tmpl := ""
	if m.GoName() == "Transactions" && m.Container() == "Block" {
		tmplName += "1"
		tmpl = `	// Transactions
	var txHashes []string
	switch v := any(s.Transactions).(type) {
	case []string:
		txHashes = v
	case []SimpleTransaction:
		txHashes = make([]string, 0, len(s.Transactions))
		for _, tx := range v {
			txHashes = append(txHashes, tx.Hash.Hex())
		}
	}
	if err = cache.WriteValue(writer, txHashes); err != nil {
		return err
	}

`
	} else if m.GoName() == "Value" && m.Container() == "Parameter" {
		tmplName += "2"
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
		tmplName += "3"
		tmpl = `// {{.GoName}}
	{{.Lower}} := make([]cache.Marshaler, 0, len(s.{{.GoName}}))
	for _, {{.LowerSingular}} := range s.{{.GoName}} {
		{{.Lower}} = append({{.Lower}}, {{if .NeedsPtr}}&{{end}}{{.LowerSingular}})
	}
	if err = cache.WriteValue(writer, {{.Lower}}); err != nil {
		return err
	}

`
	} else if m.IsObject() {
		tmplName += "4"
		tmpl = `// {{.GoName}}
	opt{{.GoName}} := &cache.Optional[Simple{{.Type}}]{
		Value: s.{{.GoName}},
	}
	if err = cache.WriteValue(writer, opt{{.GoName}}); err != nil {
		return err
	}

`
	} else {
		tmplName += "5"
		tmpl = `// {{.GoName}}
	if err = cache.WriteValue(writer, {{if .NeedsPtr}}&{{end}}s.{{.GoName}}); err != nil {
		return err
	}

`
	}

	return m.executeTemplate(tmplName, tmpl)
}

func (m *Member) UnmarshalCode() string {
	if strings.Contains(m.GoName(), "::") ||
		m.IsCalc() ||
		m.IsRawOnly() ||
		(m.Container() == "Transaction" &&
			(m.GoName() == "CompressedTx" || m.GoName() == "Traces")) {
		return ""
	}

	tmplName := "unmarshalCode"
	tmpl := ""
	if m.GoName() == "Transactions" && m.Container() == "Block" {
		tmplName += "1"
		tmpl = `		// Transactions
	s.Transactions = make([]string, 0)
	if err = cache.ReadValue(reader, &s.Transactions, version); err != nil {
		return err
	}

`
	} else if m.GoName() == "Value" && m.Container() == "Parameter" {
		tmplName += "2"
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
		tmplName += "3"
		tmpl = `// {{.GoName}}
	s.{{.GoName}} = make({{.GoType}}, 0)
	if err = cache.ReadValue(reader, &s.{{.GoName}}, version); err != nil {
		return err
	}

`
	} else if m.IsObject() {
		tmplName += "4"
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
		tmplName += "5"
		tmpl = `// {{.GoName}}
	if err = cache.ReadValue(reader, &s.{{.GoName}}, version); err != nil {
		return err
	}

`
	}

	return m.executeTemplate(tmplName, tmpl)
}

func (m *Member) YamlType() string {
	o := fmt.Sprintf("\n          items:\n            $ref: \"#/components/schemas/" + CamelCase(m.Type) + "\"")
	f := fmt.Sprintf("\n          format: %s", m.Type)
	if m.IsArray {
		return "array" + o
	}
	if m.IsObject() {
		return "object" + o
	} else if m.Type == "blknum" || m.Type == "timestamp" || m.Type == "double" ||
		m.Type == "gas" || m.Type == "uint64" || m.Type == "int64" || m.Type == "uint32" {
		return "number" + f
	} else if m.Type == "address" || m.Type == "datetime" || m.Type == "hash" || m.Type == "ipfshash" || m.Type == "blkrange" ||
		m.Type == "topic" || m.Type == "int256" || m.Type == "uint256" || m.Type == "wei" || m.Type == "bytes" {
		return "string" + f
	} else if m.Type == "bool" || m.Type == "uint8" {
		return "boolean\n          format: boolean"
	}
	return "string" + f
}

func readMember(m *Member, data *any) (bool, error) {
	// trim spaces read from the file (if any)
	m.Name = strings.Trim(m.Name, " ")
	m.Type = strings.Trim(m.Type, " ")
	m.StrDefault = strings.Trim(m.StrDefault, " ")
	m.Attributes = strings.Trim(m.Attributes, " ")
	m.Description = strings.Trim(m.Description, " ")

	m.Description = strings.ReplaceAll(m.Description, "&#44;", ",")

	m.IsPointer = strings.Contains(m.Type, "*")
	m.IsArray = strings.Contains(m.Type, "[]")
	m.Type = strings.Trim(m.Type, "[]*")

	return true, nil
}
