package types

import (
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
)

// For Commands

func (c *Command) HasEnums() bool {
	for _, op := range c.Options {
		if op.IsEnum() {
			return true
		}
	}
	return false
}

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

// For Options

func (op *Option) EnumChoices() string {
	ret := "One of "
	if op.IsArray() {
		ret = "One or more of "
	}
	return ret + "[ " + strings.Join(op.Enums, " | ") + " ]"
}

func (op *Option) EnumName() string {
	if len(op.Route) < 2 {
		return ""
	}
	return strings.ToUpper(op.Route[0:1]) + op.Route[1:] + op.GoName
}

func (op *Option) EnumTag(e string) string {
	if len(op.Route) < 2 || len(op.GoSdkName) < 2 {
		return ""
	}
	tag := strings.ToUpper(op.Route[0:1]) + op.GoSdkName[0:1]
	if tag == "CM" && e == "none" {
		tag = strings.ToUpper(op.Route[0:2]) + op.GoSdkName[0:1]
	} else if tag == "SP" && (e == "none" || e == "all" || e == "some") {
		tag = strings.ToUpper(op.Route[0:2]) + op.GoSdkName[0:1]
	}
	return tag
}

func (op *Option) EnumNone() string {
	if len(op.Route) < 3 || len(op.GoSdkName) < 2 {
		return ""
	}
	tag := op.EnumTag("none")
	return "No" + tag
}

func (op *Option) EnumDef() string {
	some := []string{}
	all := []string{}
	ret := []string{}
	for i, e := range op.Enums {
		tag := op.EnumTag(e)
		token := tag + FirstUpper(e)
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
		v := "\t\t" + op.EnumTag(e) + FirstUpper(e) + ": \"" + e + "\","
		ret = append(ret, v)
	}
	return strings.Join(ret, "\n")
}

func (op *Option) EnumList() string {
	ret := []string{}
	for _, e := range op.Enums {
		if e == "some" || e == "all" {
			continue
		}
		ret = append(ret, op.EnumTag(e)+FirstUpper(e))
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

func (op *Option) EnumTypes() []string {
	ret := []string{}
	for _, e := range op.Enums {
		opp := *op
		opp.GoName = e
		ret = append(ret, opp.ModeType())
	}
	return ret
}

func (op *Option) EnumCases() string {
	ret := []string{}
	for _, e := range op.Enums {
		if e == "some" || e == "all" {
			continue
		}
		v := op.EnumTag(e) + FirstUpper(e)
		ret = append(ret, "\t\tcase \""+e+"\":\n\t\t\tresult |= "+v)
	}
	v := "\t\tdefault:\n\t\t\treturn " + op.EnumNone() + ", fmt.Errorf(\"unknown " + op.LongName + ": %s\", val)"
	ret = append(ret, v)
	return strings.Join(ret, "\n")
}

func ValidateEnums(path string, enums []string) (bool, string) {
	if len(enums) == 0 {
		return true, ""
	}
	contents := file.AsciiFileToString(path)
	want := strings.Join(enums, "|")
	return strings.Contains(contents, want), want
}
