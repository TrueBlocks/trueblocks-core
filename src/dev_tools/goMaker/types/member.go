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

func (m *Member) GoName() string {
	if m.Name == "type" {
		return m.Class + m.Proper
	}
	return m.Proper
}

func (m *Member) TagName() string {
	if m.Name == "transactionIndex" && m.Class == "Trace" {
		return "transactionPosition"
	} else if m.Name == "blockNumber" && m.Class == "Block" {
		return "number"
	}
	return m.Name
}

func (m *Member) IsObjType() bool {
	return m.Type[0:1] != strings.ToLower(m.Type[0:1])
}

func (m *Member) RawTag() string {
	return "`json:\"" + m.TagName() + "\"`"
}

func (m *Member) Tag() string {
	if m.IsOmitEmpty {
		return "`json:\"" + m.TagName() + ",omitempty\"`"
	} else {
		return m.RawTag()
	}
}

func (m *Member) RawType() string {
	if m.GoName() == "Logs" && m.Class == "Receipt" {
		return "[]RawLog"
	}
	if m.Class == "Trace" {
		if m.IsObjType() {
			if m.GoName() == "Result" {
				return "*RawTraceResult"
			}
			return "Raw" + m.Type
		}
	}
	if m.GoName() == "AccessList" && m.Class == "Transaction" {
		return "[]StorageSlot"
	}

	ret := "string"
	one := m.Class == "Function" && (m.GoName() == "Inputs" || m.GoName() == "Outputs")
	two := m.Class == "Manifest" && m.GoName() == "Chunks"
	three := m.Class == "Parameter" && m.GoName() == "Components"
	if !one && !two && !three && m.IsArray {
		ret = "[]" + ret
	}
	return ret
}

func (m *Member) GoType() string {
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

	return ret
}
