package types

import (
	"encoding/json"
	"fmt"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

type Member struct {
	Name        string     `json:"name,omitempty" csv:"name"`
	Type        string     `json:"type,omitempty" csv:"type"`
	StrDefault  string     `json:"strDefault,omitempty" csv:"strDefault"`
	Attributes  string     `json:"attributes,omitempty" csv:"attributes"`
	DocOrder    int        `json:"docOrder,omitempty" csv:"docOrder"`
	Upgrades    string     `json:"upgrades,omitempty" csv:"upgrades"`
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

func (m *Member) IsNoTag() bool {
	return strings.Contains(m.Attributes, "notag")
}

func (m *Member) IsCalc() bool {
	return strings.Contains(m.Attributes, "calc")
}

func (m *Member) HasUpgrade() bool {
	return len(m.Upgrades) > 0
}

func (m *Member) IsRawField() bool {
	return !m.IsCalc() && !m.IsSimpOnly()
}

func (m *Member) IsSimpField() bool {
	return !m.IsCalc() && !m.IsRawOnly()
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
	if m.Name == "blockNumber" && (m.Container() == "Block" || m.Container() == "LightBlock") {
		return "`json:\"number\"`"
	}
	return "`json:\"" + m.Name + "\"`"
}

func (m *Member) Tag() string {
	if m.IsNoTag() {
		tmplName := "notag"
		tmpl := "`json:\"-\"`"
		return m.executeTemplate(tmplName, tmpl)
	} else {
		tmplName := "tag"
		tmpl := "`json:\"{{.Name}}{{if .IsOmitEmpty}},omitempty{{end}}\"`"
		return m.executeTemplate(tmplName, tmpl)
	}
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
	if m.Container() == "Block" || m.Container() == "LightBlock" {
		if m.GoName() == "Transactions" {
			return "[]any"
		} else if m.GoName() == "Withdrawals" {
			return "[]Withdrawal"
		}
	}

	ret := "string"
	switch m.Container() {
	case "Trace":
		if m.GoName() == "Action" {
			return "TraceAction"
		} else if m.GoName() == "TransactionHash" {
			return "string"
		}
		if m.IsObject() {
			if m.GoName() == "Result" {
				return "*TraceResult"
			}
		} else {
			switch m.GoName() {
			case "BlockHash":
				return "string"
			case "BlockNumber":
				return "base.Blknum"
			case "TransactionIndex":
				return "base.Txnum"
			case "TraceAddress":
				return "[]uint64"
			case "TransactionHash":
				return "hash"
			}
			return m.Type
		}
	case "Receipt":
		if m.GoName() == "Logs" {
			return "[]Log"
		}
	case "Transaction":
		if m.GoName() == "AccessList" {
			return "[]StorageSlot"
		}
	case "Appearance":
		gn := m.GoName()
		if gn == "BlockNumber" || gn == "TransactionIndex" {
			return "uint32"
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

	ret := m.Type
	if m.IsObject() {
		if m.GoName() != "TokenType" {
			ret = "" + ret
		}
	} else {
		if m.GoName() == "Value" && m.Container() == "Parameter" {
			ret = "any"
		} else {
			switch m.Type {
			case "address":
				ret = "base.Address"
			case "float":
				ret = "base.Float"
			case "blknum":
				ret = "base.Blknum"
			case "blkrange":
				ret = "string"
			case "bytes":
				ret = "string"
			case "datetime":
				ret = "string"
			case "ether":
				ret = "base.Ether"
			case "gas":
				ret = "base.Gas"
			case "hash":
				ret = "base.Hash"
			case "int256":
				ret = "base.Wei"
			case "ipfshash":
				ret = "base.IpfsHash"
			case "lognum":
				ret = "base.Lognum"
			case "timestamp":
				ret = "base.Timestamp"
			case "topic":
				ret = "base.Hash"
			case "txnum":
				ret = "base.Txnum"
			case "value":
				ret = "base.Value"
			case "wei":
				ret = "base.Wei"
			}
		}
	}

	if m.IsPointer {
		ret = "*" + ret
	}
	if m.IsArray {
		ret = "[]" + ret
	}
	if strings.HasPrefix(ret, "Raw") {
		ret = strings.Replace(ret, "Raw", "Raw", -1)
	}

	return ret
}

func (m *Member) NeedsPtr() bool {
	return m.GoType() == "base.Hash" ||
		m.GoType() == "base.Wei" ||
		m.IsObject()
}

// MarshalCode writes the writer code for caching this item
func (m *Member) MarshalCode() string {
	if m.IsCalc() ||
		m.IsRawOnly() ||
		(m.Container() == "Transaction" && m.GoName() == "Traces") {
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
	case []Transaction:
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
	} else if m.IsArray &&
		m.GoName() != "Topics" &&
		m.GoName() != "Transactions" &&
		m.GoName() != "TraceAddress" &&
		m.GoName() != "Uncles" {
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
	opt{{.GoName}} := &cache.Optional[{{.Type}}]{
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

// UnmarshalCode writes the reader code for caching this item
func (m *Member) UnmarshalCode() string {
	wasRemoved := m.HasUpgrade() && m.IsCalc()
	if (!wasRemoved && m.IsCalc()) ||
		m.IsRawOnly() ||
		(m.Container() == "Transaction" && m.GoName() == "Traces") {
		return ""
	}

	tmplName := "unmarshalCode"
	tmpl := ""
	if m.GoName() == "Transactions" && m.Container() == "Block" {
		tmplName += "1"
		tmpl = `		// Transactions
	s.Transactions = make([]string, 0)
	if err = cache.ReadValue(reader, &s.Transactions, vers); err != nil {
		return err
	}

`
	} else if m.GoName() == "Transactions" && m.Container() == "LightBlock" {
		tmplName += "2"
		tmpl = `		// Transactions
	s.Transactions = make([]base.Hash, 0)
	if err = cache.ReadValue(reader, &s.Transactions, vers); err != nil {
		return err
	}

`
	} else if m.GoName() == "Value" && m.Container() == "Parameter" {
		tmplName += "3"
		tmpl = `// {{.GoName}}
	var {{.Lower}} string
	if err = cache.ReadValue(reader, &{{.Lower}}, vers); err != nil {
		return err
	}
	if err = json.Unmarshal([]byte({{.Lower}}), &s.{{.GoName}}); err != nil {
		return fmt.Errorf("cannot unmarshal {{.GoName}}: %w", err)
	}

`
	} else if m.IsArray {
		tmplName += "4"
		tmpl = `// {{.GoName}}
	s.{{.GoName}} = make({{.GoType}}, 0)
	if err = cache.ReadValue(reader, &s.{{.GoName}}, vers); err != nil {
		return err
	}

`
	} else if m.IsObject() {
		tmplName += "5"
		tmpl = `// {{.GoName}}
	opt{{.GoName}} := &cache.Optional[{{.Type}}]{
		Value: s.{{.GoName}},
	}
	if err = cache.ReadValue(reader, opt{{.GoName}}, vers); err != nil {
		return err
	}
	s.{{.GoName}} = opt{{.GoName}}.Get()

`

	} else {
		tmplName += "6"
		tmpl = `// {{.GoName}}
	if err = cache.ReadValue(reader, &s.{{.GoName}}, vers); err != nil {
		return err
	}

`
	}

	code := m.executeTemplate(tmplName, tmpl)

	if m.HasUpgrade() {
		if wasRemoved {
			tmplName = "upgrageRemoved"
			tmpl = `	// Used to be {{.GoName}}, since removed
	v{{.GoName}} := version.NewVersion("++VERS++")
	if vers <= v{{.GoName}}.Uint64() {
		var val ++PRIOR_TYPE++
		if err = cache.ReadValue(reader, &val, vers); err != nil {
			return err
		}
	}

`
		} else {
			tmplName = "upgrage"
			tmpl = `	// {{.GoName}}
	v{{.GoName}} := version.NewVersion("++VERS++")
	if vers <= v{{.GoName}}.Uint64() {
		var val ++PRIOR_TYPE++
		if err = cache.ReadValue(reader, &val, vers); err != nil {
			return err
		}
		s.{{.GoName}} = ++CONV_FUNC++(val)
	} else {
		++CODE++
	}

`
		}

		cc := strings.Trim(code, "\n")
		parts := strings.Split(m.Upgrades, ":")
		if len(parts) != 2 {
			panic("invalid upgrade spec: " + m.Upgrades)
		}
		code = m.executeTemplate(tmplName, tmpl)
		code = strings.ReplaceAll(code, "++VERS++", parts[0])
		convert := func(s string) string {
			switch s {
			case "wei":
				return "base.Wei"
			}
			return s
		}
		// TODO: hack
		mm := map[string]string{
			"CumulativeGasUsed": "base.MustParseGas",
			"Status":            "base.Value",
			"BaseFeePerGas":     "weiToGas",
		}
		code = strings.ReplaceAll(code, "++CONV_FUNC++", mm[m.GoName()])
		code = strings.ReplaceAll(code, "++PRIOR_TYPE++", convert(parts[1]))
		code = strings.ReplaceAll(code, "++PRIOR++", parts[1])
		code = strings.ReplaceAll(code, "++CODE++", cc)
	}

	return code
}

func (m *Member) YamlType() string {
	o := fmt.Sprintf("\n          items:\n            $ref: \"#/components/schemas/" + CamelCase(m.Type) + "\"")
	f := fmt.Sprintf("\n          format: %s", m.Type)
	if m.IsArray {
		return "array" + o
	}
	if m.IsObject() {
		return "object" + o
	} else if m.Type == "blknum" || m.Type == "txnum" || m.Type == "lognum" ||
		m.Type == "timestamp" || m.Type == "float" || m.Type == "float64" || m.Type == "gas" || m.Type == "uint64" ||
		m.Type == "int64" || m.Type == "uint32" || m.Type == "int" || m.Type == "value" {
		return "number" + f
	} else if m.Type == "address" || m.Type == "datetime" || m.Type == "hash" || m.Type == "ipfshash" || m.Type == "blkrange" ||
		m.Type == "topic" || m.Type == "int256" || m.Type == "uint256" || m.Type == "wei" || m.Type == "bytes" ||
		m.Type == "string" || m.Type == "ether" {
		return "string" + f
	} else if m.Type == "bool" || m.Type == "uint8" {
		return "boolean\n          format: boolean"
	} else {
		logger.Fatal(fmt.Sprintf("unknown type '%s' in Member '%s'\n", m.Type, m.Name))
		return "unknown" + f
	}
}

func readMember(m *Member, data *any) (bool, error) {
	// trim spaces read from the file (if any)
	m.Name = strings.Trim(m.Name, " ")
	m.Type = strings.Trim(m.Type, " ")
	m.Upgrades = strings.Trim(m.Upgrades, " ")
	m.StrDefault = strings.Trim(m.StrDefault, " ")
	m.Attributes = strings.Trim(m.Attributes, " ")
	m.Description = strings.Trim(m.Description, " ")

	m.Description = strings.ReplaceAll(m.Description, "&#44;", ",")

	m.IsPointer = strings.Contains(m.Type, "*")
	m.IsArray = strings.Contains(m.Type, "[]")
	m.Type = strings.Trim(m.Type, "[]*")

	return true, nil
}

func (m *Member) executeTemplate(name, tmplCode string) string {
	return executeTemplate(m, "member", name, tmplCode)
}
